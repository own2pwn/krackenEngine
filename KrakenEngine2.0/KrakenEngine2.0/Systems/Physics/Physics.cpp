/******************************************************************************/
/*!
\file   Physics.cpp
\author Matthew Gilbert
\par    Copyright 2017, Digipen Institute of Technology
\brief
Implementation of the physics system using the PhysX library.
*/
/******************************************************************************/

#include "../../Precompiled.h"

/*
*material_ = physx_physics_->createMaterial(0.5f, 0.5f, 0.6f);

PxRigidStatic* groundPlane = PxCreatePlane(*physx_physics_, PxPlane(0, 1, 0, 0), *material_);
scenes_[0]->addActor(*groundPlane);

for (PxU32 i = 0; i < 5; i++)
TestStack(PxTransform(PxVec3(0, 0, stack_z_ -= 10.0f)), 10, 2.0f);

TestDynamic(PxTransform(PxVec3(0, 40, 100)), PxSphereGeometry(10), PxVec3(0, -50, -100));

* PxRigidDynamic* dynamic = PxCreateDynamic(*physx_physics_, t, geometry, *material_, 10.0f);
dynamic->setAngularDamping(0.5f);
dynamic->setLinearVelocity(velocity);
scenes_[0]->addActor(*dynamic);
return dynamic;
*/


namespace Framework
{
using namespace physx;

//////////////////////////////
//Copied from PhysX HelloWorld example
PxRigidDynamic* Physics::TestDynamic(const PxTransform& t,
	const PxGeometry& geometry,
	const PxVec3& velocity,
	unsigned int space_id)
{
	PxMaterial * material = physx_physics_->createMaterial(0.5f, 0.5f, 0.6f);
	PxRigidDynamic* dynamic = PxCreateDynamic(*physx_physics_, t, geometry, *material, 10.0f);
	dynamic->setAngularDamping(0.5f);
	dynamic->setLinearVelocity(velocity);
	scenes_[space_id]->addActor(*dynamic);
	return dynamic;
}

//Copied from PhysX HelloWorld example
void Physics::TestStack(const PxTransform& t, PxU32 size, PxReal halfExtent, unsigned int space_id)
{

	PxMaterial * material = physx_physics_->createMaterial(0.5f, 0.5f, 0.6f);
	PxShape* shape = physx_physics_->createShape(PxBoxGeometry(halfExtent, halfExtent, halfExtent), *material);

	for (PxU32 i = 0; i < size; i++)
	{
		for (PxU32 j = 0; j < size - i; j++)
		{
			PxTransform const localTm(PxVec3(PxReal(j * 2) - PxReal(size - i), PxReal(i * 2 + 1), 0) * halfExtent);
			PxRigidDynamic* body = physx_physics_->createRigidDynamic(t.transform(localTm));
			body->attachShape(*shape);
			PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
			scenes_[space_id]->addActor(*body);
		}
	}
	shape->release();
}

Physics* Physics::Get()
{
	static Physics physics;
	return &physics;
}

void Physics::Initialize()
{
	//Init PhysX base class
	foundation_ = PxCreateFoundation(PX_FOUNDATION_VERSION, allocator_, error_callback_);

	//Init PhysX Visual Debugger only in debug mode
#ifdef _DEBUG
	pvd_ = PxCreatePvd(*foundation_);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
	pvd_->connect(*transport, PxPvdInstrumentationFlag::eALL);
#endif

	//Init PhysX simulation class
	physx_physics_ = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation_, PxTolerancesScale(), true, pvd_);

} //end Initialize()

	void Physics::CreateScene(unsigned int space_id)
{
	//Init PhysX scene
	PxSceneDesc sceneDesc(physx_physics_->getTolerancesScale());
	//TODO - make gravity not hardcoded
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
	dispatcher_ = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = dispatcher_;
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;
	sceneDesc.flags |= PxSceneFlag::eENABLE_ACTIVE_ACTORS;
	scenes_[space_id] = physx_physics_->createScene(sceneDesc);

	//Try to minimize resizing this vector later
	spaceid_to_updated_transforms_[space_id].reserve(64);
}

void Physics::AddActor(PxActor & actor, unsigned int scene_id, GameObject * object)
{
	scenes_[scene_id]->addActor(actor);
	actor.userData = object;
}

//Controls simulation speed
float Physics::Throttle(float dt) const
{
	static float timer = 0;
	//If game is running too slow, slow down action
	if (dt > slowest_simulation_rate_)
	{
		timer = 0;
		return slowest_simulation_rate_;
	}
	//If game is running too fast, don't simulate every frame
	timer += dt;
	if (timer < fastest_simulation_rate_)
	{
		return 0;
	}
	//Need to run simulation this frame
	float const retval = timer;
	timer = 0;
	return retval;
} // end Throttle()

