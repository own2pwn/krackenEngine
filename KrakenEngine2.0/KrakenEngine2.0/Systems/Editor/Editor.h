/******************************************************************************/
/*!
\file   Editor.h
\author Brandon Haze
\par    Copyright 2017, Digipen Institute of Technology
\brief
	Interface for the editor class.
*/
/******************************************************************************/

#pragma once

#ifndef EDITOR_H
#define EDITOR_H
#include "../../Precompiled.h"


class editor : public Framework::ISystem
{
public:
	// Required from inherting from ISystem
	// TODO 
	// Not sure what kind of constructor is needed, singleton? other?
	editor();

	virtual void Initialize() override;

	virtual void Update(float dt) override;
	//

		
		
private:

	bool activated_dgb_;
	std::vector<EditorModule> editor_modules_;


};

	


/*

Editor notes / Things needed / TODO

	- Editor for all variables at runtime / use aleksey's Reflection system
		- Has heirarchy for variables
		- Possibly able to change the variables?

	- qwer system Move / scale / rotate / select items

	- alt click rotate camera wasd to move camera around when dbg on

	- Console for commands
		- console works even in release, for cheats during grading
		- Create the console
		- tab complete
		- activate / deactivate different editor modules

	- fps

	- Editor for Lua

	- Drag and drop items into game to place

	- a way to export / import maps

	- Editor for menus / Create layouts for menus / menu creation


*/


#endif // !EDITOR_H
