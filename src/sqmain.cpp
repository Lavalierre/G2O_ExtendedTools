#include "pch.h"

// Modules

#include "workspace/modules/logicalKeys/logicalKeys.h"
#include "workspace/modules/player/player.h"

extern "C" SQRESULT SQRAT_API sqmodule_load(HSQUIRRELVM vm, HSQAPI api)
{
	SqModule::Initialize(vm, api);
	Sqrat::DefaultVM::Set(vm);

	// Initializing default root table
	Sqrat::RootTable roottable(vm);

	// Initializing 'Logical Keys' module
	CLogicalKeys::instance().init(roottable);
	CPlayer::instance().init(roottable);

	return SQ_OK;
}
