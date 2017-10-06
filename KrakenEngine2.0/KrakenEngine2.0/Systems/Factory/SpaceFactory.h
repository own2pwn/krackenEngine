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
			// Gets the SpaceFactory
		static SpaceFactory * Get();
			// initializes SpaceFactory
		void Initialize() override;
			// Updates Factory
		void Update(float dt = 0.0f) override;
			// destructor
		virtual ~SpaceFactory();
			// add a new space
		Space* AddSpace(std::string name);
			// how many spaces are there
		size_t Size() const;
			// gets all the spaces
		std::unordered_map<unsigned int, Space*>* GetSpaces();
			// get a specific space
		Space* GetSpace(unsigned int id);
			// get a specific space
		Space* operator[](unsigned int id);
	private:

		std::unordered_map<unsigned int, Space*> m_spaces;
		unsigned int m_uniqueFactoryID;
			// hiden constructor (forces to use Get())
		SpaceFactory();
	};

} // Framework
#endif
