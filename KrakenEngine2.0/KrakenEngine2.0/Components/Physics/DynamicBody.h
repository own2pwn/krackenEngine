/******************************************************************************/
/*!
\file   DynamicBody.h
\author Matthew Gilbert
\par    Copyright 2017, Digipen Institute of Technology
\brief
Definition of the dynamic rigid body component.
*/
/******************************************************************************/
#ifndef DYNAMICBODY_H
#define DYNAMICBODY_H

namespace Framework
{
enum ColliderType
{
	e_sphere,
	e_plane,
	e_capsule,
	e_box,
	e_aggregate,

	e_num_geometry_type,
	e_invalid
};
class DynamicBody : public Component
{
public:
	DynamicBody();
	explicit DynamicBody(unsigned int scene,
		ColliderType collider_type,
		float mass,
		Transform const & transform,
		glm::vec3 const & velocity);
	glm::vec3 GetVelocity(void) const;
	void AddForce(glm::vec3 force);

private:
	physx::PxRigidDynamic * rigid_dynamic_;

}; // end class DynamicBody
}; // end namespace Framework

#endif