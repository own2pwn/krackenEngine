/******************************************************************************/
/*!
\file   ScriptingSystem.h
\author Aleksey Perfilev
\par    Copyright 2017, Digipen Institute of Technology
\brief
Scripting system, manages scripts
*/
/******************************************************************************/
#ifndef SCRIPTING_SYSTEM_H
#define SCRIPTING_SYSTEM_H

namespace Framework
{
	class ScriptingSystem :public ISystem
	{
	public:
			// gets pointer to scripting system
		static ScriptingSystem* Get();
			// initializes the system
		void Initialize() override;
			// updates the system
		void Update(float dt) override;
			// Gets all the scripts
		std::vector<Script*>& GetAllScripts();
			// Registesrs script with Scripting System
		void Register(Script* script);
			// Unregisters script
		void Unregister(Script* script);
			// destructor
		~ScriptingSystem();
	private:
			// constructor hiden from user
		ScriptingSystem();
			// all the scripts
		std::vector<Script*> m_scripts;

	};
} // Framework
#endif
