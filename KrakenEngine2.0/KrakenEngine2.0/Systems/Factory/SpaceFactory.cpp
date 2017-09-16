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
		
	}
	
	void SpaceFactory::Update(float dt)
	{

	}

	SpaceFactory::~SpaceFactory()
	{
		for(auto space : m_spaces)
		{
			space.second->DestroyAllObjects();
			delete space.second;
		}
		m_spaces.clear();
		m_totalSpaces = 0;
	}

	ObjectFactory* SpaceFactory::AddSpace(std::string name)
	{
		m_totalSpaces++;
		m_spaces[m_totalSpaces] = new ObjectFactory(name);
		ASSERT(m_spaces.at(m_totalSpaces) != nullptr); // fail to create
		return m_spaces.at(m_totalSpaces);
	}

	unsigned int SpaceFactory::Size() const
	{
		return m_totalSpaces;
	}

	std::unordered_map<unsigned int, ObjectFactory*> SpaceFactory::GetSpaces()
	{
		return m_spaces;
	}

	ObjectFactory* SpaceFactory::GetSpace(unsigned int id)
	{
		ObjectFactory* spaceInst = nullptr;
		if (m_spaces.find(id) != m_spaces.end())
		{
			spaceInst = m_spaces.at(id);
		}
		return spaceInst;
	}

	SpaceFactory::SpaceFactory() : m_totalSpaces(0)
	{
		REGISTER_UNORDERED_MAP_OF_TYPE(unsigned int, ObjectFactory*);
		REGISTER_TYPE(SpaceFactory);
		ADD_MEMBER(SpaceFactory, m_spaces);
	}
} // Framework
