#pragma once

#ifndef COMMAND_LINE_H
#define COMMAND_LINE_H

#include "EditorModule.h"

#define SHARED_MEM_BUFFER_SIZE 128

class Command_Line : public EditorModule
{
public:

	virtual void update() override;

	Command_Line();
	~Command_Line();

	// Creating the command line
	void CreateChildProcess();
	void DestoryChildProcess();


private:
	char* command_buffer_[SHARED_MEM_BUFFER_SIZE];
};

#endif

