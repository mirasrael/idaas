#pragma once

#include <BinaryDataObject.h>

#include "DatabaseCommands.h"

class CommandProcessor
{
public:
	CommandProcessor(void);
	~CommandProcessor(void);
	int Handle(DatabaseCommands command, BinaryDataObjectPtr &output);
};

