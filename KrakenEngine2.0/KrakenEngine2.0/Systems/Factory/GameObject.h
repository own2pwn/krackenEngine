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
		// forward declaration
	class Space;

	class GameObject
	{
	public:
			// adds a new component by template
		template <typename T>
		void AddComponent();
			// adds a new component by template
		template <typename T>
		void AddComponent(Component* component);
			// removes a component
		template <typename T>
		void RemoveComponent();
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
			// Gets Space ID
		unsigned int SpaceID() const;
			// sets Owner
		void SetOwner(Space* space);
			// gets Owner
		Space* GetOwner();
	private:
		unsigned int m_ID;	// id corresponding to this gameobject in current space
		Space* m_owner;			// pointer to the space object in
		std::string m_name;	// name of the object
		std::unordered_map<int, Component*> m_components;	// all the components
	};


///////////////////////////////////////////
// implementation of templated functions // 
///////////////////////////////////////////
	template <typename T>
	void GameObject::AddComponent()
	{	
			// getting unque id corresponding to this type of component
		int id = ComponentID<T>::GetID();

			// creation
		Component* component = new T;
		ASSERT(component != nullptr); // did not allocate
		m_components[id] = component;

		// register with the ComponentVector
		ComponentVector<T>::AddComponent(component);

			// initialization
		component->SetID(id);
		component->SetOwner(this);
		component->Initialize();
	}

	template <typename T>
	void GameObject::AddComponent(Component* component)
	{
		// getting unque id corresponding to this type of component
		int id = ComponentID<T>::GetID();

		// creation
		m_components[id] = component;

		// register with the ComponentVector
		ComponentVector<T>::AddComponent(component);

		// initialization
		component->SetID(id);
		component->SetOwner(this);
	}

	template <typename T>
	void GameObject::RemoveComponent()
	{
		int id = ComponentID<T>::GetID();
		Component * component = nullptr;

		try
		{
			component = dynamic_cast<T*>(m_components.at(id));
		}
		catch (std::out_of_range ex)
		{
			ASSERT(false); // on debug saying that component not found, on release silintly move on
		}

		if (component)
		{
			ComponentVector<T>::Remove(component);
			delete component;
				// in case somone trys to access deleted element later
			component = nullptr;
		}
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
