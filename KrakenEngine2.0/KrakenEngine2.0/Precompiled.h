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

///////////////////////// our files //////////////////
/////// care of the order they are included //////////

// tools
#include "Tools/NameGenerator/NameGenerator.h"
#include "Tools/Assert/Assert.h"
#include "Tools/uniqueID/UniqueID.h"
#include "Tools/String/String.h"
#include "Tools/TextFormator/TextFormator.h"

// base engine
#include "Systems/Interface/ISystem.h"
#include "BaseEngine/Reflection/MetaUserInterface.h"
#include "BaseEngine/Reflection/RemoveQualifier.h"
#include "BaseEngine/Reflection/MetaType.h"
#include "BaseEngine/Reflection/MetaFactory.h"
#include "BaseEngine/Reflection/MetaVariable.h"
#include "BaseEngine/Serialization/Serialization.h"
#include "BaseEngine/Reflection/MetaUserInterface.h"
#include "BaseEngine/Core/Core.h"

// systems
#include "Systems/ScriptingSystem/ComponentID.h"
#include "Systems/ScriptingSystem/Component.h"
#include "Systems/Factory/GameObject.h"
#include "Systems/Factory/Space.h"
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
#include "Tools/CodeTests/StringTest.h"
#endif

#endif