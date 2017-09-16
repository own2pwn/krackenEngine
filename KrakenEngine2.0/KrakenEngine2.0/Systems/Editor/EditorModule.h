

#pragma once
#ifndef EDITORMODULE_H
#define EDITORMODULE_H
class EditorModule
{
public:

	//Require all Editor Modules to have an update

	virtual void update() = 0;
	//

	//Start all modules deactivated 
	EditorModule() :module_active_(false){}

	// Virtual destrctor
	virtual ~EditorModule(){}

	// Toggle the module active or not
	virtual void togglemodule()
	{
		module_active_ = !module_active_;
	}

	virtual const bool& is_active() const
	{
		return module_active_;
	}

private:
	bool module_active_;
};

#endif
