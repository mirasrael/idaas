#include "StructuresHandler.h"

#include "Logging.h"

#include <bytes.hpp>
#include <typeinf.hpp>
#include <enum.hpp>

#include <boost/algorithm/string/replace.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/ref.hpp>

const std::regex StructuresHandler::invalidIdentifier("\\b(?:\\w|\\$)+::(?:\\w|[:$])+");
const std::regex StructuresHandler::typeName("^(?:struct\\s+|union\\s+)?([A-Za-z0-9_:$]+)\\s*(?:$|\\[)");

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
	to.isUnion = from->is_union();

	size_t membersCount = from->memqty;
	to.members.reserve(membersCount * 2);
	to.members.resize(membersCount);
	ea_t prev_end = 0;
	for (size_t memberIdx = 0, targetIndex = 0; memberIdx < membersCount; memberIdx++, targetIndex++) {
		member_t *member = &from->members[memberIdx];
		if (member->soff > prev_end) {
			to.members.resize(to.members.size() + 1);
			ida_struct_member& targetMember = to.members[targetIndex];
			targetMember.name = std::string("__gap__") + boost::lexical_cast<std::string>(targetIndex);
			targetMember.type = std::string("byte[") + boost::lexical_cast<std::string>(member->soff - prev_end) + std::string("]");
			targetIndex++;
		}
		copy_member(member, to.members[targetIndex]);
		prev_end = member->eoff;
	}	
}

void struct_list_copier::copy_member(const member_t *from, ida_struct_member& to) {	
	get_member_name(from->id, buffer, sizeof(buffer));
	to.name = buffer;
	qtype type, fields;
	if (from->has_ti()) {
		get_member_tinfo(from, &type, &fields);
	} else {
		if (0 != (enumflag() & from->flag)) {
			opinfo_t opinfo;
			qstring name;
			retrieve_member_info(from, &opinfo);
			get_enum_name(opinfo.ec.tid, buffer, sizeof(buffer));
			qstrncat(buffer, ";", sizeof(buffer));			
			parse_decl(idati, buffer, &name, &type, &fields, 0);
			size_t width = get_enum_width(opinfo.ec.tid);
			width = width > 0 ? 1 << (width - 1) : sizeof(int);
			if (from->eoff - from->soff > width) {
				qtype elementType = type;
				build_array_type(&type, elementType.c_str(), (from->eoff - from->soff) / width);
			}
		} else {
			get_or_guess_member_tinfo(from, &type, &fields);
		}		
	}	
	print_type_to_one_line(buffer, sizeof(buffer), idati, type.c_str(), 0, 0, fields.c_str(), 0);
	to.type = buffer;
}

StructuresHandler::StructuresHandler(void)
{
}


StructuresHandler::~StructuresHandler(void)
{
}

bool StructuresHandler::store( const ida_struct &_struct )
{	
	bool firstMember = false;
	tid_t id = get_struc_id(_struct.name.c_str());
	struc_t *struc;
	if (BADADDR != id) {
		struc = get_struc(id);
		if (struc->is_union() != _struct.isUnion)
			return false;
		// Replace all members with dummy, first added member will replace it
		if (struc->memqty > 0) {			
			member_t* first = get_member(struc, 0);			
			if (0 == first) {
				add_struc_member(struc, "__Dummy__", 0, byteflag(), 0, 1);
				del_struc_members(struc, 1, BADADDR);
			} else {				
				del_struc_members(struc, struc->is_union() ? 1 : first->eoff, BADADDR);
				set_member_name(struc, 0, "__Dummy__");
				if (first->eoff > 1) {
					set_member_type(struc, 0, byteflag(), 0, 1);
				}
			}			
			firstMember = true;
		}		
	} else {
		id = add_struc(BADADDR, _struct.name.c_str(), _struct.isUnion);
		struc = get_struc(id);
	}
	if (BADADDR == id)
		return false;
	
	qstring name;
	qtype type;
	qtype fields;
			
	std::vector<std::pair<tid_t, std::string>> escapedTypes;
	std::vector<ea_t> gapMemberOffsets;
	gapMemberOffsets.reserve(_struct.members.size() / 2 + 1);
	//logmsg("%s\n", _struct.name.c_str());
	for (std::vector<ida_struct_member>::const_iterator it = _struct.members.begin(); it != _struct.members.end(); it++)
	{				
		if (firstMember) {
			set_member_name(struc, 0, it->name.c_str());
			firstMember = false;
		} else {
			if (struc->is_union())
			{
				add_struc_member(struc, it->name.c_str(), BADADDR, byteflag(), 0, 1);
			}
			else
			{
				add_struc_member(struc, it->name.c_str(), BADADDR, 0, 0, 0);
			}			
		}		

		member_t *member;		
		member = &struc->members[struc->memqty - 1];		
		std::string fullType(escapeTypes(it->type, escapedTypes));
		if (*(fullType.end() - 1) != ';') {
			fullType.append(";");
		}		
		if (!parse_decl(idati, fullType.c_str(), &name, &type, &fields, 0 /*PT_SIL*/)) {
			logmsg("Failed to parse declaration '%s' for %s.%s\n", fullType.c_str(), _struct.name.c_str(), it->name.c_str());
			return false;
		}		
		set_member_tinfo(idati, struc, member, 0, type.c_str(), fields.c_str(), 0);
		restoreTypes(escapedTypes);

		if (boost::starts_with(it->name, "__gap__")) {
			gapMemberOffsets.push_back(member->soff);
		}
	}
	if (gapMemberOffsets.size() > 0) {
		for (std::vector<ea_t>::reverse_iterator it = gapMemberOffsets.rbegin(); it != gapMemberOffsets.rend(); it++) {
			del_struc_member(struc, *it);
		}
	}
	return true;
}

void StructuresHandler::list( std::vector<ida_struct> &_return )
{
	struct_list_copier copier;
	copier.copy_to(_return);
}

void StructuresHandler::_delete( const std::string& name )
{
	del_struc(get_struc(get_struc_id(name.c_str())));
}

std::string StructuresHandler::escapeTypes( const std::string& decl, std::vector<std::pair<tid_t, std::string>>& escapedTypes ) {
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

void StructuresHandler::restoreTypes( std::vector<std::pair<tid_t, std::string>>& escapedTypes )
{
	if (!escapedTypes.empty()) {
		for (std::vector<std::pair<tid_t, std::string>>::iterator it = escapedTypes.begin(); it != escapedTypes.end(); it++) {
			set_struc_name(it->first, it->second.c_str());
		}
		escapedTypes.clear();
	}
}

bool StructuresHandler::storeAll( const std::vector<ida_struct> &structs )
{	
	index_t index;
	for ( std::vector<ida_struct>::const_iterator it = structs.begin(); it != structs.end(); it++ ) {
		index[it->name] = &(*it);
	}

	while (index.size() > 0) {
		storeWithDependencies(index, index.begin());
	}
	return true;
}

bool StructuresHandler::storeWithDependencies( index_t& index, index_t::iterator& who )
{
	const ida_struct *struc = who->second;
	index.erase(who);
	
	std::smatch match;
	index_t::iterator dep;
	for (std::vector<ida_struct_member>::const_iterator it = struc->members.begin(); it != struc->members.end(); it++) {		
		if (regex_search(it->type, match, typeName) && (dep = index.find(match[1].str())) != index.end()) {
			storeWithDependencies(index, dep);
		}
	}	
	return store(*struc);
}
