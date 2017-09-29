/******************************************************************************/
/*!
\file   ComponentID.h
\author Aleksey Perfilev
\par    Copyright 2017, Digipen Institute of Technology
\brief
Unique id for a component
*/
/******************************************************************************/
#ifndef COMPONENT_ID_H
#define COMPONENT_ID_H

namespace Framework
{
	class ComponentIDcounter
	{
	public:
		static int m_counter;
	};

	template <typename T>
	class ComponentID
	{
	public:
		static int GetID()
		{
			static int id = ComponentIDcounter::m_counter++;
			return id;
		}
	};
} // Framework
#endif
