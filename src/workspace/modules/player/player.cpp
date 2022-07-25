/*
	Author: RTSR
	7/11/2022
*/

#include <pch.h>
#include "player.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Initializing config keys and Squirrel functions
///
/// \param roottable represents default root table of the Sqrat
/// 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CPlayer::init(Sqrat::RootTable roottable)
{
	HSQUIRRELVM vm = SqModule::vm;
	
	sq_namespace.Func("setOnFloor", &CPlayer::setOnFloor);

	// Binding the namespace to the root table
	roottable.Bind("Player", sq_namespace);
	roottable.SetInstance("Player",	&CPlayer::instance());
}

void CPlayer::setOnFloor()
{
	zCCollObjectCharacter* coll = static_cast<zCCollObjectCharacter*>(player->anictrl->vob->GetCollisionObject());
	if (coll)
	{
		zVEC3 playerPos							= player->GetPositionWorld();
		float floorY							= coll->m_oSpatialState.m_fFloorY + 5;

		// Save current HP, MP and sleeping stwate

		int iCurHealth							= player->attribute[NPC_ATR_HITPOINTS];
		int iCurMana							= player->attribute[NPC_ATR_MANA];
		int iSleepingMode						= player->sleepingMode;

		player->SetCollDet(FALSE);
		player->ResetPos(zVEC3(playerPos[0], floorY, playerPos[2]));
		player->SetCollDet(TRUE);

		// Restoring player states

		player->attribute[NPC_ATR_HITPOINTS]	= iCurHealth;
		player->attribute[NPC_ATR_MANA]			= iCurMana;
		player->SetSleepingMode(zTVobSleepingMode(iSleepingMode));
	}
}