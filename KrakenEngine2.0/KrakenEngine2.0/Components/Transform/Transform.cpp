/******************************************************************************/
/*!
\file   Transform.cpp
\author Matthew Gilbert
\par    Copyright 2017, Digipen Institute of Technology
\brief
Impplementation for the transform component.
*/
/******************************************************************************/

#include "../../Precompiled.h"

namespace Framework
{
	//Constructor
	Transform::Transform(glm::vec3 position,
		glm::quat rotation,
		glm::vec3 scale)
	{
		position_ = position;
		rotation_ = rotation;
		scale_ = scale;
	}

	////////////////////
	//Settors
	//TODO - update Physics
	void Transform::SetPose(glm::vec3 position,
		glm::quat rotation,
		glm::vec3 scale)
	{
		position_ = position;
		rotation_ = rotation;
		scale_ = scale;
	}

	//TODO - update Physics
	void Transform::SetPosition(glm::vec3 position)
	{
		position_ = position;
	}

	//TODO - update Physics
	void Transform::SetRotation(glm::quat rotation)
	{
		rotation_ = rotation;
	}

	//TODO - update Physics
	void Transform::SetScale(glm::vec3 scale)
	{
		scale_ = scale;
	}
	//End settors
	////////////////////

	////////////////////
	//Gettors
	glm::vec3 const& Transform::GetPosition() const
	{
		return position_;
	}

	glm::quat const& Transform::GetRotation() const
	{
		return rotation_;
	}

	glm::vec3 const& Transform::GetScale() const
	{
		return scale_;
	}
	// end Gettors
} // end namespace Framework