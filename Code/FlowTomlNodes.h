#pragma once

#include <CryGame/IGameFramework.h>

//! Describes the "NewDocument" node to initialize new TOML documents.
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

//! Describes the "SetValue" node to set value to TOML document.
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

//! Describes the "GetValue" node to get value from TOML document.
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
        ValueTypeMismatch,
    };
};

//! Describes the "SaveDocument" node to save TOML document to file.
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
        EnableBackup,
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

//! Describes the "OpenDocument" node to open TOML document from file.
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

//! Describes the "CloseDocument" node to close TOML document and invalidate its ID.
class CFlowTomlNode_CloseDocument : public CFlowBaseNode<eNCT_Singleton>
{
public:
    CFlowTomlNode_CloseDocument(SActivationInfo* pActInfo) {};

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
    static inline const char* m_nodeName = "TOML:CloseDocument";

    //! Input ports of this node.
    enum class EInputs : int
    {
        Close = 0,
        DocumentId,
    };

    //! Output ports of this node.
    enum class EOutputs : int
    {
        Closed = 0,
        DocumentNotFound,
    };
};

//! Describes the "GetDirectoryPathForDocuments" node to get path to the directory where we store documents.
class CFlowTomlNode_GetDirectoryPathForDocuments : public CFlowBaseNode<eNCT_Singleton>
{
public:
    CFlowTomlNode_GetDirectoryPathForDocuments(SActivationInfo* pActInfo) {};

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
    static inline const char* m_nodeName = "TOML:GetDirectoryPathForDocuments";

    //! Input ports of this node.
    enum class EInputs : int
    {
        Get = 0,
        DirectoryName,
    };

    //! Output ports of this node.
    enum class EOutputs : int
    {
        Path = 0,
        Failed
    };
};

//! Describes the "GetAllDocuments" node to get array of filenames of found documents.
class CFlowTomlNode_GetAllDocuments : public CFlowBaseNode<eNCT_Singleton>
{
public:
    CFlowTomlNode_GetAllDocuments(SActivationInfo* pActInfo) {};

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
    static inline const char* m_nodeName = "TOML:GetAllDocuments";

    //! Input ports of this node.
    enum class EInputs : int
    {
        Get = 0,
        DirectoryName,
    };

    //! Output ports of this node.
    enum class EOutputs : int
    {
        Array = 0,
        FailedToGetBasePath,
        DocumentNamesContainPipes,
    };
};

//! Describes the "RemoveDocument" node to remove the document and its backup from disk.
class CFlowTomlNode_RemoveDocument : public CFlowBaseNode<eNCT_Singleton>
{
public:
    CFlowTomlNode_RemoveDocument(SActivationInfo* pActInfo) {};

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
    static inline const char* m_nodeName = "TOML:RemoveDocument";

    //! Input ports of this node.
    enum class EInputs : int
    {
        Remove = 0,
        FileName,
        DirectoryName,
    };

    //! Output ports of this node.
    enum class EOutputs : int
    {
        Removed = 0,
        NotFound,
    };
};

REGISTER_FLOW_NODE(CFlowTomlNode_NewDocument::GetNodeName(), CFlowTomlNode_NewDocument)
REGISTER_FLOW_NODE(CFlowTomlNode_SetValue::GetNodeName(), CFlowTomlNode_SetValue)
REGISTER_FLOW_NODE(CFlowTomlNode_GetValue::GetNodeName(), CFlowTomlNode_GetValue)
REGISTER_FLOW_NODE(CFlowTomlNode_SaveDocument::GetNodeName(), CFlowTomlNode_SaveDocument)
REGISTER_FLOW_NODE(CFlowTomlNode_OpenDocument::GetNodeName(), CFlowTomlNode_OpenDocument)
REGISTER_FLOW_NODE(CFlowTomlNode_CloseDocument::GetNodeName(), CFlowTomlNode_CloseDocument)
REGISTER_FLOW_NODE(CFlowTomlNode_GetDirectoryPathForDocuments::GetNodeName(), CFlowTomlNode_GetDirectoryPathForDocuments)
REGISTER_FLOW_NODE(CFlowTomlNode_GetAllDocuments::GetNodeName(), CFlowTomlNode_GetAllDocuments)
REGISTER_FLOW_NODE(CFlowTomlNode_RemoveDocument::GetNodeName(), CFlowTomlNode_RemoveDocument)