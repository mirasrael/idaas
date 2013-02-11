#pragma once

#include <BinaryDataObject.h>
#include <BinaryDataObjectBuilder.h>

#include "DatabaseCommands.h"

class CommandProcessor
{
private:
	int HandleEnumerationsList(BinaryDataObjectBuilder &outputBuilder);
public:
	CommandProcessor(void);
	~CommandProcessor(void);
	int Handle(DatabaseCommands command, BinaryDataObjectPtr &output);	
};

