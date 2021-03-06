/*
	Author: RTSR
	Contributors: Patrix9999
	7/11/2022
*/

#pragma once

class CLogicalKeys
{
private:

	std::map<std::string, int>												m_LogicalKeysMap;	// Storage for config keys
	Sqrat::Class<CLogicalKeys, Sqrat::NoConstructor<CLogicalKeys>>			sq_namespace;		// Namespace for functions and variables

	std::string			getConfigKey		(int);
	int					getConfigKey		(std::string);
	void				registerLogicalKey	(std::string, std::string, int);

	static SQInteger	bind				(HSQUIRRELVM);
	bool				unbind				(int);
	Sqrat::Array		get					(int);
	void				reset				(bool);

	//-----------------------------

	CLogicalKeys() :
		sq_namespace(Sqrat::Class<CLogicalKeys, Sqrat::NoConstructor<CLogicalKeys>>(SqModule::vm, "LogicalKey")) {};

public:

	void			init				(Sqrat::RootTable);														// Main registering function for the module

	//-----------------------------

	CLogicalKeys(CLogicalKeys const&)		= delete;
	void operator= (CLogicalKeys const&)	= delete;

	static CLogicalKeys& instance()
	{
		static CLogicalKeys single;
		return single;
	}
};