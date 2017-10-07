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

void Physics::TestStack(const PxTransform& t, PxU32 size, PxReal halfExtent, unsigned int space_id)
{

	PxMaterial * material = physx_physics_->createMaterial(0.5f, 0.5f, 0.6f);
	PxShape* shape = physx_physics_->createShape(PxBoxGeometry(halfExtent, halfExtent, halfExtent), *material);

	for (PxU32 i = 0; i < size; i++)
	{
		for (PxU32 j = 0; j < size - i; j++)
		{
			PxTransform localTm(PxVec3(PxReal(j * 2) - PxReal(size - i), PxReal(i * 2 + 1), 0) * halfExtent);
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
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
	dispatcher_ = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = dispatcher_;
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;
	scenes_[space_id] = physx_physics_->createScene(sceneDesc);
}

void Physics::AddActor(PxActor & actor, unsigned int scene_id)
{
	scenes_[scene_id]->addActor(actor);
}

void CreateDynamic(unsigned int space_id, GameObject * object)
{
	// TODO
}

void CreateStatic(unsigned int space_id, GameObject * object)
{
	// TODO
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
		//TEMP - create test space
		SpaceFactory::Get()->AddSpace("physics_test_space");
		test_space_id = SpaceFactory::Get()->GetSpaces()->begin()->first;
		//CreateScene(0);

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
		PxMaterial * material = physx_physics_->createMaterial(0.5f, 0.5f, 0.6f);

		PxRigidStatic* groundPlane = PxCreatePlane(*physx_physics_, PxPlane(0, 1, 0, 0), *material);
		scenes_[test_space_id]->addActor(*groundPlane);

		for (PxU32 i = 0; i < 5; i++)
			TestStack(PxTransform(PxVec3(0, 0, stack_z_ -= 10.0f)), 10, 2.0f, test_space_id);

		//TestDynamic(PxTransform(PxVec3(0, 40, 100)), PxSphereGeometry(10), PxVec3(0, -50, -100), test_space_id);

		//Test component creation
		std::cerr << SpaceFactory::Get()->Size();
		GameObject * game_object = SpaceFactory::Get()->GetSpace(test_space_id)->AddObject();
		game_object->AddComponent<Transform>();
		Transform * transform = game_object->GetComponent<Transform>();
		transform->position = glm::vec3(0, 20, 100);
		transform->scale = glm::vec3(5, 5, 5);
		transform->rotation = glm::quat(0, 0, 0, 1);
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
		transform->position = glm::vec3(0, 40, 100);
		transform->scale = glm::vec3(15, 15, 15);
		transform->rotation = glm::quat(0, 0, 0, 1);
		body = new DynamicBody(test_space_id,
			e_sphere,
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
	RunTests();

	if (Throttle(dt))
	{
		for (auto pair : scenes_)
		{
			pair.second->simulate(dt);
			pair.second->fetchResults(true);
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

}; // Framework
