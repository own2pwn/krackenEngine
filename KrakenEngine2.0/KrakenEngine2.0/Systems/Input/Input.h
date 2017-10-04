/******************************************************************************/
/*!
\file   Input.h
\author Matthew Gilbert
\par    Copyright 2017, Digipen Institute of Technology
\brief
Interface for the input system.
*/
/******************************************************************************/
#ifndef INPUT_H
#define INPUT_H

namespace Framework
{
	//#define MAX_CONTROLLERS 4
	//class Input : public ISystem
	//{
	//public:
	//	// Access to this system
	//	static Input * Get();
	//	// Sets up initial states
	//	virtual void Initialize();
	//	// Updates the state of each connected controller
	//	virtual void Update(float dt = 0.0f);
	//	// Get the state for a given controller number
	//	_XINPUT_STATE const & GetController(unsigned int number) const;
	//	// Destructor
	//	virtual ~Input();
	//private:
	//	void UpdateGamepadStates(void);
	//	// TODO Keyboard state
	//	// TODO Mouse state
	//	unsigned int num_controllers_;
	//	_XINPUT_STATE controller_state_[MAX_CONTROLLERS];
	//	bool controller_connected_[MAX_CONTROLLERS] = { false };
	//};
	//}; //end class Input
} // end namespace Framework

#endif