/******************************************************************************/
/*!
\file   Input.cpp
\author Matthew Gilbert
\par    Copyright 2017, Digipen Institute of Technology
\brief
Implementation of the physics system using the PhysX library.
*/
/******************************************************************************/
#include "../../Precompiled.h"

namespace Framework
{

//Input * Input::Get()
//{
//	static Input input;
//	return &input;
//}
//
//void Input::UpdateGamepadStates()
//{
//	for(int i = 0; i < MAX_CONTROLLERS; ++i)
//	{
//		DWORD result = XInputGetState(i, &controller_state_[i]);
//		if (result == ERROR_SEVERITY_SUCCESS)
//		{
//			controller_connected_[i] = true;
//		}
//		else
//		{
//			controller_connected_[i] = false;
//		}
//	}
//}
//
//
//void Input::Initialize()
//{
//	//Init states to zero
//	memset(controller_state_, 0, sizeof(XINPUT_STATE) * 4);
//
//	//Enable input
//	XInputEnable(true);
//	
//	//Poll for initial states of each controller
//	UpdateGamepadStates();
//}
//
//void Input::Update(float dt)
//{
//	UpdateGamepadStates();
//}
//
//_XINPUT_STATE const & Input::GetController(unsigned int number) const
//{
//	return controller_state_[number];
//}

} // end namespace Framework