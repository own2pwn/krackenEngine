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

namespace Framework
{
	class Physics : public ISystem
	{
	public:
		// Access the Physics system through this function
		static Physics * Get();

		// Setup initial requirements of the system
		virtual void Initialize();

		// Run a step in the physics simulation
		virtual void Update(float dt = 0.0f);

		// Cleanup
		virtual ~Physics();

		//Added actors must have a valid transform and body component
		void AddDynamic();
	
	private:
		Physics();
		const float fastest_simulation_rate = 1.0f / 120.0f;
		const float slowest_simulation_rate = 1.0f / 10.0f;

		//Controls simulation speed
		float Throttle(float dt) const;

		//PhysX objects
		physx::PxDefaultAllocator		Allocator;
		physx::PxDefaultErrorCallback	ErrorCallback;

		physx::PxFoundation*			Foundation = nullptr;
		physx::PxPhysics*				PhysXPhysics = nullptr;

		physx::PxDefaultCpuDispatcher*	Dispatcher = nullptr;
		physx::PxScene*					Scene = nullptr;

		physx::PxMaterial*				Material = nullptr;

		physx::PxPvd*					Pvd = nullptr;

		physx::PxReal stackZ = 10.0f;

		//Methods to add test objects
		physx::PxRigidDynamic* Physics::TestDynamic(const physx::PxTransform& t,
			const physx::PxGeometry& geometry,
			const physx::PxVec3& velocity);
		void Physics::TestStack(const physx::PxTransform& t,
			physx::PxU32 size,
			physx::PxReal halfExtent);

	}; //end class Physics
}; // end namespace Framework

#endif