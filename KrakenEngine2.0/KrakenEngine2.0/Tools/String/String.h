/******************************************************************************/
/*!
\file   String.h
\author Aleksey Perfilev
\par    Copyright 2017, Digipen Institute of Technology
\brief
hashing strings to uniq ID
*/
/******************************************************************************/
#ifndef STRING_H
#define STRING_H

namespace Framework
{
	class StringCounter;
	class String
	{
	public:
			// constructor
		String();
			// conversion constructor
		String(const std::string& text);
			// copy constructor
		String(const String& string);
			// returns string itself
		inline std::string Get() const;
			// assignment operator
		String& operator=(const String& lhs);
			// assignment operator
		String& operator=(const std::string& lhs);
			// comparison operator
		bool operator==(const String& lhs) const;
			// comparison operator
		bool operator==(const std::string& lhs) const;
			// appends one string to anohter
		String operator+(const String& lhs);
			// appends one string to anohter
		String operator+(const std::string& lhs);
			// appends one string to anohter
		const String& operator+=(const String& lhs);
			// appends one string to anohter
		const String& operator+=(const std::string& lhs);
			// returns length of the string(how many chars)
		inline size_t Size() const;
			// returns true if the string is empty
		bool IsEmpty() const;
			// converts string to wide string
		std::wstring ToWide();
			// conversion operator
		operator std::string const() const;

	private:
			// the actual string
		std::string m_str;
			// map that hashes all the strings in the engine to uniq IDs
		static std::unordered_map<std::string, int> m_strToInt;
			// id of the string in the map
		int m_id;
			// stores sizer of string(how many chars) to avoid every frame calculation
		size_t m_size;
			// gets int representation of string
		int ToInt() const;
		friend std::ostream& operator<<(std::ostream& os, const String& str);
	};

	class StringCounter
	{
		friend String;
		static int counter;
	};
}; // Framework

#endif