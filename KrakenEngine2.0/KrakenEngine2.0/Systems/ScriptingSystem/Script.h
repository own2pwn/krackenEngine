/******************************************************************************/
/*!
\file   Script.h
\author Aleksey Perfilev
\par    Copyright 2017, Digipen Institute of Technology
\brief
interface for Script
*/
/******************************************************************************/
#ifndef SCRIPT_H
#define SCRIPT_H

namespace Framework
{
	class Script :public Component
	{
	public:
		Script();
		virtual ~Script();
		virtual void Update(float dt) = 0;
	};
} // Framework
#endif
