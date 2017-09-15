/******************************************************************************/
/*!
\file   MetaFactory.h
\author Aleksey Perfilev
\par    Copyright 2017, Digipen Institute of Technology
\brief
Factory of all the singletons
*/
/******************************************************************************/
#ifndef META_FACTORY
#define META_FACTORY

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
		static MetaTypesMap& Get()
		{
			static MetaTypesMap map;
			return map;
		}

		static void RegisterType(MetaType* metaType)
		{
			MetaTypesMap &map = Get();
				// Assertion: Type already registered
			ASSERT(map[metaType->Name()] == nullptr);
			map[metaType->Name()] = metaType;
		}
	};

	/***********************************************
	*	MetaFactory stores all the MetaTypes using templates
	*	usefull to GET_TYPE_BY_TEMPLATE
	***********************************************/
	template <typename T>
	class MetaFactory
	{
	public:
		MetaFactory(std::string name, size_t size)
		{
			Initialize(name, size);
		}

		static void Initialize(std::string name, size_t size)
		{
			Get()->Initialize(name, size);
			RegisterMeta();
		}

		static T* NullCast()
		{
			return reinterpret_cast<T*> (NULL);
		}

		static void RegisterMeta()
		{
			MetaType *meta = Get();
			AllMetaTypes::RegisterType(meta);
		}

		static MetaType* Get()
		{
			static MetaType instance;
			return &instance;
		}

		static void SetSerialize(SerializeFn fn)
		{
			Get()->SetSerialize(fn);
		}
	};
}; // Framework
#endif