/******************************************************************************/
/*!
\file   TextFormator.cpp
\author Aleksey Perfilev
\par    Copyright 2017, Digipen Institute of Technology
\brief
implementation of TextFormator
*/
/******************************************************************************/
#include "../../Precompiled.h"

namespace Framework
{
	void TextFormator::Padding(std::ostream& os, const int tab_lvl)
	{
		for (int i = 0; i < tab_lvl; i++)
		{
			os << "  ";
		}
	}

} // Framework
