//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================

#include "cbase.h"
#include "hud.h"
#include "hudelement.h"
#include "c_tf_player.h"
#include "iclientmode.h"
#include "ienginevgui.h"
#include <vgui/ILocalize.h>
#include <vgui/ISurface.h>
#include <vgui/IVGui.h>
#include <vgui_controls/EditablePanel.h>
#include <vgui_controls/ProgressBar.h>
#include <vgui_controls/AnimationController.h>
#include "tf_imagepanel.h"
#include "vgui_controls/Label.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

using namespace vgui;


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class CHudCursedEnergyMeter : public CHudElement, public EditablePanel
{
	DECLARE_CLASS_SIMPLE( CHudCursedEnergyMeter, EditablePanel );

public:
	CHudCursedEnergyMeter( const char *pElementName );

	virtual void	ApplySchemeSettings( IScheme *scheme );
	virtual bool	ShouldDraw( void );
	virtual void	OnTick( void );

private:

	vgui::ContinuousProgressBar*	m_pCurseMeter;
	Label*							m_pUberchargeLabel;
	float							m_flLastCursedEnergyValue;
};

DECLARE_HUDELEMENT( CHudCursedEnergyMeter );

// Purpose: 
//-----------------------------------------------------------------------------
CHudCursedEnergyMeter::CHudCursedEnergyMeter( const char *pElementName ) : CHudElement( pElementName ), BaseClass( NULL, "HudCursedEnergy" )
{
	Panel *pParent = g_pClientMode->GetViewport();
	SetParent( pParent );

	m_pCurseMeter = new ContinuousProgressBar( this, "CEMeter" );
	SetHiddenBits( HIDEHUD_MISCSTATUS );
	m_flLastCursedEnergyValue = -1;
	vgui::ivgui()->AddTickSignal( GetVPanel() );

	SetDialogVariable( "charge", 0 );

	RegisterForRenderGroup( "inspect_panel" );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHudCursedEnergyMeter::ApplySchemeSettings( IScheme *pScheme )
{
	// load control settings...
	LoadControlSettings( "resource/UI/hudcursedenergy.res" );

	m_pUberchargeLabel = dynamic_cast<Label*>( FindChildByName("CEMeter") );
	Assert( m_pUberchargeLabel );

	BaseClass::ApplySchemeSettings( pScheme );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool CHudCursedEnergyMeter::ShouldDraw( void )
{
	C_TFPlayer *pPlayer = C_TFPlayer::GetLocalTFPlayer();

	if ( !pPlayer || !pPlayer->IsAlive() )
	{
		return false;
	}

	return CHudElement::ShouldDraw();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHudCursedEnergyMeter::OnTick( void )
{
	C_TFPlayer *pPlayer = C_TFPlayer::GetLocalTFPlayer();

	if ( !pPlayer )
		return;

	float flCharge = pPlayer->GetCursedEnergy();


	if ( flCharge != m_flLastCursedEnergyValue )
	{
		
		{
			m_pCurseMeter->SetProgress( flCharge / 100);
			//SetDialogVariable( "charge", (int)( flCharge ) );
		}

		/*if ( !m_bCharged )
		{
			if ( flCharge >= 1.0 )
			{
				g_pClientMode->GetViewportAnimationController()->StartAnimationSequence( this, "HudMedicCharged" );
				m_bCharged = true;
			}
		}*/
		//else
		//{
		//	// we've got invuln charge or we're using our invuln
		//	if ( !pMedigun->IsReleasingCharge() )
		//	{
		//		g_pClientMode->GetViewportAnimationController()->StartAnimationSequence( this, "HudMedicChargedStop" );
		//		m_bCharged = false;
		//	}
		//}

	}	

	m_flLastCursedEnergyValue = flCharge;
}