/******************************************************************************/
/*!
\file   Core.cpp
\author Aleksey Perfilev
\par    Copyright 2017, Digipen Institute of Technology
\brief
Implementation of the Core class.
*/
/******************************************************************************/

#include "../../Precompiled.h"

namespace Framework
{
	Core & Core::Get()
	{
		static Core core;
		return core;
	}

	void Core::Initialize()
	{
			// Setting up Meta
		REGISTER_VECTOR_OF_TYPE(ISystem*);
		REGISTER_TYPE(Core);
		ADD_MEMBER(Core, m_Systems);

		m_MetaCore.Set(Get(), "m_MetaCore");

		// AddSystem(AudioSystem::Get());
		// AddSystem(Windows::Get());
		// AddSystem(GameLogic::Get()); // next semester
		// AddSystem(Graphics::Get());
		// AddSystem(Scripting::Get());
		// AddSystem(EventSystem::Get());
		// AddSystem(MenuSystem::Get());
		// AddSystem(Physics::Get());
		// AddSystem(ParticleSystem::Get());
		 AddSystem(SpaceFactory::Get()); // factory have to be last due update function in it
		// AddSystem(Serialization::Get()); // have to go after serialization

	}

	void Core::Run()
	{
		//using namespace std::chrono;
		m_state = cse_running;	// update state of the engine
		
		float dt = 0; // the dt used for systems
		
		while (m_state)
		{
				// if we run to slow => make it smooth(prevents physics from doing wierd stuff)
			float maxdt = 1.0f / 40;
			if (dt > maxdt)
				dt = maxdt;

				// update all the systems
			for (unsigned i = 0; i < m_Systems.size(); i++)
				m_Systems.at(i)->Update(dt);

				// updating dt and FPS
				// locking FPS to 60(wait if more)
			while (UpdateFPS(dt) > 60) {}

			m_previous_time = m_current_time;
			std::cout << m_FPS << std::endl;

		//	MetaTypesMap map = AllMetaTypes::Get();
			//break;
		}

	}

	Core::~Core() {}

	Core::Core() : m_state(cse_exiting), m_FPS(0), m_current_time(std::chrono::system_clock::now())
	{
		InitializeTypes();
	}

	void Core::AddSystem(ISystem * system)
	{
		m_Systems.push_back(system);
	}

	void Core::InitializeTypes()
	{
		REGISTER_BASIC_TYPE(bool);
		REGISTER_BASIC_TYPE(char);
		REGISTER_BASIC_TYPE(short);
		REGISTER_BASIC_TYPE(unsigned short);
		REGISTER_BASIC_TYPE(int);
		REGISTER_BASIC_TYPE(unsigned);
		REGISTER_BASIC_TYPE(unsigned int);
		REGISTER_BASIC_TYPE(long);
		REGISTER_BASIC_TYPE(unsigned long);
		REGISTER_BASIC_TYPE(float);
		REGISTER_BASIC_TYPE(double);
		REGISTER_BASIC_TYPE(bool*);
		REGISTER_BASIC_TYPE(char*);
		REGISTER_BASIC_TYPE(short*);
		REGISTER_BASIC_TYPE(unsigned short*);
		REGISTER_BASIC_TYPE(int*);
		REGISTER_BASIC_TYPE(unsigned*);
		REGISTER_BASIC_TYPE(unsigned int*);
		REGISTER_BASIC_TYPE(long*);
		REGISTER_BASIC_TYPE(unsigned long*);
		REGISTER_BASIC_TYPE(float*);
		REGISTER_BASIC_TYPE(double*);
		REGISTER_BASIC_TYPE(std::string);

		REGISTER_TYPE(String);
	}

	MetaVariable& Core::Meta()
	{
		return m_MetaCore;
	}

	CoreStatusEnum& Core::State()
	{
		return m_state;
	}

	time_t Core::FPS() const
	{
		return m_FPS;
	}

	time_t Core::UpdateFPS(float & dt)
	{
		using namespace std::chrono;

			// get current time
		m_current_time = system_clock::now();
			// calculate how much time passed since last frame
		duration<float> elapsed_seconds = m_current_time - m_previous_time;
			// transfer to miliseconds and update dt
		dt = elapsed_seconds.count();
			// update FPS
		m_FPS = int(1 / dt + 0.5f);
		return m_FPS;
	}
}; // Framework
