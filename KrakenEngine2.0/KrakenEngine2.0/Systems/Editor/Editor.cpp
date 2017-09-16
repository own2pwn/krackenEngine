#include "Editor.h"

editor::editor() : activated_dgb_(false)
{
}

void editor::Initialize()
{
	// Initialize the command line for use in release as well as dbg

	// Initialize the rest in dbg only
#if _DEBUG

#endif

}

void editor::Update(float dt)
{
	// check if dbg is active
	if (!activated_dgb_)
		return;
	// Update all active modules in the editor
	for (int i = 0; i < editor_modules_.size(); ++i)
		if(editor_modules_[i].is_active())
			editor_modules_[i].update();
}

