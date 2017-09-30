/******************************************************************************/
/*!
\file   GameObject.cpp
\author Aleksey Perfilev
\par    Copyright 2017, Digipen Institute of Technology
\brief
Implementation of GameObject
*/
/******************************************************************************/
#include "../../Precompiled.h"

namespace Framework
{
	GameObject::GameObject() : m_ID(0)
	{		
		REGISTER_UNORDERED_MAP_OF_TYPE(int, Component*);
		REGISTER_TYPE(GameObject);
		ADD_MEMBER(GameObject, m_ID);
		ADD_MEMBER(GameObject, m_name);
		ADD_MEMBER(GameObject, m_components);
	}

	void GameObject::ClearComponents()
	{
		for (auto it : m_components)
		{
			delete it.second;
		}
		m_components.clear();
	}

	std::string& GameObject::Name()
	{
		return m_name;
	}

	void GameObject::SetID(int id)
	{
		m_ID = id;
	}

	int GameObject::GetID() const
	{
		return m_ID;
	}

} // Framework
