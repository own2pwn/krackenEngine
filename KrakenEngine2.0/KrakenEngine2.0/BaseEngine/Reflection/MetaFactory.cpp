/******************************************************************************/
/*!
\file   MetaFactory.cpp
\author Aleksey Perfilev
\par    Copyright 2017, Digipen Institute of Technology
\brief
Impolementation of the MetaFactory
*/
/******************************************************************************/

#include "../../Precompiled.h"

namespace Framework
{
	std::unordered_map<std::string, MetaType*>& AllMetaTypes::Get()
	{
		static MetaTypesMap allTypes;
		return allTypes;
	}

	void AllMetaTypes::RegisterType(MetaType* metaType)
	{
		MetaTypesMap &map = Get();
		// Assertion: Type already registered
		//ASSERT(map[metaType->Name()] == nullptr);
		// if type already registered, silently continue
		if (map[metaType->Name()] == nullptr)
		{
			map[metaType->Name()] = metaType;
		}
	}
} // framework