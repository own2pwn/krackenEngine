/******************************************************************************/
/*!
\file   MetaVariable.h
\author Aleksey Perfilev
\par    Copyright 2017, Digipen Institute of Technology
\brief
Stores information about variable, used by serialization
*/
/******************************************************************************/
#ifndef METAVARIABLE_H
#define METAVARIABLE_H

namespace Framework
{
	/***********************************************
	*	generic variable stuff (MetaVariable)
	***********************************************/
	class MetaVariable
	{
	public:
			// constructors
		MetaVariable();
		MetaVariable(std::string name, void* data, const MetaType* type);
			
			// sets MetaVariable
		const MetaVariable& Set(MetaType* type, void* data);
			
			// Sets MetaVariable using template.
			// The actual use of template is hiden here(compiler figures out template parameter from the passing variable). 
			// Use example: metaVariable.Set(actualVariable)
			// returns itself so it could be chained into std::cout or something else. Example std::cout << metaVar.Set(regularVar);
		template <typename T>
		const MetaVariable& Set(T& data, std::string name);

			// gets the data of the MetaVariable using template.
		template <typename T>
		T& Data();
			// gets the data of the MetaVariable using template(const version).
		template <typename T>
		T& Data() const;

			// gets Name of he variable
		std::string Name() const;
			// gets the MetaType of variable
		const MetaType* Type() const;
			// output operator
		friend std::ostream& operator<<(std::ostream& os, const MetaVariable& metaVariable);
			// input operator
		friend std::istream& operator>>(std::istream& is, MetaVariable& metaVariable);

			// Serialize function
		std::ostream& Serialize(std::ostream& os) const;
		// Deserialize function
		std::istream& Deserialize(std::istream& is);
			// gets pointer to the data
		void* GetRowData() const;

#if _DEBUG
			// prints variable(developed at first write, consider deleting this)
		void Print() const;
#endif
	private:
		const MetaType* m_type;	// Type of the variable
		void* m_data;			// poiter to the actual data
		std::string m_name;		// name of the variable
	};

	///////////////////////////////////////////////////////////////
	// Implementation of templated function from the class above //
	///////////////////////////////////////////////////////////////
	template <typename T>
	const MetaVariable& MetaVariable::Set(T& data, std::string name)
	{
		m_type = GET_TYPE_BY_TEMPLATE(T);
		m_name = name;
		m_data = &data;
		return *this;
	}

	template <typename T>
	T& MetaVariable::Data()
	{
		MetaType* type = GET_TYPE_BY_TEMPLATE(T);
		ASSERT(type == m_type);
		return (*reinterpret_cast<T*>(m_data));
	}

	template <typename T>
	T& MetaVariable::Data() const
	{
		MetaType* type = GET_TYPE_BY_TEMPLATE(T);
		ASSERT(type == m_type);
		return (*reinterpret_cast<T*>(m_data));
	}

	/*
	class MetaVarFactory
	{
		template<typename T>
		void registervar(T& var)
		{
			MetaVariable v;
			v.Set(var);
			all_vars.push_back(v);
		}

		template<typename T, typename V>
		void registermember(T& var, V& mem)
		{
			MetaVariable* owner = all_wars[&var];
			MetaVariable* member = all_wars[&var];
		}

		std::vector<void*> all_vars;
	};


*/

}; // Framework


#endif
