#include "StdAfx.h"
#include "FlowTomlFileNodes.h"
#include "Plugin.h"

#include "External/toml11/toml.hpp"

void CFlowNewDocument::GetConfiguration(SFlowNodeConfig& config)
{
    static const SInputPortConfig in_config[] = {
            InputPortConfig_Void("New",  _HELP("Clears any added TOML data and initializes a new TOML structure.")),
        { 0 }
    };
    static const SOutputPortConfig out_config[] = {
        OutputPortConfig_Void("Out"),
        { 0 }
    };
    config.sDescription = _HELP("Clears any added TOML data and initializes a new TOML structure.");
    config.pInputPorts = in_config;
    config.pOutputPorts = out_config;
    config.SetCategory(EFLN_APPROVED);
}

void CFlowNewDocument::ProcessEvent(EFlowEvent evt, SActivationInfo* pActInfo)
{
    switch (evt)
    {
    case eFE_Activate:
        if (IsPortActive(pActInfo, 0))
        {
            const auto pConfigurationInstance = CToml4CryenginePlugin::GetInstance();
            if (!pConfigurationInstance)
            {
                CryFatalError("Plugin is not initialized.");
                return;
            }

            // Replace with a fresh object.
            pConfigurationInstance->m_currentTomlData = toml::value();

            ActivateOutput(pActInfo, 0, 0);
        }
        break;
    }
}

void CFlowNewDocument::GetMemoryUsage(ICrySizer* s) const
{
    s->Add(*this);
}
