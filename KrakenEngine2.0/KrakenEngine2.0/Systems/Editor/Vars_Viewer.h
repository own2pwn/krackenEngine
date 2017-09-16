

#pragma once
#ifndef VARS_VIEWER_H
#define VARS_VIEWER_H

#include "EditorModule.h"

class Vars_Viewer : public EditorModule
{
public:
	virtual void update() override;
	Vars_Viewer();
	~Vars_Viewer();

private:


};


#endif
