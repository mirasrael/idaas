#include "EnumerationsHandler.h"

#include "Logging.h"

#include <boost/ref.hpp>

class enumeration_list_copier : public enum_member_visitor_t {
private:
	char buffer[MAXNAMESIZE];
	ida_enum *current;
	std::vector<ida_enum_const>::iterator constantsIt;
public:	
	void copy_to(std::vector<ida_enum>& _result);
	virtual int idaapi visit_enum_member(const_t cid, uval_t value);
};

int idaapi enumeration_list_copier::visit_enum_member(const_t cid, uval_t value) {		
	get_enum_member_name(cid, buffer, sizeof(buffer));
	constantsIt->name = buffer;
	constantsIt->value = value;	
	constantsIt->mask = get_enum_member_bmask(cid);	
	constantsIt++;
	return 0;
}

void enumeration_list_copier::copy_to(std::vector<ida_enum>& _result) {
	size_t count = get_enum_qty();
	_result.resize(count);
	for (size_t i = 0; i < count; i++) {
		current = &_result.at(i);
		enum_t id = getn_enum(i);
		get_enum_name(id, buffer, sizeof(buffer));		
		current->name = buffer;
		current->isBitfield = is_bf(id);
		current->constants.resize(get_enum_size(id));
		constantsIt = current->constants.begin();
		for_all_enum_members(id, *this);
	}	
}

EnumerationsHandler::EnumerationsHandler(void)
{
}


EnumerationsHandler::~EnumerationsHandler(void)
{
}

bool EnumerationsHandler::storeAll( const std::vector<ida_enum> &enums )
{	
	for ( std::vector<ida_enum>::const_iterator it = enums.begin(); it != enums.end(); it++ )
	{
		if (!store(*it)) {
			return false;
		}
	}
	return true;
}

bool EnumerationsHandler::store( const ida_enum &_enum )
{
	enum_t id = get_enum(_enum.name.c_str());
	if (BADADDR != id) {		
		deleteEnumMembers(id);
	} else {
		id = add_enum(BADADDR, _enum.name.c_str(), 0);
		if (id == BADADDR)
			return false;
	}	
	set_enum_bf(id, _enum.isBitfield);		
	for (std::vector<ida_enum_const>::const_iterator it = _enum.constants.begin(); it != _enum.constants.end(); it++) {
		if (0 != add_enum_member(id, it->name.c_str(), it->value, it->mask)) {
			logmsg("******* Failed to add: %s %d:%d\n", it->name.c_str(), it->value, it->mask);
			return false;
		}
	}
	return true;
}

void EnumerationsHandler::list( std::vector<ida_enum> &_return )
{
	enumeration_list_copier copier;
	copier.copy_to(_return);
}

void EnumerationsHandler::_delete( const std::string& name )
{
	del_enum(get_enum(name.c_str()));
}

struct collect_enum_members : public enum_member_visitor_t {
private:
	boost::reference_wrapper<std::vector<std::pair<const_t, uval_t>>> constants;

public:
	collect_enum_members( std::vector<std::pair<const_t, uval_t>> &constants ) : constants(constants) {}

	virtual int idaapi visit_enum_member(const_t cid, uval_t value) {
		constants.get().push_back(std::pair<const_t, uval_t>(cid, value));		
		return 0;
	}
};

void EnumerationsHandler::deleteEnumMembers( const enum_t enumId )
{	
	std::vector<std::pair<const_t, uval_t>> constants;
	for_all_enum_members(enumId, collect_enum_members(constants));
	for (std::vector<std::pair<const_t, uval_t>>::iterator it = constants.begin(); it != constants.end(); it++) {
		const_t cid = it->first;
		uval_t value = it->second;
		del_enum_member(get_enum_member_enum(cid), value, get_enum_member_serial(cid), get_enum_member_bmask(cid));
	}
}
