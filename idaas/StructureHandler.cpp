#include "StructureHandler.h"
#include <struct.hpp>
#include <typeinf.hpp>

class struct_list_copier {
private:
	char buffer[4096];	
	std::vector<ida_struct_member>::iterator membersIt;
	void copy_struc(const struc_t *from, ida_struct& to);
	void copy_member(const member_t *from, ida_struct_member& to);
public:	
	void copy_to(std::vector<ida_struct>& _result);	
};

void struct_list_copier::copy_to(std::vector<ida_struct>&  _result) {
	size_t count = get_struc_qty();
	_result.resize(count);
	for (size_t idx = 0; idx < count; idx++) {
		tid_t id = get_struc_by_idx(idx);
		struc_t *s = get_struc(id);
		copy_struc(s, _result.at(idx));
	}
}

void struct_list_copier::copy_struc(const struc_t *from, ida_struct& to) {
	to.id = from->id;
	get_struc_name(from->id, buffer, sizeof(buffer));
	to.name = buffer;

	size_t membersCount = from->memqty;
	to.members.resize(membersCount);
	for (size_t memberIdx = 0; memberIdx < membersCount; memberIdx++) {
		copy_member(&from->members[memberIdx], to.members[memberIdx]);
	}	
}

void struct_list_copier::copy_member(const member_t *from, ida_struct_member& to) {
	to.id = from->id;	
	get_member_name(from->id, buffer, sizeof(buffer));
	to.name = buffer;
	qtype type, fields;
	get_member_tinfo(from, &type, &fields);
	print_type_to_one_line(buffer, sizeof(buffer), idati, type.c_str(), 0, 0, fields.c_str(), 0);
	to.type = buffer;
}

StructureHandler::StructureHandler(void)
{
}


StructureHandler::~StructureHandler(void)
{
}

int32_t StructureHandler::store( const ida_struct &_struct )
{
	tid_t id = _struct.id;
	struc_t *struc;
	if (id != -1) {
		struc = get_struc(id);
		set_struc_name(id, _struct.name.c_str());
		del_struc_members(struc, 0, get_struc_size(struc));
	} else {
		id = add_struc(BADADDR, _struct.name.c_str());
		struc = get_struc(id);
	}	

	qstring name;
	qtype type;
	qtype fields;

	for (std::vector<ida_struct_member>::const_iterator it = _struct.members.begin(); it != _struct.members.end(); it++)
	{		
		add_struc_member(struc, it->name.c_str(), BADADDR, 0, 0, 0);
		member_t *member = get_member(struc, get_struc_last_offset(struc));

		std::string fullType(it->type);
		if (*(fullType.end() - 1) != ';') {
			fullType.append(";");
		}
		parse_decl(idati, fullType.c_str(), &name, &type, &fields, 0);
		set_member_tinfo(idati, struc, member, 0, type.c_str(), fields.c_str(), 0);
	}	
	return id;
}

void StructureHandler::list( std::vector<ida_struct> &_return )
{
	struct_list_copier copier;
	copier.copy_to(_return);
}

void StructureHandler::_delete( const int32_t id )
{
	del_struc(get_struc(id));
}