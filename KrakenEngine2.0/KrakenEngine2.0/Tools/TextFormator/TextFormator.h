/******************************************************************************/
/*!
\file   TextFormator.h
\author Aleksey Perfilev
\par    Copyright 2017, Digipen Institute of Technology
\brief
tool to farmat text
*/
/******************************************************************************/
#ifndef TEXT_FORMATOR_H
#define TEXT_FORMATOR_H

namespace Framework
{
	class TextFormator
	{
	public:
			// used to insert tabs into the text
		static void Padding(std::ostream& os, const int tab_lvl);
		
		static int& TabLevel()
		{
			static int tab_lvl = 0;	// keeps track of how many tabs to insert
			return tab_lvl;
		}
	};
} // Framework

#endif