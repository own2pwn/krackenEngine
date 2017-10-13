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
	//Struct to contain all information of a transform
	class Transform : public Component
	{
	public:
		//Constructor
		Transform(glm::vec3 position = glm::vec3(0, 0, 0),
			glm::quat rotation = glm::quat(0, 0, 0, 1),
			glm::vec3 scale = glm::vec3(1, 1, 1));

		//Settors
		void SetPose(glm::vec3 position = glm::vec3(0, 0, 0),
			glm::quat rotation = glm::quat(0, 0, 0, 1),
			glm::vec3 scale = glm::vec3(1, 1, 1));
		void SetPosition(glm::vec3 position);
		void SetRotation(glm::quat rotation);
		void SetScale(glm::vec3 scale);

		//Gettors
		glm::vec3 const & GetPosition() const;
		glm::quat const & GetRotation() const;
		glm::vec3 const & GetScale() const;

		//Friend classes
		friend class Physics;
		friend class DynamicBody;

	private:
		glm::vec3 position_;
		glm::quat rotation_;
		glm::vec3 scale_;

	}; //end class Transform
}; // end namespace Framework

#endif