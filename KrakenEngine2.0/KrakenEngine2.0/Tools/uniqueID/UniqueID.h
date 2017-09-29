/******************************************************************************/
/*!
\file   UniqueID.h
\author Aleksey Perfilev
\par    Copyright 2017, Digipen Institute of Technology
\brief
Unique ID generator by type
*/
/******************************************************************************/
#ifndef UNIQUE_ID_H
#define UNIQUE_ID_H

namespace Framework
{
	template <typename TYPE>
	class UniqueID
	{
		static int AssignID()
		{
			static int id = 0;
			return ++id;
		}
	};

} // Framework
#endif
