#include "pch.h"

// Modules

#include "workspace/modules/logicalKeys/logicalKeys.h"
#include "workspace/modules/player/player.h"

void testFunc()
{
	auto coll = static_cast<zCCollObjectCharacter*>(player->anictrl->vob->GetCollisionObject());
	coll->m_oSpatialState.m_fFloorY;
	SqModule::Print(std::string(std::to_string(coll->m_oSpatialState.m_fFloorY)).c_str());
}

extern "C" SQRESULT SQRAT_API sqmodule_load(HSQUIRRELVM vm, HSQAPI api)
{
	SqModule::Initialize(vm, api);
	Sqrat::DefaultVM::Set(vm);

	// Initializing default root table
	Sqrat::RootTable roottable(vm);

	roottable.Func("FloorY", testFunc);

	// Initializing 'Logical Keys' module
	CLogicalKeys::instance().init(roottable);
	CPlayer::instance().init(roottable);

	return SQ_OK;
}
