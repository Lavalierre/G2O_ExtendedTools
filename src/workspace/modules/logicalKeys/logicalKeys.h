/*
	Author: RTSR
	Contributors: Patrix9999
	7/11/2022
*/

#pragma once

class CLogicalKeys
{
private:

	std::map<std::string, int>																m_LogicalKeysMap;	// Storage for config keys
	std::unique_ptr<Sqrat::Class<CLogicalKeys, Sqrat::NoConstructor<CLogicalKeys>>>			sq_namespace;		// Namespace for functions and variables

	std::string		getConfigKey		(int);
	int				getConfigKey		(std::string);
	void			registerLogicalKey	(std::string, std::string, int);

	bool			bind				(int, int, int = 0);
	bool			unbind				(int);
	Sqrat::Array	get					(int);
	void			reset				(bool);

	//-----------------------------

	CLogicalKeys(){};

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