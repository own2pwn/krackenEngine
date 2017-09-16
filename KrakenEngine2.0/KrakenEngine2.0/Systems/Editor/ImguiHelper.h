

#pragma once

#ifndef IMGUIHELPER_H
#define IMGUIHELPER_H


// namespace for helper functions for using ImGui
namespace imgui_helper
{
	// Creates a MenuItem for current menu, from a meta variable
	template <typename T>
	void CREATE_MENU_ITEM(Framework::MetaVariable menuvar)
	{
		ImGui::MenuItem(menuvar.Name(), std::to_string(menuvar.Data<T>()));
	}
}

#endif
