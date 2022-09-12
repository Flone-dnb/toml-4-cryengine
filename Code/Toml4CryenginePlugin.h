// Copyright 2016-2018 Crytek GmbH / Crytek Group. All rights reserved.
#pragma once

#include <CrySystem/ICryPlugin.h>
#include <CryGame/IGameFramework.h>
#include <CryEntitySystem/IEntityClass.h>
#include "TomlManager/TomlManager.h"

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

	//! Returns TOML manager to work with TOML files.
	//! 
	//! \return TOML manager.
	CTomlManager* GetTomlManager();
	
	// Cry::IEnginePlugin
	virtual bool Initialize(SSystemGlobalEnvironment& env, const SSystemInitParams& initParams) override;
	virtual const char* GetName() const override;
	// ~Cry::IEnginePlugin
	
	// ISystemEventListener
	virtual void OnSystemEvent(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam) override;
	// ~ISystemEventListener

private:

	//! Plugin name.
	static inline const char* m_pluginName = "TOML4CRYENGINE";

	//! Manager to work with TOML files.
	CTomlManager m_tomlManager;

	// Include Flownode registering and unregistering (just needs to be included once in total).
	PLUGIN_FLOWNODE_REGISTER
	PLUGIN_FLOWNODE_UNREGISTER
};