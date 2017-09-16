/******************************************************************************/
/*!
\file   Physics.cpp
\author Matthew Gilbert
\par    Copyright 2017, Digipen Institute of Technology
\brief
Implementation of the physics system using the PhysX library.
*/
/******************************************************************************/

#include "Physics.h"
#include "../../include/physx/PxPhysicsAPI.h"

namespace Framework
{
	//TODO - roll into class as necessary
	using namespace physx;
	PxDefaultAllocator		gAllocator;
	PxDefaultErrorCallback	gErrorCallback;

	PxFoundation*			gFoundation = NULL;
	PxPhysics*				gPhysics = NULL;

	PxDefaultCpuDispatcher*	gDispatcher = NULL;
	PxScene*				gScene = NULL;

	PxMaterial*				gMaterial = NULL;

	PxPvd*                  gPvd = NULL;

	PxReal stackZ = 10.0f;

	mAccumulator = 0.0f;
	mStepSize = 1.0f / 60.0f;

	static Physics * Get()
	{
		static Physics physics;
		return &physics;
	}

	void Physics::Initialize()
	{
		gFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gAllocator, gErrorCallback);

		gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), true, gPvd);

		PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
		sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
		gDispatcher = PxDefaultCpuDispatcherCreate(2);
		sceneDesc.cpuDispatcher = gDispatcher;
		sceneDesc.filterShader = PxDefaultSimulationFilterShader;
		gScene = gPhysics->createScene(sceneDesc);

		gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);

		PxRigidStatic* groundPlane = PxCreatePlane(*gPhysics, PxPlane(0, 1, 0, 0), *gMaterial);
		gScene->addActor(*groundPlane);

		for (PxU32 i = 0; i<5; i++)
			createStack(PxTransform(PxVec3(0, 0, stackZ -= 10.0f)), 10, 2.0f);

		createDynamic(PxTransform(PxVec3(0, 40, 100)), PxSphereGeometry(10), PxVec3(0, -50, -100));
	} //end Initialize()

	bool advance(PxReal dt)
	{
		mAccumulator += dt;
		if (mAccumulator < mStepSize)
			return false;

		mAccumulator -= mStepSize;

		mScene->simulate(mStepSize);
		return true;
	}

	void Physics::Update(float dt = 0.0f)
	{
		if(advance(dt))
		{
			gScene->simulate(mStepSize);
			gScene->fetchResults(true);
		}
	} //end Update()

	Physics::~Physics()
	{
		gScene->release();
		gDispatcher->release();
		gPhysics->release();
		PxPvdTransport* transport = gPvd->getTransport();
		gPvd->release();
		transport->release();

		gFoundation->release();

		printf("Physics systems destructed.\n");
	} //end ~Physics()
}; // Framework