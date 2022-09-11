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

REGISTER_FLOW_NODE(CFlowTomlNode_NewDocument::GetNodeName(), CFlowTomlNode_NewDocument)
REGISTER_FLOW_NODE(CFlowTomlNode_SetValue::GetNodeName(), CFlowTomlNode_SetValue)