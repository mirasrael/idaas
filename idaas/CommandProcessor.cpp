#include "CommandProcessor.h"
#include <enum.hpp>

CommandProcessor::CommandProcessor(void)
{
}


CommandProcessor::~CommandProcessor(void)
{
}

int CommandProcessor::HandleEnumerationsList(BinaryDataObjectBuilder &outputBuilder)
{
	char buffer[MAXNAMESIZE];

	unsigned __int32 enumsCount = get_enum_qty();
	outputBuilder.Write(enumsCount);
	for (unsigned __int32 i = 0; i < enumsCount; i++) {
		enum_t id = getn_enum(i);
		int nameLength = get_enum_name(id, buffer, sizeof(buffer));
		outputBuilder.Write(buffer, 0, nameLength + 1);
		outputBuilder.Write((unsigned __int8) is_bf(id));
		
	}
	return 0;
}

int CommandProcessor::Handle(DatabaseCommands command, BinaryDataObjectPtr &output) {
	int result = 0;
	
	BinaryDataObjectBuilder builder;
	builder.Write((unsigned __int32) command);
	switch (command) {
	case DatabaseCommands::FunctionsList:		
		builder.Write((unsigned __int32) 1);		
		break;
	case DatabaseCommands::EnumerationsList:
		result = HandleEnumerationsList(builder);
		break;
	default:
		return -1;
	}
	output = builder.Build();
	return result;
}
