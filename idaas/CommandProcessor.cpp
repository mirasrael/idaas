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
		outputBuilder.Write(unsigned __int32(id));
		outputBuilder.Write(buffer, 0, nameLength + 1);
		outputBuilder.Write((unsigned __int8) is_bf(id));

		unsigned __int32 membersCount = get_enum_size(id);
		outputBuilder.Write(membersCount);
		if (membersCount > 0) {
			bmask_t bitMask = get_first_bmask(id);
			do {
				uval_t value = get_first_enum_member(id, bitMask);
				while (value != uval_t(-1)) {
					uchar serial;
					const_t cid = get_first_serial_enum_member(id, value, &serial, bitMask);
					while (cid != const_t(-1)) {
						cid = get_next_serial_enum_member(cid, &serial);
						nameLength = get_enum_member_name(cid, buffer, sizeof(buffer));
						outputBuilder.Write(unsigned __int32(cid));
						outputBuilder.Write(buffer, 0, nameLength + 1);
						outputBuilder.Write(unsigned __int32(value));
						outputBuilder.Write(unsigned __int8(serial));
						outputBuilder.Write(unsigned __int32(bitMask));
					}
					value = get_next_enum_member(id, value, bitMask);
				}
				if (bitMask != bmask_t(-1)) {
					bitMask = get_next_bmask(id, bitMask);
				}
			} while (bitMask != bmask_t(-1));
		}		
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
