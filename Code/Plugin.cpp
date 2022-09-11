// Copyright 2016-2017 Crytek GmbH / Crytek Group. All rights reserved.
#include "StdAfx.h"
#include "Plugin.h"

#include <CrySchematyc/Env/IEnvRegistry.h>
#include <CrySchematyc/Env/EnvPackage.h>
#include <CrySchematyc/Utils/SharedString.h>

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