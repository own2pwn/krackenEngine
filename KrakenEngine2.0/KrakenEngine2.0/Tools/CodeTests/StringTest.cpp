/******************************************************************************/
/*!
\file   StringTest.cpp
\author Aleksey Perfilev
\par    Copyright 2017, Digipen Institute of Technology
\brief
Tests String class
*/
/******************************************************************************/
#include "../../Precompiled.h"
#if _DEBUG
namespace Framework
{
	void StringTest()
	{
		String str1;
		String str2;
		String str3("hello");
		std::string str4 = " world";
		String str5 = " world";
		/*
		std::cout << str1.ToInt() << std::endl;
		std::cout << str2.ToInt() << std::endl;
		std::cout << str3.ToInt() << std::endl;
		std::cout << str5.ToInt() << std::endl;
		*/
		std::cout << "is " << str1 << " empty? " << str1.IsEmpty() << std::endl;
		std::cout << "is " << str2 << " empty? " << str2.IsEmpty() << std::endl;
		std::cout << "is " << str3 << " empty? " << str3.IsEmpty() << std::endl;
		std::cout << "is " << str5 << " empty? " << str5.IsEmpty() << std::endl;

		std::cout << "length of " << str1 << " = " << str1.Size() << std::endl;
		std::cout << "length of " << str2 << " = " << str2.Size() << std::endl;
		std::cout << "length of " << str3 << " = " << str3.Size() << std::endl;
		std::cout << "length of " << str5 << " = " << str5.Size() << std::endl;

		if (str2 == str3)
			std::cout << str2 << " == " << str3.Get() << std::endl;
		else
			std::cout << str2 << " != " << str3 << std::endl;
		if (str2 == str1)
			std::cout << str1 << " == " << str2 << std::endl;
		else
			std::cout << str1 << " != " << str2 << std::endl;
		if (str3 == str4)
			std::cout << str3 << " == " << str4 << std::endl;
		else
			std::cout << str3 << " != " << str4 << std::endl;
		char ch;

		std::cout << str3 + str5 << std::endl;
		std::cout << str3 + str4 << std::endl;

		String str6 = str3 + str1 + str2;
		String str7(str3);
		if (str6 == str7)
			std::cout << str6 << " == " << str7 << std::endl;
		else
			std::cout << str6 << " != " << str7 << std::endl;

		str6 += str4;
		str7 += str5;
		std::cout << str6 << std::endl;
		std::cout << str7 << std::endl;
		
		std::cin >> ch;
	}
}; // Framework

#endif