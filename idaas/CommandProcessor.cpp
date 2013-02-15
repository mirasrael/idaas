#include "CommandProcessor.h"
#include <enum.hpp>

CommandProcessor::CommandProcessor(void)
{
}


CommandProcessor::~CommandProcessor(void)
{
}

class enumeration_writer : public enum_member_visitor_t {
private:
	char buffer[MAXNAMESIZE];
	BinaryDataObjectBuilder* m_builder;
public:
	enumeration_writer(BinaryDataObjectBuilder* builder) : m_builder(builder) {};

	void write(enum_t id);
	virtual int idaapi visit_enum_member(const_t cid, uval_t value);
};

int idaapi enumeration_writer::visit_enum_member(const_t cid, uval_t value) {
	int nameLength = get_enum_member_name(cid, buffer, sizeof(buffer));
	m_builder->Write(unsigned __int32(cid));
	m_builder->Write(buffer, 0, nameLength + 1);
	m_builder->Write(unsigned __int32(value));
	m_builder->Write(unsigned __int8(get_enum_member_serial(cid)));
	m_builder->Write(unsigned __int32(get_enum_member_bmask(cid)));
	return 0;
}

void enumeration_writer::write(enum_t id) {
	int nameLength = get_enum_name(id, buffer, sizeof(buffer));
	m_builder->Write(unsigned __int32(id));
	m_builder->Write(buffer, 0, nameLength + 1);
	m_builder->Write(unsigned __int8(is_bf(id)));
	m_builder->Write(unsigned __int32(get_enum_size(id)));
	for_all_enum_members(id, *this);
}

int CommandProcessor::HandleEnumerationsList(BinaryDataObjectBuilder &outputBuilder)
{	
	unsigned __int32 enumsCount = get_enum_qty();
	outputBuilder.Write(enumsCount);
	enumeration_writer writer(&outputBuilder);
	for (unsigned __int32 i = 0; i < enumsCount; i++) {		
		writer.write(getn_enum(i));
	}
	return 0;
}

struct delete_enum_members : public enum_member_visitor_t {
	virtual int idaapi visit_enum_member(const_t cid, uval_t value) {
		del_enum_member(get_enum_member_enum(cid), value, get_enum_member_serial(cid), get_enum_member_bmask(cid));
		return 0;
	}
};

int CommandProcessor::HandleDeleteEnumeration(BinaryDataObjectReader& input, BinaryDataObjectBuilder &output)
{
	enum_t id = input.Read<enum_t>();
	del_enum(id);
	return 0;
}

int CommandProcessor::HandleUpdateEnumeration(BinaryDataObjectReader& input, BinaryDataObjectBuilder &output)
{		
	enum_t id = input.Read<enum_t>();
	std::string &name = input.ReadString();
	bool isBitfield = input.ReadBoolean();	
	set_enum_name(id, name.c_str());	
	set_enum_bf(id, isBitfield);
	for_all_enum_members(id, delete_enum_members());

	int membersCount = input.ReadUInt32();	
	for (int memberIndex = 0; memberIndex < membersCount; memberIndex++) {
		std::string &memberName = input.ReadString();
		unsigned __int32 value = input.ReadUInt32();	
		unsigned __int32 bitMask = input.ReadUInt32();		
		add_enum_member(id, memberName.c_str(), value, bitMask);		
	}
	return 0;
}

int CommandProcessor::HandleCreateEnumeration(BinaryDataObjectReader& input, BinaryDataObjectBuilder &output)
{		
	std::string &name = input.ReadString();
	bool isBitfield = input.ReadBoolean();
	enum_t id = add_enum(BADADDR, name.c_str(), 0);
	set_enum_bf(id, isBitfield);

	int membersCount = input.ReadUInt32();
	for (int memberIndex = 0; memberIndex < membersCount; memberIndex++) {
		std::string &memberName = input.ReadString();
		unsigned __int32 value = input.ReadUInt32();		
		unsigned __int32 bitMask = input.ReadUInt32();
		add_enum_member(id, memberName.c_str(), value, bitMask);		
	}
	return 0;
}

int CommandProcessor::Handle(BinaryDataObjectPtr& input, BinaryDataObjectPtr &output) {
	int result = 0;
	
	BinaryDataObjectBuilder builder;
	BinaryDataObjectReader reader(input);

	DatabaseCommands command = reader.Read<DatabaseCommands>();
	builder.Write((unsigned __int32) command);
	switch (command) {
	case DatabaseCommands::FunctionsList:		
		builder.Write((unsigned __int32) 1);		
		break;
	case DatabaseCommands::EnumerationsList:
		result = HandleEnumerationsList(builder);
		break;
	case DatabaseCommands::CreateEnumeration:
		result = HandleCreateEnumeration(reader, builder);
		break;
	case DatabaseCommands::UpdateEnumeration:
		result = HandleUpdateEnumeration(reader, builder);
		break;
	case DatabaseCommands::DeleteEnumeration:
		result = HandleDeleteEnumeration(reader, builder);
		break;
	default:
		return -1;
	}
	output = builder.Build();
	return result;
}
