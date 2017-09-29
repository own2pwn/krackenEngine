/******************************************************************************/
/*!
\file   MetaUserInterface.h
\author Aleksey Perfilev
\par    Copyright 2017, Digipen Institute of Technology
\brief
All the macros to manipulate Meta staff
*/
/******************************************************************************/
#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

namespace Framework
{
	// registers new basic type
#define REGISTER_BASIC_TYPE(TYPE) \
	MetaFactory<RemoveQualifier<TYPE>::Type> NAME_GENERATOR()(#TYPE, sizeof(TYPE)); \
	void MetaFactory<RemoveQualifier<TYPE>::Type>::RegisterMeta(); \
	GET_TYPE_BY_TEMPLATE(TYPE)->SetSerialize(SerializeBasicType<TYPE>); \
	GET_TYPE_BY_TEMPLATE(TYPE)->SetDeserialize(DeserializeType<TYPE>)

	// registers a new object type
#define REGISTER_TYPE(TYPE) \
	MetaFactory<RemoveQualifier<TYPE>::Type> NAME_GENERATOR()(#TYPE, sizeof(TYPE)); \
	void MetaFactory<RemoveQualifier<TYPE>::Type>::RegisterMeta(); \
	GET_TYPE_BY_TEMPLATE(TYPE)->SetSerialize(SerializeType<TYPE>)

	// registers a vector of specified type
#define REGISTER_VECTOR_OF_TYPE(TYPE) REGISTER_VECTOR_TYPE(std::vector<TYPE>,TYPE)

	// registers an unordered map of specified type
#define REGISTER_UNORDERED_MAP_OF_TYPE(TYPE_A,TYPE_B) \
	do { \
		std::string NAME = TURN_TO_STRING(unordered_map<TYPE_A); \
		NAME += ","; \
		NAME += TURN_TO_STRING(TYPE_B>); \
		MetaFactory<std::unordered_map<TYPE_A,TYPE_B> > NAME_GENERATOR()(NAME, sizeof(std::unordered_map<TYPE_A, TYPE_B>)); \
		void MetaFactory<std::unordered_map<TYPE_A, TYPE_B> >::RegisterMeta(); \
		GET_TYPE_BY_TEMPLATE(std::unordered_map<TYPE_A COMMA TYPE_B>)->SetSerialize(SerializeUnorderedMapType<TYPE_A, TYPE_B>); \
	} while(0)

	// registers a vector
#define REGISTER_VECTOR_TYPE(VECTOR, TYPE) \
	MetaFactory<RemoveQualifier< VECTOR >::Type> NAME_GENERATOR()( #VECTOR , sizeof(VECTOR)); \
	void MetaFactory<RemoveQualifier< VECTOR >::Type>::RegisterMeta(); \
	GET_TYPE_BY_TEMPLATE(VECTOR)->SetSerialize(SerializeVectorType< VECTOR , TYPE>)

	// adds member to a type(class)
#define ADD_MEMBER(TYPE, MEMBER) \
	 GET_TYPE_BY_TEMPLATE(TYPE)->AddMember(new MetaMember(#MEMBER, GET_OFFSET(TYPE, MEMBER), GET_TYPE_BY_TEMPLATE(GET_MEMBER_TYPE(TYPE, MEMBER))))

	// gets the type of a member
#define GET_MEMBER_TYPE(TYPE, MEMBER) \
	RemoveQualifier<decltype(TYPE::MEMBER)>::Type
	
	// Finds the offset of a member of a specified type.
#define GET_OFFSET(TYPE, MEMBER) \
	(size_t)(&(((TYPE*) NULL)->MEMBER))

	// Gets type info by template. Use example: Metatype* type = GET_TYPE_BY_TEMPLATE(int)
#define GET_TYPE_BY_TEMPLATE(T) MetaFactory<T>::Get()

	// Gets type info by template. Use example: Metatype* type = GET_TYPE_BY_STRING("int")
#define GET_TYPE_BY_STRING(STRING) Framework::AllMetaTypes::Get().find(STRING)->second

	// turns whatever you passed to it into a string
#define TURN_TO_STRING2(...) #__VA_ARGS__
#define TURN_TO_STRING(...) TURN_TO_STRING2(__VA_ARGS__)
	// anables ability to pass unordered_map type to a macro so its not getting confused by ,
#define COMMA ,
	/***********************************************
	*	for debuging purposes
	***********************************************/
	// for debuging
#if _DEBUG
	// prints type info by template. Use example: PRINT_TYPE_BY_TEMPLATE(int)
#define PRINT_TYPE_BY_TEMPLATE(T) \
	do{ \
		MetaType* type = GET_TYPE_BY_TEMPLATE(T); \
		std::cout << type; \
	} while (0)

	// prints type info by string. Use example: PRINT_TYPE_BY_STRING("int")
#define PRINT_TYPE_BY_STRING(T) \
	do{ \
			MetaType* type = GET_TYPE_BY_STRING(T); \
			std::cout << type; \
	} while (0)
#endif

} // Framework

#endif
