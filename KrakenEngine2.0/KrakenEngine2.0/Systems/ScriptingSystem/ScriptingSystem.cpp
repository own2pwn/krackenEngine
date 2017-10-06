/******************************************************************************/
/*!
\file   ScriptingSystem.cpp
\author Aleksey Perfilev
\par    Copyright 2017, Digipen Institute of Technology
\brief
Implementation of ScriptingSystem
*/
/******************************************************************************/
#include "../../Precompiled.h"

namespace Framework
{
	ScriptingSystem* ScriptingSystem::Get()
	{
		static ScriptingSystem scriptingSystem;
		return &scriptingSystem;
	}

	void ScriptingSystem::Initialize()
	{
		m_scripts.clear();
	}

	void ScriptingSystem::Update(float dt)
	{
		for (auto script : m_scripts)
		{
			script->Update(dt);
		}
	}

	std::vector<Script*>& ScriptingSystem::GetAllScripts()
	{
		return m_scripts;
	}

	void ScriptingSystem::Register(Script* script)
	{
		m_scripts.push_back(script);
	}
	
	void ScriptingSystem::Unregister(Script* script)
	{
		auto index = std::find(m_scripts.begin(), m_scripts.end(), script);
		if (index != m_scripts.end())
		{
			m_scripts.erase(index);
		}
	}

	ScriptingSystem::~ScriptingSystem()
	{
		for (unsigned i = 0; i < m_scripts.size(); ++i)
		{
			m_scripts.pop_back();
		}
	}

	ScriptingSystem::ScriptingSystem()
	{
		REGISTER_VECTOR_OF_TYPE(Script*);
		REGISTER_TYPE(ScriptingSystem);
		ADD_MEMBER(ScriptingSystem, m_scripts);
	}
} // Framework
