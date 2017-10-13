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
		void AddActor(physx::PxActor & actor, unsigned int scene_id, GameObject * object);

		//Special access for Physics related components
		friend class DynamicBody;
	
	private:
		Physics();

		//Access the transforms that updated last frame
		//Key is the space id
		//Data is a vector of pointers to the updated transforms
		std::unordered_map<unsigned int, std::vector<Transform * > > const & GetUpdatedTransforms(void) const;

		const float fastest_simulation_rate_ = 1.0f / 80.0f;
		const float slowest_simulation_rate_ = 1.0f / 20.0f;
		//TEMP - get rid of this after testing
		unsigned int test_space_id;
		//TODO - Implement rate averaging/fixed rate
		//float average_simulation_rate_ = -1.0f;
		
		//Stores transforms that moved during last simulation
		std::unordered_map<unsigned int, std::vector<Transform *> > spaceid_to_updated_transforms_;

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