void Physics::RunTests(void)
{
	////////////////////
	//Testing stuff
	static bool test_done = false;
	if (test_done == false)
	{
		//Create test space
		test_space_id = SpaceFactory::Get()->AddSpace("physics_test_space")->GetID();

		//PhysX Visual Debugger only exists in debug mode
#ifdef _DEBUG
		PxPvdSceneClient* pvdClient = scenes_[test_space_id]->getScenePvdClient();
		if (pvdClient)
		{
			pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
			pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
			pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
		}
#endif

		////////////////////
		//Create test material and objects
		//PxMaterial * material = physx_physics_->createMaterial(0.5f, 0.5f, 0.6f);

		//PxRigidStatic* groundPlane = PxCreatePlane(*physx_physics_, PxPlane(0, 1, 0, 0), *material);
		//scenes_[test_space_id]->addActor(*groundPlane);

		//PxReal stack_z_ = 10.0f;
		//for (PxU32 i = 0; i < 5; i++)
			//TestStack(PxTransform(PxVec3(0, 0, stack_z_ -= 10.0f)), 10, 2.0f, test_space_id);

		//TestDynamic(PxTransform(PxVec3(0, 40, 100)), PxSphereGeometry(10), PxVec3(0, -50, -100), test_space_id);

		//Test component creation
		std::cerr << SpaceFactory::Get()->Size();
		GameObject * game_object = SpaceFactory::Get()->GetSpace(test_space_id)->AddObject();
		game_object->AddComponent<Transform>();
		Transform * transform = game_object->GetComponent<Transform>();
		transform->position_ = glm::vec3(0, 20, 100);
		transform->scale_ = glm::vec3(5, 5, 5);
		transform->rotation_ = glm::quat(0, 0, 0, 1);
		DynamicBody * body = new DynamicBody(test_space_id,
			e_sphere,
			10000.0f,
			*transform,
			glm::vec3(0, -50, -100));
		game_object->AddComponent<DynamicBody>(body);

		//Second test object
		game_object = SpaceFactory::Get()->GetSpace(test_space_id)->AddObject();
		game_object->AddComponent<Transform>();
		transform = game_object->GetComponent<Transform>();
		transform->position_ = glm::vec3(0, 40, 100);
		transform->scale_ = glm::vec3(15, 15, 15);
		transform->rotation_ = glm::quat(0, 0, 0, 1);
		body = new DynamicBody(test_space_id,
			e_capsule,
			50000.0f,
			*transform,
			glm::vec3(0, -50, -100));
		game_object->AddComponent<DynamicBody>(body);
		////////////////////
		test_done = true;
	}
	//end testing stuff
	////////////////////
}

void Physics::Update(float dt)
{
#ifdef _DEBUG
	RunTests();
#endif

	if (Throttle(dt))
	{
		for (auto pair : scenes_)
		{
			//Update physx simulation
			pair.second->simulate(dt);
			pair.second->fetchResults(true);

			//Get array of actors that moved
			std::vector<Transform *> & updated_transforms = spaceid_to_updated_transforms_[pair.first];
			PxU32 num_active_actors;
			PxActor ** active_actors = pair.second->getActiveActors(num_active_actors);

			
			for(PxU32 i = 0; i < num_active_actors; ++i)
			{
				//Find pointer to the engine transform
				PxActor * actor = active_actors[i];
				GameObject * object = static_cast<GameObject*>(actor->userData);
				Transform * transform = object->GetComponent<Transform>();
				//Add pointer to container
				updated_transforms.push_back(transform);
				//Update engine transform to match simulation transform
				//NOTE: This static cast make cloth simulation not work
				PxTransform const & px_transform = static_cast<PxRigidActor*>(active_actors[i])->getGlobalPose();
				PxVec3 const & px_position = px_transform.p;
				PxQuat const & px_rotation = px_transform.q;
				transform->position_ = glm::vec3(px_position.x, px_position.y, px_position.z);
				transform->rotation_ = glm::quat(px_rotation.x, px_rotation.y, px_rotation.z, px_rotation.w);
			}
		}
	}
} //end Update()

Physics::~Physics()
{
	//Release PhysX objects
	for (auto pair : scenes_)
	{
		pair.second->release();
	}
	dispatcher_->release();
	physx_physics_->release();

	//Only have PVD in debug mode
#ifdef _DEBUG
	PxPvdTransport* transport = pvd_->getTransport();
	pvd_->release();
	transport->release();
#endif

	foundation_->release();
} //end ~Physics()

Physics::Physics()
{
}

std::unordered_map<unsigned, std::vector<Transform*>> const & Physics::GetUpdatedTransforms() const
{
	return spaceid_to_updated_transforms_;
}
} // end Framework
