#include "StdAfx.h"
#include "FlowTomlNodes.h"
#include "Plugin.h"

#include "External/toml11/toml.hpp"

void CFlowNewDocument::GetConfiguration(SFlowNodeConfig& config)
{
    static const SInputPortConfig in_config[] = {
            InputPortConfig_Void("New",  _HELP("Initializes a new TOML structure.")),
        { 0 }
    };
    static const SOutputPortConfig out_config[] = {
        OutputPortConfig<int>("Document ID", _HELP("Unique identifier for this document.")),
        { 0 }
    };
    config.sDescription = _HELP("Initializes a new TOML structure.");
    config.pInputPorts = in_config;
    config.pOutputPorts = out_config;
    config.SetCategory(EFLN_APPROVED);
}

void CFlowNewDocument::ProcessEvent(EFlowEvent evt, SActivationInfo* pActInfo)
{
    switch (evt)
    {
    case eFE_Activate:
        if (IsPortActive(pActInfo, static_cast<int>(EInputs::NewDocument)))
        {
            const auto pConfigurationInstance = CToml4CryenginePlugin::GetInstance();
            if (!pConfigurationInstance)
            {
                CryFatalError("Plugin is not initialized.");
                return;
            }

            ActivateOutput(pActInfo, static_cast<int>(EOutputs::DocumentId), pConfigurationInstance->registerNewTomlDocument());
        }
        break;
    }
}

void CFlowNewDocument::GetMemoryUsage(ICrySizer* s) const
{
    s->Add(*this);
}

const char* CFlowNewDocument::GetNodeName()
{
    return m_nodeName;
}
