/******************************************************************************/
/*!
\file   GameObject.h
\author Aleksey Perfilev
\par    Copyright 2017, Digipen Institute of Technology
\brief
Inteface for GameObject
*/
/******************************************************************************/
#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

namespace Framework
{
	class GameObject
	{
	public:
			// adds a new component by template
		template <typename T>
		void AddComponent();
			// get a component by template
		template <typename T>
		T* GetComponent();
			// deletes all the components from this game object 
		void ClearComponents();
			// constructor	
		GameObject();
			// Gets/sets name
		std::string& Name();
			// Sets ID
		void SetID(int id);
		  // Gets ID
		int GetID() const;
	private:
		unsigned int m_ID;
		std::string m_name;
		std::unordered_map<int, Component*> m_components;
	};

///////////////////////////////////////////
// implementation of templated functions // 
///////////////////////////////////////////
	template <typename T>
	void GameObject::AddComponent()
	{
		int id = ComponentID<T>::GetID();

		m_components[id] = new T;
		ASSERT(m_components.at(id) != nullptr);

	}

	template <typename T>
	T* GameObject::GetComponent()
	{
		int id = ComponentID<T>::GetID();

		T* component;
		try
		{
			component = dynamic_cast<T*>(m_components.at(id));
		}
		catch (std::out_of_range ex)
		{
			return nullptr;
		}

		return component;
	}

} // Framework
#endif
