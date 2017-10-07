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

		// Create a physics space
		void CreateScene(unsigned int space_id);

		//Add an actor to the simulation
		void AddActor(physx::PxActor & actor, unsigned int scene_id);

		//Special access for Physics related components
		friend DynamicBody::DynamicBody(unsigned int scene,
			ColliderType collider_type,
			float mass,
			Transform const & transform,
			glm::vec3 const & velocity);
	
	private:
		Physics();
		const float fastest_simulation_rate_ = 1.0f / 80.0f;
		const float slowest_simulation_rate_ = 1.0f / 20.0f;
		unsigned int test_space_id;
		//TODO - Implement rate averaging/fixed rate
		//float average_simulation_rate_ = -1.0f;

		std::unordered_map<physx::PxActor *, GameObject *> actor_to_object_;
		std::unordered_map<GameObject *, physx::PxActor *> object_to_actor_;

		//Controls simulation speed
		float Throttle(float dt) const;
		
		//Helper functions

		//PhysX objects
		physx::PxFoundation*			foundation_ = nullptr;
		physx::PxPhysics*				physx_physics_ = nullptr;
		std::unordered_map<unsigned int, physx::PxScene * > scenes_;

		physx::PxDefaultCpuDispatcher*	dispatcher_ = nullptr;
		physx::PxPvd*					pvd_ = nullptr;

		//NOTE: Might need refactoring
		physx::PxDefaultAllocator		allocator_;
		physx::PxDefaultErrorCallback	error_callback_;

		//TODO - Make these variables unnecessary
		//physx::PxMaterial*				material_ = nullptr;
		physx::PxReal					stack_z_ = 10.0f;

		//Methods for testing
		void RunTests(void);
		physx::PxRigidDynamic* Physics::TestDynamic(const physx::PxTransform& t,
			const physx::PxGeometry& geometry,
			const physx::PxVec3& velocity,
			unsigned int space_id);
		void Physics::TestStack(const physx::PxTransform& t,
			physx::PxU32 size,
			physx::PxReal halfExtent,
			unsigned int space_id);

	}; //end class Physics
}; // end namespace Framework

#endif