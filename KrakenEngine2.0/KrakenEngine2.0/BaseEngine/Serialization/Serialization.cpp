/******************************************************************************/
/*!
\file   Serialization.cpp
\author Aleksey Perfilev
\par    Copyright 2017, Digipen Institute of Technology
\brief
Implementation of specializations.
*/
/******************************************************************************/
#include "../../Precompiled.h"

namespace Framework
{
	template <>
	void SerializeBasicType<bool>(std::ostream& os, const MetaVariable& metaVar)
	{
		int& tab_lvl = TextFormator::TabLevel();
		TextFormator::Padding(os, tab_lvl);
		os << metaVar.Name();
		os << " = " << (metaVar.Data<bool>() ? "true" : "false") << std::endl;
	}

	template <>
	void SerializeBasicType<char>(std::ostream& os, const MetaVariable& metaVar)
	{
		int& tab_lvl = TextFormator::TabLevel();
		TextFormator::Padding(os, tab_lvl);
		os << metaVar.Name();
		os << " = '" << metaVar.Data<char>() << "'" << std::endl;
	}

	template <>
	void SerializeBasicType<std::string>(std::ostream& os, const MetaVariable& metaVar)
	{
		int& tab_lvl = TextFormator::TabLevel();
		TextFormator::Padding(os, tab_lvl);
		os << metaVar.Name();
		os << " = \"" << metaVar.Data<std::string>() << "\"" << std::endl;
	}

	



//	Serialization& Serialization::Get()
//	{
//		static Serialization serialization;
//		return serialization;
//	}
//
//	void Serialization::Initialize()
//	{
//
//	}
//
//	Serialization::~Serialization()
//	{
//		
//	}
//
//	void Serialization::RegisterVar(MetaVariable* var)
//	{
//			// see if the var already registered and needs to be updated
//		auto it = std::find(m_vars.begin(), m_vars.end(), var);
//		if (it != m_vars.end())
//			m_vars.erase(it);
//	
//		m_vars.push_back(var);
//	}
//
//#if _DEBUG
//	
//	void Serialization::PrintVars()
//	{
//		for (auto var : m_vars)
//		{
//			var->Print();
//		}
//	}
//
//	void Serialization::PrintVarsFull()
//	{/*
//		for (auto var : m_vars)
//		{
//			var->PrintFull();
//		}*/
//	}
//
//#endif
//
//	void Serialization::Serialize()
//	{
//
//	}
//
//	void Serialization::Unserialize()
//	{
//
//	}
//
//	void Serialization::SaveGame(std::string filename)
//	{
//			// gather current data
//		GatherInformation();
//			// save to file
//		if (JsonSave(filename))
//		{
//#if _DEBUG
//			std::cout << "Game saved\n\n";
//#endif
//		}
//		else
//		{
//#if _DEBUG
//			std::cout << "Failed to save\n\n";
//#endif
//		}
//
//	}
//
//	bool Serialization::JsonSave(std::string filename)
//	{
//		std::ofstream out(filename, std::ofstream::out);
//		ASSERT(out.is_open());		// ATTENTION TEAM: if your game breaks here you need to check out some .json file from perforce(you dont have permitions to modify it right now)
//		if (!out.is_open())
//		{
//#if _DEBUG
//			std::cout << "Could not open " << filename << std::endl;
//#endif
//			return false;
//		}
//
//		out << m_game;
//		out.close();
//		return true;
//	}
//
//	Serialization::Serialization()
//	{
//		char *pValue;
//		size_t len;
//		errno_t err = _dupenv_s(&pValue, &len, "APPDATA");
//		ASSERT(err == 0);
//		m_autosave_file = pValue;
//		m_autosave_file += "\\RaceBoy";
//		_mkdir(m_autosave_file.c_str());
//		m_autosave_file += "\\autosave.json";
//	}
//
//	void Serialization::GatherInformation()
//	{
//		 // clear old data
//		m_game.clear();
//
//		for (auto var : m_vars)
//		{
//			GatherInfoRec(var, m_game);
//		}
//	}
//
//	void Serialization::GatherInfoRec(MetaVariable* cur_var, Json::Value& cur_json)
//	{/*
//		if (cur_var->GetMembers().empty())
//		{
//			cur_json[cur_var->GetName()] = cur_var->GetAddress();
//		}*/
//	}
//	
}; // Framework
