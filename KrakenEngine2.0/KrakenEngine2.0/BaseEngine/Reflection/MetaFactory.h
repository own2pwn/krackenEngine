/******************************************************************************/
/*!
\file   MetaFactory.h
\author Aleksey Perfilev
\par    Copyright 2017, Digipen Institute of Technology
\brief
Factory of all the singletons
*/
/******************************************************************************/
#ifndef META_FACTORY_H
#define META_FACTORY_H

namespace Framework
{
	/***********************************************
	*	AllMetaTypes stores all the MetaTypes in one hash table
	*	usefull to GET_TYPE_BY_STRING
	***********************************************/
	typedef std::unordered_map<std::string, MetaType*> MetaTypesMap;

	class AllMetaTypes
	{
	public:
			// gets the map of all MetaTypes 
		static MetaTypesMap& Get();
			// registers MetaType with Factory(puts it into the map)
		static void RegisterType(MetaType* metaType);
	};

	/***********************************************
	*	MetaFactory stores all the MetaTypes using templates
	*	usefull to GET_TYPE_BY_TEMPLATE
	***********************************************/
	template <typename T>
	class MetaFactory
	{
	public:	
			// constructor
		MetaFactory(std::string name, size_t size);
			// Initializes current type
		static void Initialize(std::string name, size_t size);
			// casts NULL to given type(places type at 0 memory(hypothetically))
		static T* NullCast();
			// registers given MetaType with factories
		static void RegisterMeta();
			// Gets type
		static MetaType* Get();
			// Sets Serialize function
		static void SetSerialize(SerializeFn fn);
	};

	///////////////////////////////////////////
	// implementation of the templated class //
	///////////////////////////////////////////
	template <typename T>
	MetaFactory<T>::MetaFactory(std::string name, size_t size)
	{
		Initialize(name, size);
	}

	template <typename T>
	void MetaFactory<T>::Initialize(std::string name, size_t size)
	{
		Get()->Initialize(name, size);
		RegisterMeta();
	}

	template <typename T>
	T* MetaFactory<T>::NullCast()
	{
		return reinterpret_cast<T*> (NULL);
	}

	template <typename T>
	void MetaFactory<T>::RegisterMeta()
	{
		MetaType *meta = Get();
		AllMetaTypes::RegisterType(meta);
	}

	template <typename T>
	MetaType* MetaFactory<T>::Get()
	{
		static MetaType instance;
		return &instance;
	}

	template <typename T>
	void MetaFactory<T>::SetSerialize(SerializeFn fn)
	{
		Get()->SetSerialize(fn);
	}
}; // Framework
#endif