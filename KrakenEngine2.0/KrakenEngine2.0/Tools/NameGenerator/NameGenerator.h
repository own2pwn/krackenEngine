/******************************************************************************/
/*!
\file   NameGenerator.h
\author Aleksey Perfilev
\par    Copyright 2017, Digipen Institute of Technology
\brief
Generates name for a variable using predefined macro __COUNTER__
*/
/******************************************************************************/
//#ifndef NAME_GENERATOR
//#define NAME_GENERATOR
#pragma once

#define PASTE_TOKENS_2(A, B) A##B
#define PASTE_TOKENS(A, B) PASTE_TOKENS_2(A, B)
#define NAME_GENERATOR_INTERNAL(COUNTER) PASTE_TOKENS(GENERATED_TOKEN_, COUNTER)
#define NAME_GENERATOR() NAME_GENERATOR_INTERNAL(__COUNTER__)

//#endif
