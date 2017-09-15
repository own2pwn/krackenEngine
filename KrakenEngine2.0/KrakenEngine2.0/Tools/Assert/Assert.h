/******************************************************************************/
/*!
\file   Assert.h
\author Aleksey Perfilev
\par    Copyright 2017, Digipen Institute of Technology
\brief
debug tool, type ASSERT(<expression>) for debuging in your code. Works ONLY IN DEBUG mode, deletes itself in release
*/
/******************************************************************************/
#ifndef ASSERT_H
#define ASSERT_H

#if _DEBUG
#define ASSERT(exp) \
do                  \
{                   \
	if (!(exp))     \
	{               \
	__debugbreak(); \
	}               \
} while (false)
#else
#define ASSERT(exp)
#endif

#endif
