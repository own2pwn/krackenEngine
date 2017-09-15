/******************************************************************************/
/*!
\file   MetaType.cpp
\author Aleksey Perfilev
\par    Copyright 2017, Digipen Institute of Technology
\brief
Stores information about types
*/
/******************************************************************************/

#include "../../../Precompiled.h"

namespace Framework
{
	////////////////////////////
	// MetaType
	////////////////////////////

	const std::string& MetaType::Name() const
	{
		return m_name;
	}

	size_t MetaType::Size() const
	{
		return m_size;
	}

	const std::vector<MetaMember const *>& MetaType::Members() const
	{
		return m_members;
	}

	bool MetaType::HasMembers() const
	{
		return !m_members.empty();
	}

	void MetaType::AddMember(MetaMember const * member)
	{
		m_members.push_back(member);
	}

	void MetaType::SetSerialize(SerializeFn fn = nullptr)
	{
		m_serialize = fn;
	}

	bool MetaType::IsSerializible() const
	{
		return m_serialize;
	}

	void MetaType::SetDeserialize(DeserializeFn fn = nullptr)
	{
		m_deserialize = fn;
	}

	bool MetaType::IsDeserializible() const
	{
		return m_deserialize;
	}

	std::ostream& MetaType::Serialize(std::ostream& os, const MetaVariable& metaVar) const
	{
		if (m_serialize != nullptr)
		{
			m_serialize(os, metaVar);
		}
		return os;
	}

	std::istream& MetaType::Deserialize(std::istream& is, MetaVariable& metaVar) const
	{
		if (m_deserialize != nullptr)
		{
			m_deserialize(is, metaVar);
		}
		return is;
	}

	std::ostream& operator<<(std::ostream& os, const MetaType& type)
	{
		int& tab_lvl = TextFormator::TabLevel();
		TextFormator::Padding(os, tab_lvl);
		os << "type = " << type.Name() << "(" << type.Size() << ")" << std::endl;
		if (type.HasMembers())
		{
			TextFormator::Padding(os, tab_lvl);
			std::cout << "{\n";
			tab_lvl++;
			for (auto member : type.Members())
			{
				os << member->Type();
			}
			tab_lvl--;
			TextFormator::Padding(os, tab_lvl);
			os << "}\n";
		}
		return os;
	}

	std::ostream& operator<<(std::ostream& os, const MetaType* type)
	{
		os << *type;
		return os;
	}

#if _DEBUG
	void MetaType::Print(int tab_lvl) const
	{
		TextFormator::TabLevel() = tab_lvl;
		std::cout << *this;
	}

#endif

	MetaType::~MetaType()
	{
		DealocateMembers();
	}

	void MetaType::Initialize(std::string name_, size_t size_)
	{
		m_name = name_;
		m_size = size_;
	}

	void MetaType::DealocateMembers()
	{
		for (auto member : Members())
		{
			delete member;
		}
	}

	////////////////////////////
	// MetaMember
	////////////////////////////
	std::string MetaMember::Name() const
	{
		return m_name;
	}

	size_t MetaMember::Offset() const
	{
		return m_offset;
	}

	const MetaType* MetaMember::Type() const
	{
		return m_type;
	}

}; // Framework
