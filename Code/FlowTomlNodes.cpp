#include "StdAfx.h"
#include "FlowTomlNodes.h"
#include "Plugin.h"
#include <ILevelSystem.h>

#include "External/toml11/toml.hpp"

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

            // Check if key is empty.
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
                pTomlData->operator[](keyName.data()) = toml::value(std::string(value));
            }
            else 
            {
                pTomlData->operator[](sectionName.data()).operator[](keyName.data()) = toml::value(std::string(value));
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

void CFlowTomlNode_SaveDocument::GetConfiguration(SFlowNodeConfig& config)
{
    static const SInputPortConfig in_config[] = {
        InputPortConfig<int>("DocumentID",  _HELP("Document to save."), "Document ID"),
        InputPortConfig<string>("FileName", _HELP("Name of the file without \".toml\" extension for the document."), "File Name"),
        InputPortConfig<int>("Location", 0, _HELP("Local directory to save the document to."), "Location", m_tomlFileLocationEnum),
        InputPortConfig<bool>("Overwrite", true,  _HELP("Value to set."), "Overwrite"),
        { 0 }
    };
    static const SOutputPortConfig out_config[] = {
        OutputPortConfig_Void("Done", _HELP("Executed if successfully finished the operation."), "Done"),
        OutputPortConfig_Void("DocumentNotFound", _HELP("Executed when the specified document ID is incorrect."), "Document Not Found"),
        OutputPortConfig_Void("FailedToGetCurrentLevel", _HELP("Executed when location is Map and we failed to get current level."), "Failed To Get Current Level"),
        OutputPortConfig_Void("UnableToCreateFile", _HELP("Executed when failed to create/open the output file."), "Unable To Create File"),
        { 0 }
    };
    config.sDescription = _HELP("Saves TOML document to file. Use NewDocument to get document ID.");
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

            // See if this document exists.
            int documentId = GetPortInt(pActInfo, static_cast<int>(EInputs::DocumentId));
            const auto pTomlData = pPluginInstance->GetTomlData(documentId);
            if (!pTomlData)
            {
                ActivateOutput(pActInfo, static_cast<int>(EOutputs::DocumentNotFound), 0);
                return;
            }

            // Check if TOML data is empty.
            if (pTomlData->is_uninitialized())
            {
                CryWarning(
                    VALIDATOR_MODULE_FLOWGRAPH,
                    VALIDATOR_WARNING,
                    "The specified TOML document is empty, nothing to save (document %d).", documentId);
                return;
            }

            // Get inputs.
            const auto fileName = GetPortString(pActInfo, static_cast<int>(EInputs::FileName));
            const auto location = GetPortInt(pActInfo, static_cast<int>(EInputs::Location));
            const auto bOverwrite = GetPortBool(pActInfo, static_cast<int>(EInputs::Overwrite));

            // Check if file name is empty.
            if (fileName.empty())
            {
                CryWarning(
                    VALIDATOR_MODULE_FLOWGRAPH,
                    VALIDATOR_WARNING,
                    "The specified file name cannot be empty, unable to save document (document %d).", documentId);
                return;
            }

            // Qualify path.
            string savePath;
            switch (location)
            {
            case ETomlFileLocation::MAP:
            {
                if (gEnv->IsEditor())
                {
                    char* levelName;
                    char* levelPath;
                    gEnv->pGameFramework->GetEditorLevel(&levelName, &levelPath);
                    savePath = levelPath;
                }
                else
                {
                    ILevelInfo* pLevel = gEnv->pGameFramework->GetILevelSystem()->GetCurrentLevel();
                    if (pLevel)
                    {
                        savePath = pLevel->GetPath();
                    }
                    else
                    {
                        ActivateOutput(pActInfo, static_cast<int>(EOutputs::FailedToGetCurrentLevel), 0);
                        return;
                    }
                }
            }
            break;
            case ETomlFileLocation::GAME:
            {
                savePath = PathUtil::GetGameFolder();
            }
            break;
            case ETomlFileLocation::USER:
            {
                savePath = "%USER%/";
            }
            break;
            }

            savePath = PathUtil::Make(savePath.c_str(), fileName, ".toml");

            // Check if we need to overwrite the file.
            ICryPak* pPak = gEnv->pCryPak;
            if (bOverwrite || !pPak->IsFileExist(savePath.c_str()))
            {
                std::ofstream outFile(savePath, std::ios::binary);
                if (!outFile.is_open()) {
                    CryWarning(
                        VALIDATOR_MODULE_FLOWGRAPH,
                        VALIDATOR_WARNING,
                        "Unable to create/open file %s, unable to save document (document %d).", savePath.c_str(), documentId);
                    ActivateOutput(pActInfo, static_cast<int>(EOutputs::UnableToCreateFile), 0);
                    return;
                }
                outFile << *pTomlData;
                outFile.close();

                CryLogAlways("Saved TOML document at %s", savePath.c_str());
            }

            // Trigger output pin.
            ActivateOutput(pActInfo, static_cast<int>(EOutputs::Done), 0);
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