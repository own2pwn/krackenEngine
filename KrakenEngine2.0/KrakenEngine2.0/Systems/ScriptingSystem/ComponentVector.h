/******************************************************************************/
/*!
\file   ComponentVector.h
\author Aleksey Perfilev
\par    Copyright 2017, Digipen Institute of Technology
\brief
Hold diferent components in their own vectors to ease the use of components/scripts
*/
/******************************************************************************/
#ifndef COMPONENT_VECTOR_H
#define COMPONENT_VECTOR_H

namespace Framework
{
	template <typename T>
	class ComponentVector
	{
	public:
			// Adds a component
		static void AddComponent(Component* component);
			// Gets a vector
		static std::vector<Component*> & Get();
			// Removes a component
		static void Remove(Component* component);
	};

	///////////////////////////////////////////
	// implementation of the templated class //
	///////////////////////////////////////////
	template<typename T>
	void ComponentVector<T>::AddComponent(Component* component)
	{
		std::vector<Component*> & componentVector = Get();
		componentVector.push_back(component);
	}

	template<typename T>
	std::vector<Component*> & ComponentVector<T>::Get()
	{
		static std::vector<Component*> componentVector;
		return componentVector;
	}
	template<typename T>
	void ComponentVector<T>::Remove(Component* component)
	{
		std::vector<Component*> & componentVector = Get();
		auto it = std::find(componentVector.begin(), componentVector.end(), component);
		if (it  != componentVector.end())
		{
			std::swap(*it, componentVector.back());
			componentVector.pop_back();
		}
		else
		{
			ASSERT(false); // trying to delete not existing component, on release will silintly continue
		}

	}
}
#endif
