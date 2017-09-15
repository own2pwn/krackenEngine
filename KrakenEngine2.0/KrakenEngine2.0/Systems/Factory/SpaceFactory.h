/******************************************************************************/
/*!
\file   SpaceFactory.h
\author Aleksey Perfilev
\par    Copyright 2017, Digipen Institute of Technology
\brief
Creates and manages spaces where Object Factorys stored
*/
/******************************************************************************/
#ifndef SPACE_FACTORY_H
#define SPACE_FACTORY_H

namespace Framework
{
	class SpaceFactory : public ISystem
	{
	public:
		static SpaceFactory * Get();

		virtual void Initialize();

		virtual void Update(float dt = 0.0f);

		// destructor
		virtual ~SpaceFactory();

		ObjectFactory* AddSpace(std::string name);
		
		unsigned int Size() const;
		std::unordered_map<unsigned int, ObjectFactory*> GetSpaces();
		ObjectFactory* GetSpace(unsigned int id);
	private:

		std::unordered_map<unsigned int, ObjectFactory*> m_spaces;
		unsigned int m_totalSpaces;

		SpaceFactory();
	};

} // Framework
#endif
