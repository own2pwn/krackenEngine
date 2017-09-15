/******************************************************************************/
/*!
\file   main.cpp
\author Aleksey Perfilev
\par    Copyright 2017, Digipen Institute of Technology
\brief
Main file of the project, everything starts here.
*/
/******************************************************************************/

#include "Precompiled.h"
//#include "BaseEngine/Core/Core.h"

using namespace Framework;

int main()
{
#if _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
		// create and initialize core of the engine
	Core::Get().Initialize();
		// start running the engine
	Core::Get().Run();
		// everything dealocated automaticly when leaving scope
	
#if _DEBUG
		
	//std::cout <<  << std::endl;
	GET_TYPE_BY_STRING("int")->Print();
	MetaType* a = GET_TYPE_BY_TEMPLATE(double);
	a->Print();

	PRINT_TYPE_BY_TEMPLATE(double);

	std::string x = "hello";
	MetaVariable var;
	std::cout << var.Set(x, TURN_TO_STRING(x));

	PRINT_TYPE_BY_STRING("std::string");

	std::cout << var.Data<std::string>() << std::endl;
	std::cout << GET_TYPE_BY_STRING("int");
	
										  // Register a structure
	struct Object
	{
		int x;
		const char *objectName;
	};
	REGISTER_TYPE(Object);
	ADD_MEMBER(Object, x);
	ADD_MEMBER(Object, objectName);
	GET_TYPE_BY_STRING("Object")->Print();

	REGISTER_TYPE(MetaType);
	std::cout << GET_TYPE_BY_TEMPLATE(int);
	
	class A
	{
	public:
		A() : b(1.1f){}
		float b;
	};

	// TODO: check padding
	class B
	{
	public:
		B() : c(false),b(2.2f)  {}
		A a;
		const bool c;
		float b;
	};

	class C
	{
	public:
		C() : a(),c('c'), s("string")
		{
			vec.push_back(B());
			vec.push_back(B());
			vec.push_back(B());
			vec.push_back(B());

			map["1"] = A();
			map["2"] = A();
			map["3"] = A();
		}
		A a;
		B b;
		char c;
		std::string s;
		std::vector<B> vec;
		std::unordered_map<std::string, A> map;
	};

	REGISTER_VECTOR_OF_TYPE(B);
	REGISTER_UNORDERED_MAP("std::unordered_map<std::string, A>",std::string, A);

	REGISTER_TYPE(A);
	ADD_MEMBER(A, b);
	REGISTER_TYPE(B);
	ADD_MEMBER(B, a);
	ADD_MEMBER(B, b);
	ADD_MEMBER(B, c);
	
	REGISTER_TYPE(C);
	ADD_MEMBER(C, a);
	ADD_MEMBER(C, b);
	ADD_MEMBER(C, c);
	ADD_MEMBER(C, s);
	ADD_MEMBER(C, vec);
	ADD_MEMBER(C, map);
	
	C c;
	MetaVariable varC;
	varC.Set(c, "c");

	std::cout << varC;

	std::ofstream out("test.txt", std::ofstream::out);
	ASSERT(out.is_open());	
	if (!out.is_open())
	{
		std::cout << "Could not open test.txt" << std::endl;
		return false;
	}
	int Int = 5;
	MetaVariable mInt;
	mInt.Set(Int,"mInt");
	out << mInt;
	out.close();

	std::ifstream in("test.txt", std::ifstream::out);
	ASSERT(in.is_open());
	if (!in.is_open())
	{
		std::cout << "Could not open test.txt" << std::endl;
		return false;
	}
	//////////////////////////////////////////////////////////
	// TODO: allocate new variable(empty), or give a better 
	// thought to deserialize function using template specify
	// what kind of type we trying to deserialize
	//////////////////////////////////////////////////////////
/*	
	std::string string;
	MetaVariable devar;
	devar.Set(string, "string");
	in >> devar;
	//*/
	in.close();
//	std::cout << devar.Name();
	
	system("PAUSE");
#endif

	return 0;
}