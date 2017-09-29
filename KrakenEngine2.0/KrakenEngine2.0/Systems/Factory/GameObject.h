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
		void ClearComponents();
	private:
		//std::vector<GameObject*> m_spaces;
		std::string m_name;
	//	std::unordered_map<int, Component*> m_components;
	};
} // Framework
#endif
