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

namespace Framework
{
	using namespace physx;

	//////////////////////////////

	PxRigidDynamic* Physics::TestDynamic(const PxTransform& t, const PxGeometry& geometry, const PxVec3& velocity = PxVec3(0))
	{
		PxRigidDynamic* dynamic = PxCreateDynamic(*PhysXPhysics, t, geometry, *Material, 10.0f);
		dynamic->setAngularDamping(0.5f);
		dynamic->setLinearVelocity(velocity);
		Scene->addActor(*dynamic);
		return dynamic;
	}

	void Physics::TestStack(const PxTransform& t, PxU32 size, PxReal halfExtent)
	{
		PxShape* shape = PhysXPhysics->createShape(PxBoxGeometry(halfExtent, halfExtent, halfExtent), *Material);
		for (PxU32 i = 0; i<size; i++)
		{
			for (PxU32 j = 0; j<size - i; j++)
			{
				PxTransform localTm(PxVec3(PxReal(j * 2) - PxReal(size - i), PxReal(i * 2 + 1), 0) * halfExtent);
				PxRigidDynamic* body = PhysXPhysics->createRigidDynamic(t.transform(localTm));
				body->attachShape(*shape);
				PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
				Scene->addActor(*body);
			}
		}
		shape->release();
	}

	Physics * Physics::Get()
	{
		static Physics physics;
		return &physics;
	}

	void Physics::Initialize()
	{
		//Init PhysX base class
		Foundation = PxCreateFoundation(PX_FOUNDATION_VERSION, Allocator, ErrorCallback);

		//Init PhysX Visual Debugger only in debug mode
#ifdef _DEBUG
		Pvd = PxCreatePvd(*Foundation);
		PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
		Pvd->connect(*transport, PxPvdInstrumentationFlag::eALL);
#endif

		//Init PhysX simulation class
		PhysXPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *Foundation, PxTolerancesScale(), true, Pvd);

		//Init PhysX scene
		PxSceneDesc sceneDesc(PhysXPhysics->getTolerancesScale());
		sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
		Dispatcher = PxDefaultCpuDispatcherCreate(2);
		sceneDesc.cpuDispatcher = Dispatcher;
		sceneDesc.filterShader = PxDefaultSimulationFilterShader;
		Scene = PhysXPhysics->createScene(sceneDesc);

		//PhysX Visual Debugger only exists in debug mode
#ifdef _DEBUG
		PxPvdSceneClient* pvdClient = Scene->getScenePvdClient();
		if (pvdClient)
		{
			pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
			pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
			pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
		}
#endif

		////////////////////
		//Create test material and objects
		Material = PhysXPhysics->createMaterial(0.5f, 0.5f, 0.6f);

		PxRigidStatic* groundPlane = PxCreatePlane(*PhysXPhysics, PxPlane(0, 1, 0, 0), *Material);
		Scene->addActor(*groundPlane);

		for (PxU32 i = 0; i<5; i++)
			TestStack(PxTransform(PxVec3(0, 0, stackZ -= 10.0f)), 10, 2.0f);

		TestDynamic(PxTransform(PxVec3(0, 40, 100)), PxSphereGeometry(10), PxVec3(0, -50, -100));
		////////////////////

	} //end Initialize()

	  //Controls simulation speed
	float Physics::Throttle(float dt) const 
	{
		static float timer = 0;
		//If game is running too slow, slow down action
		if (dt > slowest_simulation_rate)
		{
			timer = 0;
			return slowest_simulation_rate;
		}
		//If game is running too fast, don't simulate every frame
		timer += dt;
		if (timer < fastest_simulation_rate)
		{
			return 0;
		}
		//Need to run simulation this frame
		else
		{
			float const retval = timer;
			timer = 0;
			return retval;
		}
	} // end Throttle()

	void Physics::Update(float dt)
	{
		//PX_UNUSED(interactive);
		if (Throttle(dt))
		{
			Scene->simulate(dt);
			Scene->fetchResults(true);
		}
	} //end Update()

	Physics::~Physics()
	{
		//Release PhysX objects
		Scene->release();
		Dispatcher->release();
		PhysXPhysics->release();

		//Only have PVD in debug mode
#ifdef _DEBUG
		PxPvdTransport* transport = Pvd->getTransport();
		Pvd->release();
		transport->release();
#endif

		Foundation->release();

	} //end ~Physics()

	Physics::Physics()
	{
	}

}; // Framework