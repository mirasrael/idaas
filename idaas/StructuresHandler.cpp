#include "StructuresHandler.h"

#include "Logging.h"

#include <bytes.hpp>
#include <typeinf.hpp>
#include <enum.hpp>

#include <boost/algorithm/string/replace.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/ref.hpp>

const std::regex StructuresHandler::invalidIdentifier("\\b(?:\\w|\\$)+::(?:\\w|[:$])+");
const std::regex StructuresHandler::typeName("^(?:struct\\s+|union\\s+)?([A-Za-z0-9_:$]+)\\s*(?:$|\\[)");

StructuresHandler::StructuresHandler(void)
{
}


StructuresHandler::~StructuresHandler(void)
{
}

bool StructuresHandler::store( const IdaStruct &_struct )
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
	for (std::vector<IdaStructMember>::const_iterator it = _struct.members.begin(); it != _struct.members.end(); it++)
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

void StructuresHandler::list( std::vector<IdaStruct> &_return )
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

bool StructuresHandler::storeAll( const std::vector<IdaStruct> &structs )
{	
	index_t index;
	for ( std::vector<IdaStruct>::const_iterator it = structs.begin(); it != structs.end(); it++ ) {
		index[it->name] = &(*it);
		// create empty structures initially because they can be used in pointers
		if (BADADDR == get_struc_id(it->name.c_str())) {
			tid_t id = add_struc(BADADDR, it->name.c_str(), it->isUnion);
			add_struc_member(get_struc(id), "__Dummy__", 0, byteflag(), 0, 1);
		}
	}

	while (index.size() > 0) {
		storeWithDependencies(index, index.begin());
	}
	return true;
}

bool StructuresHandler::storeWithDependencies( index_t& index, index_t::iterator& who )
{
	const IdaStruct *struc = who->second;
	index.erase(who);
	
	std::smatch match;
	index_t::iterator dep;
	for (std::vector<IdaStructMember>::const_iterator it = struc->members.begin(); it != struc->members.end(); it++) {		
		if (regex_search(it->type, match, typeName) && (dep = index.find(match[1].str())) != index.end()) {
			storeWithDependencies(index, dep);
		}
	}	
	return store(*struc);
}
