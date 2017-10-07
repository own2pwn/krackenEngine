/******************************************************************************/
/*!
\file   Transform.h
\author Matthew Gilbert
\par    Copyright 2017, Digipen Institute of Technology
\brief
Interface for the transform component.
*/
/******************************************************************************/
#ifndef TRANSFORM_H
#define TRANSFORM_H

namespace Framework
{
	class Transform : public Component
	{
	public:
		glm::vec3 position;
		glm::quat rotation;
		glm::vec3 scale;

	}; //end class Transform
}; // end namespace Framework

#endif