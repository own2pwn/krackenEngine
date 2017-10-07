/******************************************************************************/
/*!
\file   Core.h
\author Aleksey Perfilev
\par    Copyright 2017, Digipen Institute of Technology
\brief
Core of the engine, stores all the systems, updates them, runs the engine.
*/
/******************************************************************************/
#ifndef CORE_H
#define CORE_H

namespace Framework
{
	class Core
	{
	public:
			// initilializes all the systems
		void Initialize();
			// Creates the Core and returns referens to it
		static Core & Get();
			// runs the engine
		void Run();
			// destructor
		~Core();
			// get all the meta information
		MetaVariable& Meta();
			// gets current state of the Core, notice that it return by reference, 
			// so you can set to pause for example using this function
		CoreStatesEnum& State();
			// gets current fps
		time_t FPS() const;
	private:
		Core();
			// adds a system to the core
		void AddSystem(ISystem * sytem);
			
			// initializes all the basic types for Reflection system
		void InitializeTypes();
		
		std::vector<ISystem*> m_Systems; // all the systems
		MetaVariable m_MetaCore;		 // MetaVariable for Core(which has whole engine info inside)
		CoreStatesEnum m_state;			 // current state of the engine
		
		time_t m_FPS; // current fps
		std::chrono::time_point<std::chrono::system_clock> m_current_time;  // current time
		std::chrono::time_point<std::chrono::system_clock> m_previous_time; // time for previous frame

			// updates FPS
		time_t UpdateFPS(float& dt);
	};
} // Framework

#endif