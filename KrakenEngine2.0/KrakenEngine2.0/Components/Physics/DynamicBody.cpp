/******************************************************************************/
/*!
\file   DynamicBody.cpp
\author Matthew Gilbert
\par    Copyright 2017, Digipen Institute of Technology
\brief
Implementation of the dynamic rigid body component.
*/
/******************************************************************************/

#include "../../Precompiled.h"

namespace Framework
{
using namespace physx;
//Forward Declarations
class Physics;

DynamicBody::DynamicBody() : rigid_dynamic_(nullptr)
{
#ifdef _DEBUG
	std::cout << "Invalid DynamicBody created!\n";
#endif
}

DynamicBody::DynamicBody(unsigned int scene_id,
	ColliderType collider_type,
	float mass,
	Transform const & transform,
	glm::vec3 const & velocity)
	: rigid_dynamic_(nullptr)
{
	//Setup aliases for readability
	glm::vec3 const & position = transform.GetPosition();
	glm::vec3 const & scale = transform.GetScale();
	glm::quat const & rotation = transform.GetRotation();
	PxPhysics & physx_physics = *Physics::Get()->physx_physics_;

	switch (collider_type)
	{
	case e_sphere:
		//Create physx object
		rigid_dynamic_ = PxCreateDynamic(physx_physics,
			PxTransform(PxVec3(position.x, position.y, position.z), PxQuat(rotation.x, rotation.y, rotation.z, rotation.w)),
			PxSphereGeometry(scale.x),
			*physx_physics.createMaterial(0.5f, 0.5f, 0.6f),
			mass / ((4.f / 3.f) * glm::pi<float>() * scale.x * scale.x * scale.x));
		break;

	case e_plane:
		break;

	case e_capsule:
		rigid_dynamic_ = PxCreateDynamic(physx_physics,
			PxTransform(PxVec3(position.x, position.y, position.z), PxQuat(rotation.x, rotation.y, rotation.z, rotation.w)),
			PxCapsuleGeometry(scale.x, scale.y),
			*physx_physics.createMaterial(0.5f, 0.5f, 0.6f),
			mass / ((glm::pi<float>() * scale.x * scale.x * scale.y) + (4.f / 3.f) * glm::pi<float>() * scale.x * scale.x * scale.x));
		break;

	case e_box:
		break;

	default:
		break;
	}

	//Finalize object insertion
	if(rigid_dynamic_)
	{
		rigid_dynamic_->setAngularDamping(0.5f);
		rigid_dynamic_->setLinearVelocity(PxVec3(velocity.x, velocity.y, velocity.z));
		Physics::Get()->AddActor(*rigid_dynamic_, scene_id, transform.Owner());
	}
#ifdef _DEBUG
	else
	{
		std::cerr << "DynamicBody failed in constructor!\n";
	}
#endif
}

glm::vec3 DynamicBody::GetVelocity(void) const
{
	PxVec3 const temp = rigid_dynamic_->getLinearVelocity();
	return glm::vec3(temp.x, temp.y, temp.z);
}

void DynamicBody::AddForce(glm::vec3 force)
{
	rigid_dynamic_->addForce(PxVec3(force.x, force.y, force.z));
}

}// end namespace Framework