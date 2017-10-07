/******************************************************************************/
/*!
\file   MetaType.h
\author Aleksey Perfilev
\par    Copyright 2017, Digipen Institute of Technology
\brief
Stores information about types
*/
/******************************************************************************/
#ifndef META_TYPE_H
#define META_TYPE_H

namespace Framework
{
	// forward declarations
	template <typename T>
	class MetaFactory;
	class MetaMember;
	class MetaVariable;

		// serialize/deserialize function signitures
	typedef void(*SerializeFn)(std::ostream& os, const MetaVariable& metaVariable);
	typedef void(*DeserializeFn)(std::istream& os, MetaVariable& metaVariable);

	/***********************************************
	*	MetaType - generic type 
	***********************************************/
	class MetaType
	{
		template <typename T>
		friend class MetaFactory;
	
	public:
			// Gets the name of the type
		const std::string& Name() const;
			// gets the size of the type
		size_t Size() const;
			// gets the vector of all the members in the type
		const std::vector<MetaMember const *>& Members() const;

			// checks if there are any members in this type
		bool HasMembers() const;
			// adds new member to the type
		void AddMember(MetaMember const * member);
			
			// sets serialize function 
		void SetSerialize(SerializeFn fn);
			// checks if serialize function was ever set
		bool IsSerializible() const;

			// sets deserialize function 
		void SetDeserialize(DeserializeFn fn);
			// checks if deserialize function was ever set
		bool IsDeserializible() const;
		
			// Serializes variable passed from Serialize function from MetaVariable
		std::ostream& Serialize(std::ostream& os, const MetaVariable& metaVar) const;
			// Desializes from input
		std::istream& Deserialize(std::istream& is, MetaVariable& metaVar) const;
			// out operators
		friend std::ostream& operator<<(std::ostream& os, const MetaType& type);
		friend std::ostream& operator<<(std::ostream& os, const MetaType* type);
#if _DEBUG
			// out operator with a bit more control for debugging
		void Print(int tab_lvl = 0) const;
#endif

		~MetaType();

	private:
		std::string m_name;							// name of the type
		size_t m_size;								// sizeof type
		std::vector<MetaMember const *> m_members;	// members of the type
		SerializeFn m_serialize;					// serialize function
		DeserializeFn m_deserialize;

			// constructor hiden from user, forces to use MetaFactory(user should be calling macros from MetaUserInterface.h)
		MetaType() : m_size(0), m_serialize(nullptr), m_deserialize(nullptr) {}
			// initialization hiden from user, forces to use MetaFactory(user should be calling macros from MetaUserInterface.h)
		void Initialize(std::string name, size_t size);
			// Dealocates members (called upen destruction)
		void DealocateMembers();
	};

	/***********************************************
	 *	MetaMember - generic member of an object
	 ***********************************************/
	class MetaMember
	{
	public:
		MetaMember() : m_offset(0), m_type(nullptr) {}
		MetaMember(std::string name, size_t offset, MetaType* type) : m_name(name), m_offset(offset), m_type(type)/*, m_next(nullptr)*/ {}
			
			// Gets the name of the member
		std::string Name() const;
			// gets the offset of the member in the class/struct
		size_t Offset() const;
			// gets the Type of the member
		const MetaType* Type() const;

	private:
		std::string m_name;		// name of the member
		size_t m_offset;		// offset of the member in the class/struct
		const MetaType* m_type;	// type of the member
	};
}; // Framework

#endif
