/******************************************************************************/
/*!
\file   PhysicsSystem.h
\author Matthew Gilbert
\par    Copyright 2017, Digipen Institute of Technology
\brief
Interface for the physics system.
*/
/******************************************************************************/
#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

namespace Framework
{
	class PhysicsSystem
	{
	public:
			// all systems need to be initialized, if not specified used empty
		virtual void Initialize();
			// all systems need to be updated
		virtual void Update(float dt = 0.0f);
			// destructor
		virtual ~PhysicsSystem();
	}; // ISystem
}; // Framework

#endif