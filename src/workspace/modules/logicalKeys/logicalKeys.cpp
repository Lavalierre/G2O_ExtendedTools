/*
	Author: RTSR
	Contributors: Patrix9999
	7/11/2022
*/

#include <pch.h>
#include "logicalKeys.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Initializing config keys and Squirrel functions
///
/// \param roottable represents default root table of the Sqrat
/// 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CLogicalKeys::init(Sqrat::RootTable roottable)
{
	HSQUIRRELVM vm = SqModule::vm;

	// Defining namespace
	sq_namespace = std::make_unique<Sqrat::Class<CLogicalKeys, Sqrat::NoConstructor<CLogicalKeys>>>(vm, "LogicalKey");
	
	// Registering keys
	registerLogicalKey("GAME_LAME_HEAL",		"keyHeal",				GAME_LAME_HEAL);			// Heal potion hotkey
	registerLogicalKey("GAME_LAME_POTION",		"keyPotion",			GAME_LAME_POTION);			// Mana potion hotkey

	registerLogicalKey("GAME_LOCK_TARGET",		"keyLockTarget",		GAME_LOCK_TARGET);
	registerLogicalKey("GAME_LOOK",				"keyLook",				GAME_LOOK);
	registerLogicalKey("GAME_LOOK_FP",			"keyLookFP",			GAME_LOOK_FP);				// Firstperson mode hotkey

	registerLogicalKey("GAME_PARADE",			"keyParade",			GAME_PARADE);
	registerLogicalKey("GAME_WEAPON",			"keyWeapon",			GAME_WEAPON);

	registerLogicalKey("GAME_ACTIONRIGHT",		"keyActionRight",		GAME_ACTIONRIGHT);			// G2 controls stuff
	registerLogicalKey("GAME_ACTIONLEFT",		"keyActionLeft",		GAME_ACTIONLEFT);			// G2 controls stuff
	registerLogicalKey("GAME_ACTION",			"keyAction",			GAME_ACTION);

	registerLogicalKey("GAME_UP",				"keyUp",				GAME_UP);
	registerLogicalKey("GAME_DOWN",				"keyDown",				GAME_DOWN);
	registerLogicalKey("GAME_RIGHT",			"keyRight",				GAME_RIGHT);
	registerLogicalKey("GAME_LEFT",				"keyLeft",				GAME_LEFT);

	registerLogicalKey("GAME_STRAFELEFT",		"keyStrafeLeft",		GAME_STRAFELEFT);
	registerLogicalKey("GAME_STRAFERIGHT",		"keyStrafeRight",		GAME_STRAFERIGHT);

	registerLogicalKey("GAME_SLOW",				"keySlow",				GAME_SLOW);					// Walk mode
	registerLogicalKey("GAME_SMOVE",			"keySMove",				GAME_SMOVE);				// Special Move / Jump
	registerLogicalKey("GAME_SNEAK",			"keySneak",				GAME_SNEAK);

	registerLogicalKey("GAME_INVENTORY",		"keyInventory",			GAME_INVENTORY);
	registerLogicalKey("GAME_SCREEN_STATUS",	"keyShowStatus",		GAME_SCREEN_STATUS);
	registerLogicalKey("GAME_SCREEN_LOG",		"keyShowLog",			GAME_SCREEN_LOG);
	registerLogicalKey("GAME_SCREEN_MAP",		"keyShowMap",			GAME_SCREEN_MAP);
	registerLogicalKey("GAME_END",				"keyEnd",				GAME_END);

	// Binding functions
	sq_namespace->Func("bind",					&CLogicalKeys::bind);
	sq_namespace->Func("unbind",				&CLogicalKeys::unbind);
	sq_namespace->Func("get",					&CLogicalKeys::get);
	sq_namespace->Func("reset",					&CLogicalKeys::reset);

	// Binding the namespace to the root table
	roottable.Bind("LogicalKey", *sq_namespace);
	roottable.SetInstance("LogicalKey",	&CLogicalKeys::instance());
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Overloaded function gets config key (map key) by given logical key (map value)
///
/// \param logicalId represents game's logical key id
/// 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::string CLogicalKeys::getConfigKey(int logicalId)
{
	for (auto it = m_LogicalKeysMap.begin(); it != m_LogicalKeysMap.end(); it++)
		if (it->second == logicalId)
			return it->first;

	return "";
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Overloaded function gets logical key (map value) by given config key (map key)
///
/// \param configKey represents key name which is written in the Gothic.ini
/// 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CLogicalKeys::getConfigKey(std::string configKey)
{
	for (auto it = m_LogicalKeysMap.begin(); it != m_LogicalKeysMap.end(); it++)
		if (it->first == configKey)
			return it->second;

	return -1;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Function registering logical keys as Squirrel variables and binds config keys
///
/// \param squirrelName represents which name will be in the Squirrel constants
/// \param configKey	represents which name will be written in the Gothic.ini
/// \param logicalId	represents id of the game's logical key
/// 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CLogicalKeys::registerLogicalKey(std::string squirrelName, std::string configKey, int logicalId)
{
	// Binding the config key to the logical key
	m_LogicalKeysMap.insert(std::pair<std::string, int>(configKey, logicalId));

	// Registering new variables
	sq_namespace->SetValue(squirrelName.c_str(), logicalId);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Function binds the logical key to physical keys
///
/// \param logicalId	represents id of the game's logical key
/// \param gameKeyId	represents main physical key that will be binded to this logical key
/// \param addGameKeyId	represents additional physical key that will be binded. Parameter is optional
/// 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CLogicalKeys::bind(int logicalId, int gameKeyId, int addGameKeyId)
{
	if (logicalId < GAME_LEFT && logicalId > GAME_LAME_HEAL)
	{
		SqModule::Error("(LogicalKey.bind) parameter 0 must be in range of logical keys");
		return false;
	}

	std::string configKey = getConfigKey(logicalId);
	if (configKey != "")
	{
		zCArray<zWORD> controlValueList;
		controlValueList.EmptyList();
		controlValueList.Insert(gameKeyId);

		if (addGameKeyId != 0)
			controlValueList.Insert(addGameKeyId);

		zoptions->WriteRaw("KEYS", configKey.c_str(), controlValueList.GetArray(), controlValueList.GetNumInList() << 1, FALSE);
		zinput->BindKeys(0);

		return true;
	}

	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Function removes all bindings from the logical key
///
/// \param logicalId	represents id of the game's logical key
/// 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CLogicalKeys::unbind(int logicalId)
{
	if (logicalId < GAME_LEFT && logicalId > GAME_LAME_HEAL)
	{
		SqModule::Error("(LogicalKey.unbind) parameter 0 must be in range of logical keys");
		return false;
	}

	std::string configKey = getConfigKey(logicalId);
	if (configKey != "")
	{
		zWORD buffer = 0x0000;
		zoptions->WriteRaw("KEYS", configKey.c_str(), &buffer, sizeof(buffer), FALSE);
		zinput->BindKeys(0);

		return true;
	}

	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Function returns all bindings for the specific logical key
///
/// \param logicalId	represents id of the game's logical key
/// 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Sqrat::Array CLogicalKeys::get(int logicalId)
{
	Sqrat::Array return_arr(SqModule::vm);

	if (logicalId < GAME_LEFT && logicalId > GAME_LAME_HEAL)
	{
		SqModule::Error("(LogicalKey.unbind) parameter 0 must be in range of logical keys");
		return return_arr;
	}

	zCArray<zWORD> controlValues;
	controlValues.EmptyList();
	zinput->GetBinding(logicalId, controlValues);

	for (int i = 0; i < controlValues.GetNumInList(); i++)
		return_arr.Append(controlValues[i]);

	return return_arr;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Function resets all bindings for logical keys to default settings
///
/// \param alternative represents if the default settings will be alternative
/// 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CLogicalKeys::reset(bool alternative)
{
	for (auto it = m_LogicalKeysMap.begin(); it != m_LogicalKeysMap.end(); it++)
		zoptions->RemoveEntry("KEYS", it->first.c_str());

	zinput->BindKeys(alternative);
}