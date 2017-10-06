/******************************************************************************/
/*!
\file   Serialization.h
\author Aleksey Perfilev
\par    Copyright 2017, Digipen Institute of Technology
\brief
writes MetaVariable to ostrean and reads from istream.
*/
/******************************************************************************/
#ifndef SERIALIZATION_H
#define SERIALIZATION_H

namespace Framework
{
		// forward declaration
	class MetaVariable;
	
	//////////////////////////////
	// Serialization functions	//
	//////////////////////////////

		// serializes complex type(class/structure)
	template <typename T>
	void SerializeType(std::ostream& os, const MetaVariable& metaVar);

		// serializes basic types(int, bool, float etc.)
	template <typename T>
	void SerializeBasicType(std::ostream& os, const MetaVariable& metaVar);
	
		// specialization for bool(user friendly "true" and "false")
	template <>
	void SerializeBasicType<bool>(std::ostream& os, const MetaVariable& metaVar);
	
		// specialization for char(puts ' around char)
	template <>
	void SerializeBasicType<char>(std::ostream& os, const MetaVariable& metaVar);

		// specialization for string(puts " around char)
	template <>
	void SerializeBasicType<std::string>(std::ostream& os, const MetaVariable& metaVar);

	template <typename T, typename S>
	void SerializeVectorType(std::ostream& os, const MetaVariable& metaVar);

	template <typename T1, typename T2>
	void SerializeUnorderedMapType(std::ostream& os, const MetaVariable& metaVar);

	///////////////////////////////////////////////////////////////
	// Implementation of templated function from above //
	///////////////////////////////////////////////////////////////
	template <typename T>
	void SerializeType(std::ostream& os, const MetaVariable& metaVar)
	{
			// get the correct padding
		int& tab_lvl = TextFormator::TabLevel();

		TextFormator::Padding(os, tab_lvl);
		os << metaVar.Name() << std::endl; 

		TextFormator::Padding(os, tab_lvl); 
		os << "{\n";					
		tab_lvl++;
		for (auto member : metaVar.Type()->Members())
		{
			MetaVariable meta(member->Name(), (char*)metaVar.GetRowData() + member->Offset(), member->Type());
			os << meta;
		}
		tab_lvl--;
		TextFormator::Padding(os, tab_lvl);
		os << "}\n";
	}
	/*
	template <typename T>
	void SerializePointerType(std::ostream& os, const MetaVariable& metaVar)
	{
		// get the correct padding
		int& tab_lvl = TextFormator::TabLevel();

		TextFormator::Padding(os, tab_lvl);
		os << metaVar.Name() << std::endl;

		TextFormator::Padding(os, tab_lvl);
		os << "{\n";
		tab_lvl++;
		auto types = metaVar.Type()->Members();
		for (auto member : metaVar.Type()->Members())
		{
			MetaVariable meta(member->Name(), (char*)metaVar.GetRowData() + member->Offset(), member->Type());
			os << meta;
		}
		tab_lvl--;
		TextFormator::Padding(os, tab_lvl);
		os << "}\n";
	}
	//*/
	template <typename T>
	void SerializeBasicType(std::ostream& os, const MetaVariable& metaVar)
	{
		int& tab_lvl = TextFormator::TabLevel();
		TextFormator::Padding(os, tab_lvl);

		os << metaVar.Name();
		os << " = " << metaVar.Data<T>() << std::endl;
	}


	template <typename T, typename S>
	void SerializeVectorType(std::ostream& os, const MetaVariable& metaVar)
	{
		int& tab_lvl = TextFormator::TabLevel();
		TextFormator::Padding(os, tab_lvl);

		os << metaVar.Name() << " (vector):" << std::endl;

		TextFormator::Padding(os, tab_lvl);
		os << "{\n";
		tab_lvl++;
		for (auto it : metaVar.Data<T>())
		{
			MetaVariable meta("data", &it, GET_TYPE_BY_TEMPLATE(S));
			os << meta;

		}
		tab_lvl--;
		TextFormator::Padding(os, tab_lvl);
		os << "}\n";
	}
	/*
	template <typename T, typename S>
	void SerializeVectorPointerType(std::ostream& os, const MetaVariable& metaVar)
	{
		int& tab_lvl = TextFormator::TabLevel();
		TextFormator::Padding(os, tab_lvl);

		os << metaVar.Name() << " (vector):" << std::endl;

		TextFormator::Padding(os, tab_lvl);
		os << "{\n";
		tab_lvl++;
		for (auto it : metaVar.Data<T>())
		{
			MetaVariable meta("data", it, GET_TYPE_BY_TEMPLATE(S));
			os << meta;

		}
		tab_lvl--;
		TextFormator::Padding(os, tab_lvl);
		os << "}\n";
	}
	//*/
	template <typename T1, typename T2>
	void SerializeUnorderedMapType(std::ostream& os, const MetaVariable& metaVar)
	{
		int& tab_lvl = TextFormator::TabLevel();
		TextFormator::Padding(os, tab_lvl);

		os << metaVar.Name() << " (u_map):" << std::endl;

		TextFormator::Padding(os, tab_lvl);
		os << "{\n";
		tab_lvl++;
		for (auto it : metaVar.Data<std::unordered_map<T1,T2> >())
		{
			MetaVariable metaIndex("key", (char*)(&(it.first)), MetaFactory<T1>::Get());
			MetaVariable metaData("data", &(it.second), MetaFactory<T2>::Get());
			os << metaIndex << metaData << std::endl;

		}
		tab_lvl--;
		TextFormator::Padding(os, tab_lvl);
		os << "}\n";
	}


	//////////////////////////////////
	// Deserialization functions	//
	//////////////////////////////////

	template <typename T>
	void DeserializeType(std::istream& is, MetaVariable& metaVar)
	{
		//is >> metaVar.Data<T>();
	}

	/*
	class Serialization
	{
	public:
		static Serialization& Get();
		void Initialize();
		~Serialization();
		void RegisterVar(MetaVariable* var);
#if _DEBUG
		void PrintVars();
		void PrintVarsFull();
#endif
		void Serialize();
		void Unserialize();
		void SaveGame(std::string filename);
		bool JsonSave(std::string filename);
		void GatherInformation();
		void GatherInfoRec(MetaVariable* cur_var, Json::Value& cur_json);
	private:
		Serialization();

		Json::Value m_game;
		std::string m_autosave_file; // name of the autosave file in a folder
		std::vector<MetaVariable*> m_vars;
	};*/
}; // Framework

#endif