/******************************************************************************/
/*!
\file   Component.h
\author Aleksey Perfilev
\par    Copyright 2017, Digipen Institute of Technology
\brief
Inteface for Component
*/
/******************************************************************************/
#ifndef COMPONENT_H
#define COMPONENT_H

namespace Framework
{
		// forward declaration
	class GameObject;

	class Component
	{
	public:
			// constructor
		Component();
			// initializes component
		virtual void Initialize();
			// destructor
		virtual ~Component();
			// set/get Name
		std::string& Name();
			// sets id
		void SetID(int id);
			// Gets id
		int GetID() const;
			// Sets owner
		void SetOwner(GameObject* gameobject);
			// Gets owner
		GameObject* Owner() const;
			// gets GameObjects ID
		unsigned int ObjectID() const;
			// gets SpaceID
		unsigned int SpaceID() const;
	private:
		int m_ID;							// id corresponding to this component
		GameObject* m_owner;	// objects pointer
		std::string m_name;		// name of the Component
		//	std::unordered_map<int, Component*> m_components;
	};
} // Framework
#endif
