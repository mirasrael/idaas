#pragma once

#include <BinaryDataObject.h>
#include <BinaryDataObjectBuilder.h>
#include <BinaryDataObjectReader.h>

#include "DatabaseCommands.h"
#include "Logging.h"

class CommandProcessor
{
private:
	int HandleEnumerationsList(BinaryDataObjectBuilder &outputBuilder);
	int HandleCreateEnumeration(BinaryDataObjectReader &input, BinaryDataObjectBuilder &output);
	int HandleUpdateEnumeration(BinaryDataObjectReader &input, BinaryDataObjectBuilder &output);
	int HandleDeleteEnumeration(BinaryDataObjectReader& input, BinaryDataObjectBuilder &output);
public:
	CommandProcessor(void);
	~CommandProcessor(void);
	int Handle(BinaryDataObjectPtr& input, BinaryDataObjectPtr &output);	
};

