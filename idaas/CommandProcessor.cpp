#include "CommandProcessor.h"
#include <BinaryDataObjectBuilder.h>


CommandProcessor::CommandProcessor(void)
{
}


CommandProcessor::~CommandProcessor(void)
{
}

int CommandProcessor::Handle(DatabaseCommands command, BinaryDataObjectPtr &output) {
	BinaryDataObjectBuilder builder;
	builder.Write((unsigned __int32) command);
	switch (command) {
	case DatabaseCommands::FunctionsList:		
		builder.Write((unsigned __int32) 1);		
		break;
	default:
		return -1;
	}
	output = builder.Build();
	return 0;
}
