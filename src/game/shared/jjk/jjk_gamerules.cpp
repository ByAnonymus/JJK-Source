#include "cbase.h"
#include "jjk_gamerules.h"

#ifdef CLIENT_DLL
#include "c_jjk_player.h"
#else
#include "jjk_player.h"
#endif

REGISTER_GAMERULES_CLASS(CJJKRules);

BEGIN_NETWORK_TABLE_NOBASE(CJJKRules, DT_CJJKRules)
END_NETWORK_TABLE()

LINK_ENTITY_TO_CLASS(jjk_rules, CJJKGameRulesProxy);
IMPLEMENT_NETWORKCLASS_ALIASED(CJJKGameRulesProxy, DT_CJJKGameRuleProxy)

#ifdef CLIENT_DLL
void RecvProxy_CJJKRules(const RecvProp *pProp, void **pOut, void *pData, int objectID)
{
    CJJKRules *pRules = CJJKRules();
    Assert(pRules);
    *pOut = pRules;
}

BEGIN_RECV_TABLE(CJJKGameRulesProxy, DT_CJJKGameRuleProxy)
    RecvPropDataTable("jjk_rules_data", 0, 0, &REFERENCE_RECV_TABLE(DT_CJJKRules), RecvProxy_CJJKRules),
END_RECV_TABLE()
#else
void* SendProxy_CJJKRules(const SendProp *pProp, const void *pData, DVariant *pOut, int iElement, int objectID)
{
    CJJKRules *pRules = CJJKRules();
    Assert(pRules);
    return pRules;
}

BEGIN_SEND_TABLE(CJJKGameRulesProxy, DT_CJJKGameRuleProxy)
SendPropDataTable("jjk_rules_data", 0, &REFERENCE_SEND_TABLE(DT_CJJKRules), SendProxy_CJJKRules)
END_SEND_TABLE()
#endif

CJJKRules::CJJKRules()
{   
}

CJJKRules::~CJJKRules()
{
}

bool CJJKRules::ClientCommand( CBaseEntity *pEdict, const CCommand &args )
{
#ifndef CLIENT_DLL
    if(BaseClass::ClientCommand(pEdict, args))
        return true;

    CJJKPlayer *pPlayer =(CJJKPlayer *)pEdict;
    if (pPlayer->ClientCommand(args))
        return true;
#endif
return false;
}