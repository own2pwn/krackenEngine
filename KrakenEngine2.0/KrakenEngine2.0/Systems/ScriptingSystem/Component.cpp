/******************************************************************************/
/*!
\file   Component.cpp
\author Aleksey Perfilev
\par    Copyright 2017, Digipen Institute of Technology
\brief
Implementation of Component
*/
/******************************************************************************/
#include "../../Precompiled.h"

namespace Framework
{
	Component::Component() : m_ID(-1), m_owner(nullptr) {}

	void Component::Initialize() {}
	
	Component::~Component() {}

	std::string& Component::Name()
	{
		return m_name;
	}

	void Component::SetID(int id)
	{
		m_ID = id;
	}

	int Component::GetID() const
	{
		return m_ID;
	}

	void Component::SetOwner(GameObject* gameObject)
	{
		m_owner = gameObject;
	}

	GameObject* Component::Owner() const
	{
		return m_owner;
	}

} // Framework
