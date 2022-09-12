#include "StdAfx.h"
#include "FlowTomlNodes.h"
#include "Toml4CryenginePlugin.h"
#include <ILevelSystem.h>

#include "TomlManager/External/toml11/toml.hpp"

void CFlowTomlNode_NewDocument::GetConfiguration(SFlowNodeConfig& config)
{
    static const SInputPortConfig in_config[] = {
        InputPortConfig_Void("New",  _HELP("Initializes a new TOML structure.")),
        { 0 }
    };
    static const SOutputPortConfig out_config[] = {
        OutputPortConfig<int>("DocumentID", _HELP("Unique identifier for this document."), "Document ID"),
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
            ActivateOutput(pActInfo, static_cast<int>(EOutputs::DocumentId), pPluginInstance->GetTomlManager()->RegisterNewTomlDocument());
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
        InputPortConfig<int>("DocumentID",  _HELP("Document to set new value to."), "Document ID"),
        InputPortConfig<string>("SectionName",  _HELP("[Optional] Name of the section to set the value to."), "Section Name"),
        InputPortConfig<string>("Key",  _HELP("Name of the key for the value."), "Key"),
        InputPortConfig_Void("Value",  _HELP("Value to set."), "Value"),
        { 0 }
    };
    static const SOutputPortConfig out_config[] = {
        OutputPortConfig<int>("DocumentID", _HELP("Executed when the value is set successfully. Unique identifier of the document."), "Document ID"),
        OutputPortConfig_Void("DocumentNotFound", _HELP("Executed when the specified document ID is incorrect."), "Document Not Found"),
        OutputPortConfig_Void("FailedToConvertValue", _HELP("Executed when failed to convert the specified value to string."), "Failed To Convert Value"),
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

            // Get inputs.
            int documentId = GetPortInt(pActInfo, static_cast<int>(EInputs::DocumentId));
            const auto sectionName = GetPortString(pActInfo, static_cast<int>(EInputs::SectionName));
            const auto keyName = GetPortString(pActInfo, static_cast<int>(EInputs::KeyName));
            const TFlowInputData valueData = GetPortAny(pActInfo, static_cast<int>(EInputs::Value));
            string value;
            if (!valueData.GetValueWithConversion(value))
            {
                ActivateOutput(pActInfo, static_cast<int>(EOutputs::FailedToConvertValue), 0);
                return;
            }

            // Set value.
            const auto optionalError
                = pPluginInstance->GetTomlManager()->SetValue(documentId, std::string(keyName), std::string(value), std::string(sectionName));

            if (!optionalError.has_value())
            {
                // Trigger output pin.
                ActivateOutput(pActInfo, static_cast<int>(EOutputs::DocumentId), 0);
            }
            else
            {
                switch (optionalError.value())
                {
                case CTomlManager::SetValueError::KeyEmpty:
                    CryWarning(
                        VALIDATOR_MODULE_FLOWGRAPH,
                        VALIDATOR_WARNING,
                        "The specified key name cannot be empty, unable to set value (document %d).", documentId);
                    break;
                case CTomlManager::SetValueError::DocumentNotFound:
                    ActivateOutput(pActInfo, static_cast<int>(EOutputs::DocumentNotFound), 0);
                    break;
                }
            }
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

void CFlowTomlNode_SaveDocument::GetConfiguration(SFlowNodeConfig& config)
{
    static const SInputPortConfig in_config[] = {
        InputPortConfig<int>("DocumentID",  _HELP("Document to save."), "Document ID"),
        InputPortConfig<string>("FileName", _HELP("Name of the file without \".toml\" extension for the document."), "File Name"),
        InputPortConfig<string>("DirectoryName", _HELP("Usually your game name. Directory for file (will be appended to the base path)."), "Directory Name"),
        InputPortConfig<bool>("Overwrite", true,  _HELP("Whether to overwrite already existing file or not."), "Overwrite"),
        { 0 }
    };
    static const SOutputPortConfig out_config[] = {
        OutputPortConfig_Void("Done", _HELP("Executed if successfully finished the operation."), "Done"),
        OutputPortConfig_Void("DocumentNotFound", _HELP("Executed when the specified document ID is incorrect."), "Document Not Found"),
        OutputPortConfig_Void("FailedToGetBasePath", _HELP("Executed when failed to get base path."), "Failed To Get Base Path"),
        OutputPortConfig_Void("UnableToCreateFile", _HELP("Executed when failed to create/open the output file."), "Unable To Create File"),
        { 0 }
    };
    config.sDescription = _HELP("Saves TOML document to file, base path is \"%localappdata%\" on Windows, \"%HOME%/.config\" on Linux. Use NewDocument to get document ID.");
    config.pInputPorts = in_config;
    config.pOutputPorts = out_config;
    config.SetCategory(EFLN_APPROVED);
}

void CFlowTomlNode_SaveDocument::ProcessEvent(EFlowEvent evt, SActivationInfo* pActInfo)
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

            // Get inputs.
            const auto documentId = GetPortInt(pActInfo, static_cast<int>(EInputs::DocumentId));
            const auto fileName = GetPortString(pActInfo, static_cast<int>(EInputs::FileName));
            const auto directoryName = GetPortString(pActInfo, static_cast<int>(EInputs::DirectoryName));
            const auto bOverwrite = GetPortBool(pActInfo, static_cast<int>(EInputs::Overwrite));

            // Save document.
            const auto optionalError
                = pPluginInstance->GetTomlManager()->SaveDocument(documentId, std::string(fileName), std::string(directoryName), bOverwrite);

            if (!optionalError.has_value())
            {
                // Trigger output pin.
                ActivateOutput(pActInfo, static_cast<int>(EOutputs::Done), 0);
            }
            else
            {
                switch (optionalError.value())
                {
                case CTomlManager::SaveDocumentError::FileNameEmpty:
                    CryWarning(
                        VALIDATOR_MODULE_FLOWGRAPH,
                        VALIDATOR_WARNING,
                        "The specified file name cannot be empty, unable to set value (document %d).", documentId);
                    break;
                case CTomlManager::SaveDocumentError::DirectoryNameEmpty:
                    CryWarning(
                        VALIDATOR_MODULE_FLOWGRAPH,
                        VALIDATOR_WARNING,
                        "The specified directory name cannot be empty, unable to set value (document %d).", documentId);
                    break;
                case CTomlManager::SaveDocumentError::DocumentNotFound:
                    ActivateOutput(pActInfo, static_cast<int>(EOutputs::DocumentNotFound), 0);
                    break;
                case CTomlManager::SaveDocumentError::FailedToGetBasePath:
                    ActivateOutput(pActInfo, static_cast<int>(EOutputs::FailedToGetBasePath), 0);
                    break;
                case CTomlManager::SaveDocumentError::UnableToCreateFile:
                    ActivateOutput(pActInfo, static_cast<int>(EOutputs::UnableToCreateFile), 0);
                    break;
                }
            }
        }
        break;
    }
}

void CFlowTomlNode_SaveDocument::GetMemoryUsage(ICrySizer* s) const
{
    s->Add(*this);
}

const char* CFlowTomlNode_SaveDocument::GetNodeName()
{
    return m_nodeName;
}