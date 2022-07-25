/*
	Author: RTSR
	7/11/2022
*/

#pragma once

class CPlayer
{
private:

	Sqrat::Class<CPlayer, Sqrat::NoConstructor<CPlayer>>			sq_namespace;			// Namespace for functions and variables

	void		setOnFloor		();

	//-----------------------------

	CPlayer() :
		sq_namespace(Sqrat::Class<CPlayer, Sqrat::NoConstructor<CPlayer>>(SqModule::vm, "Player")) {};

public:

	void			init				(Sqrat::RootTable);									// Main registering function for the module

	//-----------------------------

	CPlayer(CPlayer const&)		= delete;
	void operator= (CPlayer const&)	= delete;

	static CPlayer& instance()
	{
		static CPlayer single;
		return single;
	}
};