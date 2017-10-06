/******************************************************************************/
/*!
\file   CoreStates.h
\author Aleksey Perfilev
\par    Copyright 2017, Digipen Institute of Technology
\brief
States for Core.
*/
/******************************************************************************/
#ifndef CORE_STATES_H
#define CORE_STATES_H

namespace Framework
{
	enum CoreStatesEnum
	{
		cse_exiting = 0,
		cse_running,
		cse_paused
	};
}
#endif
