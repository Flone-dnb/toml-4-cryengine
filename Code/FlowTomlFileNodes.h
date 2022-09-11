#pragma once

#include <CryGame/IGameFramework.h>

class CFlowNewDocument : public CFlowBaseNode<eNCT_Singleton>
{
public:
    CFlowNewDocument(SActivationInfo* pActInfo) {};

    virtual void GetConfiguration(SFlowNodeConfig& config);
    virtual void ProcessEvent(EFlowEvent evt, SActivationInfo* pActInfo);
    virtual void GetMemoryUsage(ICrySizer* s) const;
};

REGISTER_FLOW_NODE("TOML:NewDocument", CFlowNewDocument)