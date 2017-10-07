/******************************************************************************/
/*!
\file   Space.h
\author Aleksey Perfilev
\par    Copyright 2017, Digipen Institute of Technology
\brief
Creates and manages Game Objects
*/
/******************************************************************************/
#ifndef OBJECT_FACTORY_H
#define OBJECT_FACTORY_H

namespace Framework
{
	class SpaceFactory;

	class Space
	{
	public:
			// creates a space with the name
		Space(std::string name);
			// Gets the name of the space
		std::string& Name();
			// Initializes space
		virtual void Initialize();
			// updates space
		virtual void Update(float dt = 0.0f);
			// destructor
		virtual ~Space();
			// destroys all the objects in this space
		void DestroyAllObjects();
			// how many objects in this space
		size_t TotalObjects() const;
			// addds an object to this space
		GameObject* AddObject();
			// removes an object from this space
		void RemoveObject(const int id);
			// gets all objects in this space
		std::unordered_map<unsigned int, GameObject*>* GetObjects();
			// gets an object with specified id
		GameObject* GetObject(unsigned int id);
			// get a specific space
		GameObject* operator[](unsigned int id);
			// sets space id
		void SetID(unsigned int id);
			// gets space id
		unsigned int GetID() const;
	private:
		Space();

		std::unordered_map<unsigned int, GameObject*> m_objects; // all the objects in this space
		unsigned int m_uniqueObjectID;													 // makes unique IDs for objects
		unsigned int m_ID;																			 // id of this space in the spacefactory
		std::string m_name;																			 // name of this sapce
		std::vector<unsigned int> m_objectsIDsToDestroy;				 // objects to destroy this frame
	};
} // Framework
#endif
