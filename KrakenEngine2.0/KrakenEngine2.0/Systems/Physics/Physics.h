/******************************************************************************/
/*!
\file   Physics.h
\author Matthew Gilbert
\par    Copyright 2017, Digipen Institute of Technology
\brief
Interface for the physics system.
*/
/******************************************************************************/
#ifndef PHYSICS_H
#define PHYSICS_H

#include "../Systems/ISystem.h"

namespace Framework
{
	class Physics : public ISystem
	{
	public:
		// TODO - add comment
		static Physics * Get();
		// TODO - add comment
		virtual void Initialize();
		// TODO - add comment
		virtual void Update(float dt = 0.0f);
		// TODO - add comment
		virtual ~Physics();
		// TODO - add comment

	}; //end class Physics
}; // end namespace Framework

#endif