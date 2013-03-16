#include "StructureHandler.h"

#include "Logging.h"

#include <bytes.hpp>
#include <typeinf.hpp>

#include <boost/algorithm/string/replace.hpp>

const std::regex StructureHandler::invalidIdentifier("\\b(?:\\w|\\$)+::(?:\\w|[:$])+");

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
	get_struc_name(from->id, buffer, sizeof(buffer));
	to.name = buffer;

	size_t membersCount = from->memqty;
	to.members.resize(membersCount);
	for (size_t memberIdx = 0; memberIdx < membersCount; memberIdx++) {
		copy_member(&from->members[memberIdx], to.members[memberIdx]);
	}	
}

void struct_list_copier::copy_member(const member_t *from, ida_struct_member& to) {	
	get_member_name(from->id, buffer, sizeof(buffer));
	to.name = buffer;
	qtype type, fields;
	get_or_guess_member_tinfo(from, &type, &fields);
	print_type_to_one_line(buffer, sizeof(buffer), idati, type.c_str(), 0, 0, fields.c_str(), 0);
	to.type = buffer;
}

StructureHandler::StructureHandler(void)
{
}


StructureHandler::~StructureHandler(void)
{
}

bool StructureHandler::store( const ida_struct &_struct )
{	
	bool firstMember = false;
	tid_t id = get_struc_id(_struct.name.c_str());
	struc_t *struc;
	if (BADADDR != id) {
		struc = get_struc(id);
		// Replace all members with dummy, first added member will replace it
		if (struc->memqty > 0) {
			member_t* first = &struc->members[0];
			if (first->soff > 0) {
				add_struc_member(struc, "__Dummy__", 0, byteflag(), 0, 1);
				del_struc_members(struc, 1, BADADDR);
			} else {
				set_member_name(struc, 0, "__Dummy__");
				del_struc_members(struc, first->eoff, BADADDR);
			}
			firstMember = true;
		}		
	} else {
		id = add_struc(BADADDR, _struct.name.c_str());
		struc = get_struc(id);
	}
	if (BADADDR == id)
		return false;
	
	qstring name;
	qtype type;
	qtype fields;
			
	std::vector<std::pair<tid_t, std::string>> escapedTypes;
	for (std::vector<ida_struct_member>::const_iterator it = _struct.members.begin(); it != _struct.members.end(); it++)
	{		
		if (firstMember) {
			set_member_name(struc, 0, it->name.c_str());
			firstMember = false;
		} else {
			add_struc_member(struc, it->name.c_str(), BADADDR, 0, 0, 0);
		}
		member_t *member = get_member(struc, get_struc_last_offset(struc));		

		std::string fullType(escapeTypes(it->type, escapedTypes));
		if (*(fullType.end() - 1) != ';') {
			fullType.append(";");
		}					
		if (!parse_decl(idati, fullType.c_str(), &name, &type, &fields, PT_SIL)) {
			logmsg("decl: %s.%s %s\n", _struct.name.c_str(), it->name.c_str(), fullType.c_str());
			return false;
		}		
		set_member_tinfo(idati, struc, member, 0, type.c_str(), fields.c_str(), 0);
		restoreTypes(escapedTypes);		
	}	
	return true;
}

void StructureHandler::list( std::vector<ida_struct> &_return )
{
	struct_list_copier copier;
	copier.copy_to(_return);
}

void StructureHandler::_delete( const std::string& name )
{
	del_struc(get_struc(get_struc_id(name.c_str())));
}

std::string StructureHandler::escapeTypes( const std::string& decl, std::vector<std::pair<tid_t, std::string>>& escapedTypes ) {
	std::smatch match;
	std::string s(decl);
	std::string output;
	output.reserve(s.size());
	while (std::regex_search(s, match, invalidIdentifier)) {
		std::string escapedName(match.str());
		boost::replace_all(escapedName, "::", "__");

		tid_t sid = get_struc_id(match.str().c_str());
		set_struc_name(sid, escapedName.c_str());

		output.append(match.suffix());
		output.append(escapedName);

		escapedTypes.push_back(std::pair<tid_t, std::string>(sid, match.str()));

		s = match.suffix();
	}
	output.append(s);	
	return output;
}

void StructureHandler::restoreTypes( std::vector<std::pair<tid_t, std::string>>& escapedTypes )
{
	if (!escapedTypes.empty()) {
		for (std::vector<std::pair<tid_t, std::string>>::iterator it = escapedTypes.begin(); it != escapedTypes.end(); it++) {
			set_struc_name(it->first, it->second.c_str());
		}
		escapedTypes.clear();
	}
}
