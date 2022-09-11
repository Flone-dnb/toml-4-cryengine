#include "StdAfx.h"
#include "FlowTomlNodes.h"
#include "Plugin.h"

#include "External/toml11/toml.hpp"

void CFlowTomlNode_NewDocument::GetConfiguration(SFlowNodeConfig& config)
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

void CFlowTomlNode_NewDocument::ProcessEvent(EFlowEvent evt, SActivationInfo* pActInfo)
{
    switch (evt)
    {
    case eFE_Activate:
        if (IsPortActive(pActInfo, static_cast<int>(EInputs::NewDocument)))
        {
            // Get plugin instance.
            const auto pPluginInstance = CToml4CryenginePlugin::GetInstance();
            if (!pPluginInstance)
            {
                CryFatalError("Plugin is not initialized.");
                return;
            }

            // Trigger output pin.
            ActivateOutput(pActInfo, static_cast<int>(EOutputs::DocumentId), pPluginInstance->RegisterNewTomlDocument());
        }
        break;
    }
}

void CFlowTomlNode_NewDocument::GetMemoryUsage(ICrySizer* s) const
{
    s->Add(*this);
}

const char* CFlowTomlNode_NewDocument::GetNodeName()
{
    return m_nodeName;
}

void CFlowTomlNode_SetValue::GetConfiguration(SFlowNodeConfig& config)
{
    static const SInputPortConfig in_config[] = {
        InputPortConfig<int>("Document ID",  _HELP("Document to set new value to.")),
        InputPortConfig<string>("Section Name",  _HELP("[Optional] Name of the section to set the value to.")),
        InputPortConfig<string>("Key",  _HELP("Name of the key for the value.")),
        InputPortConfig_Void("Value",  _HELP("Value to set.")),
        { 0 }
    };
    static const SOutputPortConfig out_config[] = {
        OutputPortConfig<int>("Document ID", _HELP("Executed when the value is set successfully. Unique identifier of the document.")),
        OutputPortConfig_Void("Document Not Found", _HELP("Executed when the specified document ID is incorrect.")),
        OutputPortConfig_Void("Failed To Convert Value", _HELP("Executed when failed to convert the specified value to string.")),
        { 0 }
    };
    config.sDescription = _HELP("Sets a value to TOML document. Use NewDocument to get document ID.");
    config.pInputPorts = in_config;
    config.pOutputPorts = out_config;
    config.SetCategory(EFLN_APPROVED);
}

void CFlowTomlNode_SetValue::ProcessEvent(EFlowEvent evt, SActivationInfo* pActInfo)
{
    switch (evt)
    {
    case eFE_Activate:
        if (IsPortActive(pActInfo, static_cast<int>(EInputs::DocumentId)))
        {
            // Get plugin instance.
            const auto pPluginInstance = CToml4CryenginePlugin::GetInstance();
            if (!pPluginInstance)
            {
                CryFatalError("Plugin is not initialized.");
                return;
            }

            // See if this document exists.
            int documentId = GetPortInt(pActInfo, static_cast<int>(EInputs::DocumentId));
            const auto pTomlData = pPluginInstance->GetTomlData(documentId);
            if (!pTomlData)
            {
                ActivateOutput(pActInfo, static_cast<int>(EOutputs::DocumentNotFound), 0);
                return;
            }

            // Get inputs.
            const auto sectionName = GetPortString(pActInfo, static_cast<int>(EInputs::SectionName));
            const auto keyName = GetPortString(pActInfo, static_cast<int>(EInputs::KeyName));
            const TFlowInputData valueData = GetPortAny(pActInfo, static_cast<int>(EInputs::Value));
            string value;
            if (!valueData.GetValueWithConversion(value))
            {
                ActivateOutput(pActInfo, static_cast<int>(EOutputs::FailedToConvertValue), 0);
                return;
            }

            // Check if key is not empty.
            if (keyName.empty())
            {
                CryWarning(
                    VALIDATOR_MODULE_FLOWGRAPH,
                    VALIDATOR_WARNING,
                    "The specified key name cannot be empty, unable to set value (document %d).", documentId);
                return;
            }

            // Set value to TOML data.
            if (sectionName.empty())
            {
                pTomlData->operator[](keyName.data()) = toml::value(value);
            }
            else 
            {
                pTomlData->operator[](sectionName.data()).operator[](keyName.data()) = toml::value(value);
            }

            // Trigger output pin.
            ActivateOutput(pActInfo, static_cast<int>(EOutputs::DocumentId), 0);
        }
        break;
    }
}

void CFlowTomlNode_SetValue::GetMemoryUsage(ICrySizer* s) const
{
    s->Add(*this);
}

const char* CFlowTomlNode_SetValue::GetNodeName()
{
    return m_nodeName;
}