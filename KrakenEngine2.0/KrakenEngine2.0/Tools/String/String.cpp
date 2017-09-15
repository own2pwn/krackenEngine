/******************************************************************************/
/*!
\file   String.cpp
\author Aleksey Perfilev
\par    Copyright 2017, Digipen Institute of Technology
\brief
hashing strings to uniq ID
*/
/******************************************************************************/

#include "../../Precompiled.h"

namespace Framework
{
	int StringCounter::counter = 1;
	std::unordered_map<std::string, int> String::m_strToInt;

	String::String() : m_str(""), m_id(0), m_size(0) {}

	String::String(const std::string& text)
	{
		*this = text;
		m_size = text.size();
	}

	String::String(const String& string)
	{
		m_str = string.m_str;
		m_id = string.m_id;
		m_size = string.Size();
	}

	int String::ToInt() const
	{
		return m_strToInt.at(m_str);
	}

	inline std::string String::Get() const
	{
		return m_str;
	}

	String& String::operator=(const String& lhs)
	{
		*this = lhs.m_str;
		m_size = lhs.m_size;
		return *this;
	}

	String& String::operator=(const std::string& lhs)
	{
		m_str = lhs;
		std::unordered_map<std::string, int>::const_iterator
			it = m_strToInt.find(m_str);

		if (it == m_strToInt.end())
		{
			StringCounter::counter++;
			m_id = StringCounter::counter;
			m_strToInt[m_str] = m_id;
		}
		else
		{
			m_id = m_strToInt.at(m_str);
		}

		m_size = lhs.size();
		
		return *this;
	}

	bool String::operator==(const String& lhs) const
	{
		if (ToInt() == lhs.ToInt())
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool String::operator==(const std::string& lhs) const
	{
		String lhsStr(lhs);
		return *this == lhsStr;
	}

	String String::operator+(const String& lhs)
	{
		return String(this->m_str + lhs.m_str);
	}
	
	String String::operator+(const std::string& lhs)
	{
		return String(this->m_str + lhs);
	}
	
	const String& String::operator+=(const String& lhs)
	{
		*this = *this + lhs;
		return *this;
	}
	
	const String& String::operator+=(const std::string& lhs)
	{
		*this = this->m_str + lhs;
		return *this;
	}

	inline unsigned String::Size() const
	{
		return m_size;
	}

	bool String::IsEmpty() const
	{
			// is the string is not empty
		if (m_size)
		{
			return false;
		}
		else // string is empty
		{
			return true;
		}
	}

	std::wstring String::ToWide()
	{
		std::wstring w_str;
		for (unsigned i = 0; i < m_size; i++)
		{
			w_str += m_str[i];
		}
		return w_str;
	}

	String::operator std::string const() const 
	{
		return m_str;
	}

	std::ostream& operator<<(std::ostream& os, const String& str)
	{
		return os << str.Get();
	}
}; // Framework
