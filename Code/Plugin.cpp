// Copyright 2016-2017 Crytek GmbH / Crytek Group. All rights reserved.
#include "StdAfx.h"
#include "Plugin.h"

#include <CrySchematyc/Env/IEnvRegistry.h>
#include <CrySchematyc/Env/EnvPackage.h>
#include <CrySchematyc/Utils/SharedString.h>

#if defined(WIN32)
#include <ShlObj.h>
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")
#elif __linux__
#include <unistd.h>
#include <sys/types.h>
#include <cstddef>
#include <cstdlib>
#endif

// Included only once per DLL module.
#include <CryCore/Platform/platform_impl.inl>

static CToml4CryenginePlugin* g_pInstance = nullptr;

CToml4CryenginePlugin* CToml4CryenginePlugin::GetInstance()
{
	return g_pInstance;
}

CToml4CryenginePlugin::CToml4CryenginePlugin()
{
	g_pInstance = this;
}

CToml4CryenginePlugin::~CToml4CryenginePlugin()
{
	gEnv->pSystem->GetISystemEventDispatcher()->RemoveListener(this);

	if (gEnv->pSchematyc)
	{
		gEnv->pSchematyc->GetEnvRegistry().DeregisterPackage(CToml4CryenginePlugin::GetCID());
	}

	g_pInstance = nullptr;
}

int CToml4CryenginePlugin::RegisterNewTomlDocument()
{
	std::scoped_lock guard(m_mtxTomlDocuments);

	// Get new document ID.
	int newDocumentId = m_nextTomlDocumentId;
	m_nextTomlDocumentId += 1;

	// Create a fresh TOML object.
	m_tomlDocuments[newDocumentId] = toml::value();

	return newDocumentId;
}

toml::value* CToml4CryenginePlugin::GetTomlData(int DocumentId) {
	std::scoped_lock guard(m_mtxTomlDocuments);

	const auto it = m_tomlDocuments.find(DocumentId);
	if (it == m_tomlDocuments.end())
	{
		return nullptr;
	}

	return &it->second;
}

const char* CToml4CryenginePlugin::GetName() const
{
	return m_pluginName;
}

std::optional<std::filesystem::path> CToml4CryenginePlugin::GetDirectoryForConfigs()
{
	std::filesystem::path directoryPath;

#if defined(WIN32)

	// Try to get AppData folder.
	PWSTR pathTmp;
	const HRESULT result = SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, nullptr, &pathTmp);
	if (result != S_OK) {
		CoTaskMemFree(pathTmp);

		CryLogAlways("[%s]: failed to get directory for configs, error code: %ld", GetName(), result);
		return {};
	}

	directoryPath = pathTmp;

	CoTaskMemFree(pathTmp);

#elif __linux__

	const auto sHomePath = std::string(getenv("HOME"));
	if (sHomePath.empty()) {
		CryLogAlways("[%s]: environment variable HOME is not set", GetName());
		return {};
	}

	directoryPath = sHomePath + "/.config/";

#endif

	if (!std::filesystem::exists(directoryPath)) {
		std::filesystem::create_directories(directoryPath);
	}

	return directoryPath;
}

bool CToml4CryenginePlugin::Initialize(SSystemGlobalEnvironment& env, const SSystemInitParams& initParams)
{
	gEnv->pSystem->GetISystemEventDispatcher()->RegisterListener(this ,"CToml4CryenginePlugin");

	return true;
}

void CToml4CryenginePlugin::OnSystemEvent(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam)
{
	switch (event)
	{
	case ESYSTEM_EVENT_REGISTER_SCHEMATYC_ENV:
	{
		// Register all components that belong to this plug-in
		auto staticAutoRegisterLambda = [](Schematyc::IEnvRegistrar& registrar)
		{
			// Call all static callback registered with the CRY_STATIC_AUTO_REGISTER_WITH_PARAM
			Detail::CStaticAutoRegistrar<Schematyc::IEnvRegistrar&>::InvokeStaticCallbacks(registrar);
		};

		if (gEnv->pSchematyc)
		{
			gEnv->pSchematyc->GetEnvRegistry().RegisterPackage(
				stl::make_unique<Schematyc::CEnvPackage>(
					CToml4CryenginePlugin::GetCID(),
					"EntityComponents",
					"Crytek GmbH",
					"Components",
					staticAutoRegisterLambda
					)
			);
		}
	}
	break;
	}
}

CRYREGISTER_SINGLETON_CLASS(CToml4CryenginePlugin)