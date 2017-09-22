/******************************************************************************/
/*!
\file   ISystem.h
\author Aleksey Perfilev
\par    Copyright 2017, Digipen Institute of Technology
\brief
Interface for all the systems.
*/
/******************************************************************************/
#ifndef ISYSTEM_H
#define ISYSTEM_H

namespace Framework
{
	class ISystem
	{
	public:
			// all systems need to be initialized, if not specified used empty
		virtual void Initialize() {}
			// all systems need to be updated
		virtual void Update(float dt = 0.0f) = 0;
			// destructor
		virtual ~ISystem() {}
	}; // ISystem
}; // Framework

#endif