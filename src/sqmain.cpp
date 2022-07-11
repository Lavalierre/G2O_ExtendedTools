#include "pch.h"

extern "C" SQRESULT SQRAT_API sqmodule_load(HSQUIRRELVM vm, HSQAPI api)
{
	SqModule::Initialize(vm, api);
	Sqrat::DefaultVM::Set(vm);

	SqModule::Print("Hello from module!\n");

	return SQ_OK;
}
