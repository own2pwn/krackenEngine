/******************************************************************************/
/*!
\file   Space.cpp
\author Aleksey Perfilev
\par    Copyright 2017, Digipen Institute of Technology
\brief
Implementation of Object Factory
*/
/******************************************************************************/

#include "../../Precompiled.h"

namespace Framework
{
	Space::Space() : m_uniqueObjectID(0)
	{
		REGISTER_UNORDERED_MAP_OF_TYPE(unsigned int, GameObject*);
		REGISTER_TYPE(Space);
		ADD_MEMBER(Space, m_objects);
	}

	Space::Space(std::string name) : m_uniqueObjectID(0), m_name(name)
	{
		REGISTER_UNORDERED_MAP_OF_TYPE(unsigned int, GameObject*);
		REGISTER_TYPE(Space);
		ADD_MEMBER(Space, m_objects);
	}

	std::string& Space::Name()
	{
		return m_name;
	}

	void Space::Initialize()
	{

	}

	void Space::Update(float) // not using dt
	{
		for (auto id : m_objectsIDsToDestroy)
		{
#if _DEBUG
			if (m_objects.find(id) == m_objects.end())
			{
				std::cout << "trying to delete non existing element";
				continue;
			}
#endif
			m_objects[id]->ClearComponents();
			delete m_objects.at(id);
			m_objects.erase(id);
		}
		m_objectsIDsToDestroy.clear();
	}

	Space::~Space()
	{
		DestroyAllObjects();
	}

	void Space::DestroyAllObjects()
	{
		for (auto obj : m_objects)
		{
			obj.second->ClearComponents();
			delete obj.second;
		}

		m_objects.clear();
	}

	size_t Space::TotalObjects() const
	{
		return m_objects.size();
	}

	GameObject* Space::AddObject()
	{
		m_uniqueObjectID++;
		GameObject* gameObject = new GameObject; 
		gameObject->SetID(m_uniqueObjectID);
		m_objects[m_uniqueObjectID] = gameObject;
		ASSERT(m_objects.at(m_uniqueObjectID) != nullptr); // fail to create
		
		return m_objects.at(m_uniqueObjectID);
	}

	void Space::RemoveObject(const int id)
	{
		m_objectsIDsToDestroy.push_back(id);
	}

	std::unordered_map<unsigned int, GameObject*>* Space::GetObjects()
	{
		return &m_objects;
	}


	GameObject* Space::GetObject(unsigned int id)
	{
#if _DEBUG
		if (m_objects.find(id) == m_objects.end())
		{
			return nullptr;
		}
#endif
		return m_objects.at(id);
	}

	GameObject* Space::operator[](unsigned int id)
	{
		return GetObject(id);
	}

} // Framework
