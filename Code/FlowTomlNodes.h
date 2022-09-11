#pragma once

#include <CryGame/IGameFramework.h>

//! Describes a "NewDocument" node to initialize new TOML documents.
class CFlowNewDocument : public CFlowBaseNode<eNCT_Singleton>
{
public:
    CFlowNewDocument(SActivationInfo* pActInfo) {};
    
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

REGISTER_FLOW_NODE(CFlowNewDocument::GetNodeName(), CFlowNewDocument)