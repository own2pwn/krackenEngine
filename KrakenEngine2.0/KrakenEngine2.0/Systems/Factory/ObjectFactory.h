/******************************************************************************/
/*!
\file   ObjectFactory.h
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
	class ObjectFactory
	{
	public:
		ObjectFactory();
		ObjectFactory(std::string name);

		virtual void Initialize();

		virtual void Update(float dt = 0.0f);
		// destructor
		virtual ~ObjectFactory();

		void DestroyAllObjects();
	private:
		//std::vector<GameObject*> m_spaces;
		std::string m_name;
	};
} // Framework
#endif
