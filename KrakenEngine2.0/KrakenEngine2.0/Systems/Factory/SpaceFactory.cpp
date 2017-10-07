/******************************************************************************/
/*!
\file   SpaceFactory.cpp
\author Aleksey Perfilev
\par    Copyright 2017, Digipen Institute of Technology
\brief
Implementation of Factory
*/
/******************************************************************************/

#include "../../Precompiled.h"

namespace Framework
{
	SpaceFactory * SpaceFactory::Get()
	{
		static SpaceFactory spaceFactory;
		return &spaceFactory;
	}


	void SpaceFactory::Initialize()
	{
		for (auto space : m_spaces)
		{
			space.second->Initialize();
		}
	}
	
	void SpaceFactory::Update(float dt)
	{
		for (auto space : m_spaces)
		{
			space.second->Update(dt);
		}
	}

	SpaceFactory::~SpaceFactory()
	{
		for(auto space : m_spaces)
		{
			space.second->DestroyAllObjects();
			delete space.second;
		}
		m_spaces.clear();
	}

	Space* SpaceFactory::AddSpace(std::string name)
	{
		m_uniqueFactoryID++;
		Space* space = new Space(name);
		space->SetID(m_uniqueFactoryID);
		m_spaces[m_uniqueFactoryID] = space;
		ASSERT(m_spaces.at(m_uniqueFactoryID) != nullptr); // fail to create
		
		return m_spaces.at(m_uniqueFactoryID);
	}

	size_t SpaceFactory::Size() const
	{
		return m_spaces.size();
	}

	std::unordered_map<unsigned int, Space*>* SpaceFactory::GetSpaces()
	{
		return &m_spaces;
	}

	Space* SpaceFactory::GetSpace(unsigned int id)
	{
#if _DEBUG
		if (m_spaces.find(id) == m_spaces.end())
		{
			return nullptr;
		}
#endif
		return m_spaces.at(id);
	}

	Space* SpaceFactory::operator[](unsigned int id)
	{
		return GetSpace(id);
	}


	SpaceFactory::SpaceFactory() : m_uniqueFactoryID(0)
	{
		REGISTER_UNORDERED_MAP_OF_TYPE(unsigned int, Space*);
		REGISTER_TYPE(SpaceFactory);
		ADD_MEMBER(SpaceFactory, m_spaces);
	}
} // Framework
