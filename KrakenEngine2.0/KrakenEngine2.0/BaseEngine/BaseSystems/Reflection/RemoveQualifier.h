/******************************************************************************/
/*!
\file   RemoveQualifier.h
\author Aleksey Perfilev
\par    Copyright 2017, Digipen Institute of Technology
\brief
Removes qualifiers from a variable.
*/
/******************************************************************************/
#ifndef REMOVE_QUALIFIER_H
#define REMOVE_QUALIFIER_H

namespace Framework
{
		// base case
	template <typename T>
	struct RemoveQualifier
	{
		typedef T Type;
	};

		// removes const qualifier
	template <typename T>
	struct RemoveQualifier<const T>
	{
		typedef typename RemoveQualifier<T>::Type Type;
	};

		// removes reference qualifier
	template <typename T>
	struct RemoveQualifier<T&>
	{
		typedef typename RemoveQualifier<T>::Type Type;
	};

		// removes const reference qualifier
	template <typename T>
	struct RemoveQualifier<const T&>
	{
		typedef typename RemoveQualifier<T>::Type Type;
	};

		// removes const qualifier
	template <typename T>
	struct RemoveQualifier<const T*>
	{
		typedef typename RemoveQualifier<T*>::Type Type;
	};

		// removes r-value qualifier
	template <typename T>
	struct RemoveQualifier<T&&>
	{
		typedef typename RemoveQualifier<T>::Type Type;
	};
}; // Framework
#endif
