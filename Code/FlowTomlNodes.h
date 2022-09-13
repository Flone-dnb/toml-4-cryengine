#pragma once

#include <CryGame/IGameFramework.h>

//! Describes a "NewDocument" node to initialize new TOML documents.
class CFlowTomlNode_NewDocument : public CFlowBaseNode<eNCT_Singleton>
{
public:
    CFlowTomlNode_NewDocument(SActivationInfo* pActInfo) {};
    
    //! Returns node configuration (input, output ports, description, etc.).
    virtual void GetConfiguration(SFlowNodeConfig& config) override;

    //! Processes Flow Graph events.
    virtual void ProcessEvent(EFlowEvent evt, SActivationInfo* pActInfo) override;

    //! Returns memory usage of this object.
    virtual void GetMemoryUsage(ICrySizer* s) const override;

    //! Returns name of this node.
    //! 
    //! \return Name of this node.
    static const char* GetNodeName();

private:

    //! Name of this node.
    static inline const char* m_nodeName = "TOML:NewDocument";

    //! Input ports of this node.
    enum class EInputs : int
    {
        NewDocument = 0,
    };

    //! Output ports of this node.
    enum class EOutputs : int
    {
        DocumentId = 0,
    };
};

//! Describes a "SetValue" node to set value to TOML document.
class CFlowTomlNode_SetValue : public CFlowBaseNode<eNCT_Singleton>
{
public:
    CFlowTomlNode_SetValue(SActivationInfo* pActInfo) {};

    //! Returns node configuration (input, output ports, description, etc.).
    virtual void GetConfiguration(SFlowNodeConfig& config) override;

    //! Processes Flow Graph events.
    virtual void ProcessEvent(EFlowEvent evt, SActivationInfo* pActInfo) override;

    //! Returns memory usage of this object.
    virtual void GetMemoryUsage(ICrySizer* s) const override;

    //! Returns name of this node.
    //! 
    //! \return Name of this node.
    static const char* GetNodeName();

private:

    //! Name of this node.
    static inline const char* m_nodeName = "TOML:SetValue";

    //! Input ports of this node.
    enum class EInputs : int
    {
        DocumentId = 0,
        SectionName,
        KeyName,
        Value,
    };

    //! Output ports of this node.
    enum class EOutputs : int
    {
        DocumentId = 0,
        DocumentNotFound,
        FailedToConvertValue,
    };
};

//! Describes a "GetValue" node to get value from TOML document.
class CFlowTomlNode_GetValue : public CFlowBaseNode<eNCT_Singleton>
{
public:
    CFlowTomlNode_GetValue(SActivationInfo* pActInfo) {};

    //! Returns node configuration (input, output ports, description, etc.).
    virtual void GetConfiguration(SFlowNodeConfig& config) override;

    //! Processes Flow Graph events.
    virtual void ProcessEvent(EFlowEvent evt, SActivationInfo* pActInfo) override;

    //! Returns memory usage of this object.
    virtual void GetMemoryUsage(ICrySizer* s) const override;

    //! Returns name of this node.
    //! 
    //! \return Name of this node.
    static const char* GetNodeName();

private:

    //! Name of this node.
    static inline const char* m_nodeName = "TOML:GetValue";

    //! Input ports of this node.
    enum class EInputs : int
    {
        DocumentId = 0,
        SectionName,
        KeyName,
    };

    //! Output ports of this node.
    enum class EOutputs : int
    {
        Value = 0,
        DocumentNotFound,
        ValueNotFound,
        ValueTypeNotString,
    };
};

//! Describes a "SaveDocument" node to save TOML document to file.
class CFlowTomlNode_SaveDocument : public CFlowBaseNode<eNCT_Singleton>
{
public:
    CFlowTomlNode_SaveDocument(SActivationInfo* pActInfo) {};

    //! Returns node configuration (input, output ports, description, etc.).
    virtual void GetConfiguration(SFlowNodeConfig& config) override;

    //! Processes Flow Graph events.
    virtual void ProcessEvent(EFlowEvent evt, SActivationInfo* pActInfo) override;

    //! Returns memory usage of this object.
    virtual void GetMemoryUsage(ICrySizer* s) const override;

    //! Returns name of this node.
    //! 
    //! \return Name of this node.
    static const char* GetNodeName();

private:

    //! Name of this node.
    static inline const char* m_nodeName = "TOML:SaveDocument";

    //! Input ports of this node.
    enum class EInputs : int
    {
        DocumentId = 0,
        FileName,
        DirectoryName,
        Overwrite,
    };

    //! Output ports of this node.
    enum class EOutputs : int
    {
        Done = 0,
        DocumentNotFound,
        FailedToGetBasePath,
        UnableToCreateFile,
    };
};

//! Describes a "OpenDocument" node to open TOML document from file.
class CFlowTomlNode_OpenDocument : public CFlowBaseNode<eNCT_Singleton>
{
public:
    CFlowTomlNode_OpenDocument(SActivationInfo* pActInfo) {};

    //! Returns node configuration (input, output ports, description, etc.).
    virtual void GetConfiguration(SFlowNodeConfig& config) override;

    //! Processes Flow Graph events.
    virtual void ProcessEvent(EFlowEvent evt, SActivationInfo* pActInfo) override;

    //! Returns memory usage of this object.
    virtual void GetMemoryUsage(ICrySizer* s) const override;

    //! Returns name of this node.
    //! 
    //! \return Name of this node.
    static const char* GetNodeName();

private:

    //! Name of this node.
    static inline const char* m_nodeName = "TOML:OpenDocument";

    //! Input ports of this node.
    enum class EInputs : int
    {
        Open = 0,
        FileName,
        DirectoryName,
    };

    //! Output ports of this node.
    enum class EOutputs : int
    {
        DocumentId = 0,
        FileNotFound,
        FailedToGetBasePath,
        ParsingFailed,
    };
};

REGISTER_FLOW_NODE(CFlowTomlNode_NewDocument::GetNodeName(), CFlowTomlNode_NewDocument)
REGISTER_FLOW_NODE(CFlowTomlNode_SetValue::GetNodeName(), CFlowTomlNode_SetValue)
REGISTER_FLOW_NODE(CFlowTomlNode_GetValue::GetNodeName(), CFlowTomlNode_GetValue)
REGISTER_FLOW_NODE(CFlowTomlNode_SaveDocument::GetNodeName(), CFlowTomlNode_SaveDocument)
REGISTER_FLOW_NODE(CFlowTomlNode_OpenDocument::GetNodeName(), CFlowTomlNode_OpenDocument)