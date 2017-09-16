/******************************************************************************/
/*!
\file   Precompiled.h
\author Aleksey Perfilev
\par    Copyright 2017, Digipen Institute of Technology
\brief
All the includes are listed here to avoid multiple includes in any other files
*/
/******************************************************************************/
#ifndef PRECOMPILED_H
#define PRECOMPILED_H

///////////////////////// standart includes //////////
#include <unordered_map>
#include <vector>
#include <fstream>	// for translator and debug
#include <iostream>
#include <direct.h> // mkdir in serialization
#include <ctime>	// used by FPS in Core
#include <chrono>	// used for FPS

///////////////////////// other files ////////////////
#include "Tools/jsoncpp/json/json.h"

///////////////////////// our files //////////////////
/////// care of the order they are included //////////

// tools
#include "Tools/NameGenerator/NameGenerator.h"
#include "Tools/Assert/Assert.h"
#include "Tools/String/String.h"
#include "Tools/TextFormator/TextFormator.h"

// base engine
#include "Systems/ISystem.h"
#include "BaseEngine/BaseSystems/Reflection/MetaUserInterface.h"
#include "BaseEngine/BaseSystems/Reflection/RemoveQualifier.h"
#include "BaseEngine/BaseSystems/Reflection/MetaType.h"
#include "BaseEngine/BaseSystems/Reflection/MetaFactory.h"
#include "BaseEngine/BaseSystems/Reflection/MetaVariable.h"
#include "BaseEngine/BaseSystems/Serialization/Serialization.h"
#include "BaseEngine/BaseSystems/Reflection/MetaUserInterface.h"
#include "BaseEngine/Entity.h"
#include "BaseEngine/Core/Core.h"

// systems
#include "Systems/Factory/ObjectFactory.h"
#include "Systems/Factory/SpaceFactory.h"



// Editor
#include "Systems/Editor/EditorModule.h"
#include "Systems/Editor/ImguiHelper.h"
#include "Systems/Editor/Editor.h"
#include "Systems/Editor/Command_Line.h"
#if _DEBUG
#include "Systems/Editor/Vars_Viewer.h"
#endif


//////////////////////// unit tests //////////////////
#if _DEBUG
#include "Tools/CodeTests/StringTest/StringTest.h"
#endif

#endif