#ifndef JJK_GAMERULES_H
#define JJK_GAMERULES_H
#pragma once

#include "gamerules.h"
#include "teamplay_gamerules.h"
#include "gamevars_shared.h"

#ifndef CLIENT_DLL
#include "jjk_player.h"
#endif

#ifdef CLIENT_DLL
    #define CJJKRules C_JJKRules
    #define CJJKGameRulesProxt C_JJKGameRulesProxy
#endif

class CJJKGameRulesProxy : public CGameRulesProxy
{
    public:
    DECLARE_CLASS(CJJKGameRulesProxy, CGameRulesProxy);
    DECLARE_NETWORKCLASS();
};

class JJKViewVectors : public CViewVectors
{
public:
	JJKViewVectors( 
		Vector vView,
		Vector vHullMin,
		Vector vHullMax,
		Vector vDuckHullMin,
		Vector vDuckHullMax,
		Vector vDuckView,
		Vector vObsHullMin,
		Vector vObsHullMax,
		Vector vDeadViewHeight,
		Vector vCrouchTraceMin,
		Vector vCrouchTraceMax ) :
			CViewVectors( 
				vView,
				vHullMin,
				vHullMax,
				vDuckHullMin,
				vDuckHullMax,
				vDuckView,
				vObsHullMin,
				vObsHullMax,
				vDeadViewHeight )
	{
		m_vCrouchTraceMin = vCrouchTraceMin;
		m_vCrouchTraceMax = vCrouchTraceMax;
	}

	Vector m_vCrouchTraceMin;
	Vector m_vCrouchTraceMax;	
};

class CJJKRules : public CTeamplayRules
{
	public:
	DECLARE_CLASS(CJJKRules, CTeamplayRules);

	#ifdef CLIENT_DLL

	DECLARE_CLIENTCLASS_NOBASE();
	#else

	DECLARE_SERVERCLASS_NOBASE();
	#endif

	CJJKRules();
	virtual ~CJJKRules();

	virtual void Precache();
	virtual bool ShouldCollide(int collisionGroup0, int collisionGroup1 );
	virtual bool ClientCommand( CBaseEntity *pEdict, const CCommand &args );
	
	virtual void Think(void);
	virtual void CreateStandardEntities( void );
	virtual void ClientSettingsChanged( CBasePlayer *pPlayer );
	virtual int PlayerRelationship( CBaseEntity *pPlayer, CBaseEntity *pTarget );
	virtual void GoToIntermission( void );
	virtual void DeathNotice( CBasePlayer *pVictim, const CTakeDamageInfo &info );
	virtual const char *GetGameDescription( void );
	virtual const CViewVectors* GetViewVectors() const;
	const JJKViewVectors* GetJJKViewVectors() const;

	float GetMapRemainingTime();
	void CleanUpMap();
	void CheckRestartGame();
	void RestartGame();

	void OnNavMeshLoad( void );

	bool IsOfficialMap( void );

	virtual void ClientDisconnected( edict_t *pClient );

	bool CheckGameOver( void );
	bool IsIntermission( void );

	void PlayerKilled( CBasePlayer *pVictim, const CTakeDamageInfo &info );

	
	bool	IsTeamplay( void ) { return m_bTeamPlayEnabled;	}
	void	CheckAllPlayersReady( void );

	virtual bool IsConnectedUserInfoChangeAllowed( CBasePlayer *pPlayer );

private:
	CNetworkVar( bool, m_bTeamPlayEnabled );
	CNetworkVar( float, m_flGameStartTime );
	CUtlVector<EHANDLE> m_hRespawnableItemsAndWeapons;
	float m_tmNextPeriodicThink;
	float m_flRestartGameTime;
	bool m_bCompleteReset;
	bool m_bAwaitingReadyRestart;
	bool m_bHeardAllPlayersReady;

#ifndef CLIENT_DLL
	bool m_bChangelevelDone;
#endif
};

inline CJJKRules* JJKRules()
{
	return static_cast<CJJKRules*>(g_pGameRules);
}
#endif