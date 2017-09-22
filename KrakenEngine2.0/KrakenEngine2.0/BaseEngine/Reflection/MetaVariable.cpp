/******************************************************************************/
/*!
\file   MetaVariable.cpp
\author Aleksey Perfilev
\par    Copyright 2017, Digipen Institute of Technology
\brief
Stores information about MetaVariable, used by serialization
*/
/******************************************************************************/

#include "../../Precompiled.h"

namespace Framework
{
	MetaVariable::MetaVariable() : m_type(nullptr), m_data(nullptr) {}

	MetaVariable::MetaVariable(std::string name, void* data, const MetaType* type) : m_type(type), m_data(data), m_name(name) {}

	const MetaVariable& MetaVariable::Set(MetaType* type, void* data)
	{
		m_type = type;
		m_data = data;
		return *this;
	}

	std::string MetaVariable::Name() const
	{
		return m_name;
	}

	const MetaType* MetaVariable::Type() const
	{
		return m_type;
	}
	
	std::ostream& operator<<(std::ostream& os, const MetaVariable& metaVariable)
	{
		if (metaVariable.m_type->IsSerializible())
		{
			metaVariable.Serialize(os);
		}

		return os;
	}

	std::istream& operator>>(std::istream& is, MetaVariable& metaVariable)
	{
		if (metaVariable.m_type->IsDeserializible())
		{
			metaVariable.Deserialize(is);
		}

		return is;
	}


	void* MetaVariable::GetRowData() const 
	{
		return m_data;
	}

	std::ostream& MetaVariable::Serialize(std::ostream& os) const
	{
		Type()->Serialize(os, *this);
		return os;
	}

	std::istream& MetaVariable::Deserialize(std::istream& is)
	{
		Type()->Deserialize(is, *this);
		return is;
	}

#if _DEBUG
	void MetaVariable::Print() const
	{
		m_type->Print();
		std::cout << "  with address = " << m_data << std::endl;
	}

#endif

};