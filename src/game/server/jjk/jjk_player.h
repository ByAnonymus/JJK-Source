#ifndef JJK_PLAYER_H
#define JJK_PLAYER_H
#pragma once

#include "basemultiplayerplayer.h"
#include "jjk_playeranimstate.h"
#include "jjk_shareddefs.h"
#include "jjk_player_shared.h"


class CJJKPlayer;
class CJJKWeaponBase;

//============================================================
// 
// Player State Information
// 
class CPlayerStateInfo
{
public:

	int				m_nPlayerState;
	const char		*m_pStateName;

	// Enter/Leave state.
	void ( CJJKPlayer::*pfnEnterState )();	
	void ( CJJKPlayer::*pfnLeaveState )();

	// Think (called every frame).
	void ( CJJKPlayer::*pfnThink )();
};

struct DamagerHistory_t
{
	DamagerHistory_t()
	{
		Reset();
	}
	void Reset()
	{
		hDamager = NULL;
		flTimeDamage = 0;
	}
	EHANDLE hDamager;
	float flTimeDamage;
};
#define MAX_DAMAGER_HISTORY 2

//============================================================
// 
// JJK Player
// 
class CJJKPlayer: public CBaseMultiplayerPlayer
{
public:
	DECLARE_CLASS( CJJKPlayer, CBaseMultiplayerPlayer);
	DECLARE_SERVERCLASS();
	DECLARE_DATADESC();

	CJJKPlayer();
	~CJJKPlayer();

	// Creation/Destruction
	static CJJKPlayer	*CreatePlayer(const char *className, edict_t *ed);
	static CJJKPlayer	*Instance(int iEnt);

	virtual void Spawn();
	virtual void ForceRespawn();
	virtual CBaseEntity	*EntSelectSpawnPoint( void );
	virtual void		InitialSpawn();
	virtual void		Precache();
	virtual bool		IsReadyToPlay( void );
	virtual bool		IsReadyToSpawn( void );
	virtual bool		ShouldGainInstantSpawn( void );
	virtual void		ResetScores( void );

	virtual void		CheatImpulseCommands(int iImpulse);

	virtual void		CommitSuicide(bool bExplode = false, bool bForce = false);

	// Combats
	virtual void		TraceAttack(const CTakeDamageInfo &info, const Vector &vecDir, trace_t *ptr, CDmgAccumulator *pAccumulator);
	virtual int			TakeHealth( float flHealth, int bitsDamageType );
	virtual	void		Event_KilledOther( CBaseEntity *pVictim, const CTakeDamageInfo &info );
	virtual void		Event_Killed( const CTakeDamageInfo &info );
	virtual void		PlayerDeathThink( void );

	virtual int			OnTakeDamage( const CTakeDamageInfo &inputInfo );
	virtual int			OnTakeDamage_Alive( const CTakeDamageInfo &info );
	void				AddDamagerToHistory( EHANDLE hDamager );
	void				ClearDamagerHistory();
	DamagerHistory_t	&GetDamagerHistory( int i ) { return m_DamagerHistory[i]; }
	virtual void		DamageEffect(float flDamage, int fDamageType);
	virtual	bool		ShouldCollide( int collisionGroup, int contentsMask ) const;

	void				SetHealthBuffTime( float flTime )		{ m_flHealthBuffTime = flTime; }

	// UNUSED BUT KEPT IN CASE I'LL NEED TO BE ABLE TO GET ACTIVE MODE OF THE PLAYER'S MOVESET
	// CTFWeaponBase		*GetActiveTFWeapon( void ) const;

	void				SaveMe( void );

	void				FireBullet( const FireBulletsInfo_t &info, bool bDoEffects, int nDamageType, int nCustomDamageType = TF_DMG_CUSTOM_NONE );
	void				ImpactWaterTrace( trace_t &trace, const Vector &vecStart );
	void				NoteWeaponFired();

	void				Regenerate( void );
	float				GetNextRegenTime( void ){ return m_flNextRegenerateTime; }
	void				SetNextRegenTime( float flTime ){ m_flNextRegenerateTime = flTime; }

	float				GetNextChangeClassTime( void ){ return m_flNextChangeClassTime; }
	void				SetNextChangeClassTime( float flTime ){ m_flNextChangeClassTime = flTime; }

	// Class.
	CJJKPlayerClass		*GetPlayerClass( void ) 					{ return &m_PlayerClass; }
	int					GetDesiredPlayerClassIndex( void )			{ return m_Shared.m_iDesiredPlayerClass; }
	void				SetDesiredPlayerClassIndex( int iClass )	{ m_Shared.m_iDesiredPlayerClass = iClass; }

	// Think
	virtual void		PreThink();
	virtual void		PostThink();

	virtual void		ItemPostFrame();
	virtual void		Weapon_FrameUpdate( void );
	virtual void		Weapon_HandleAnimEvent( animevent_t *pEvent );
	
};

#endif