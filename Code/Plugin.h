// Copyright 2016-2018 Crytek GmbH / Crytek Group. All rights reserved.
#pragma once

#include <unordered_map>
#include <mutex>
#include <CrySystem/ICryPlugin.h>
#include <CryGame/IGameFramework.h>
#include <CryEntitySystem/IEntityClass.h>

#include "External/toml11/toml.hpp"

class CToml4CryenginePlugin 
	: public Cry::IEnginePlugin
	, public ISystemEventListener
{
public:
	CToml4CryenginePlugin();

	CRYINTERFACE_SIMPLE(Cry::IEnginePlugin)
	CRYGENERATE_SINGLETONCLASS_GUID(CToml4CryenginePlugin, "Toml4Cryengine", "372e563d-6002-4e67-8710-1b0e33b15912"_cry_guid)

	virtual ~CToml4CryenginePlugin();

	//! Returns an instance of this class.
	//! 
	//! \return Instance of this class.
	static CToml4CryenginePlugin* GetInstance();

	//! Registers a new TOML document and returns this document's unique ID.
	//! 
	//! \return New document ID.
	int registerNewTomlDocument();
	
	// Cry::IEnginePlugin
	virtual bool Initialize(SSystemGlobalEnvironment& env, const SSystemInitParams& initParams) override;
	// ~Cry::IEnginePlugin
	
	// ISystemEventListener
	virtual void OnSystemEvent(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam) override;
	// ~ISystemEventListener

private:

	//! Created but not saved yet TOML documents.
	std::unordered_map<size_t, toml::value> m_tomlDocuments;

	//! ID for the next created TOML document.
	int m_nextTomlDocumentId = 0;

	//! Mutex for read/write operations on TOML documents and IDs.
	std::mutex m_mtxTomlDocuments;

	// Include Flownode registering and unregistering (just needs to be included once in total).
	PLUGIN_FLOWNODE_REGISTER
	PLUGIN_FLOWNODE_UNREGISTER
};