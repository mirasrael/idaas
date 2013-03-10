#include "DatabaseHandler.h"
#include "Logging.h"
#include <pro.h>
#include <enum.hpp>
#include <auto.hpp>

using namespace idaas;

#include <boost/bind.hpp>
#include <boost/function.hpp>

template<class T>
struct sync_request_t_with_result : exec_request_t {
	T result;
	T (*command)();

	int idaapi execute() {
		result = command();
	}
};

template<class T>
struct sync_request_t : exec_request_t {	
	boost::function<void()> command;	

	int idaapi execute() {
		command();
		return 0;
	}
};

template<class _This>
void run_in_main_thread(void (_This::*func)(), _This* obj) {
	sync_request_t<DatabaseHandler> request;
	request.command = boost::bind(func, obj);
	execute_sync(request, MFF_WRITE);
};

template<class _This, class A0>
void run_in_main_thread(void (_This::*func)(A0&), _This* obj, A0& arg0) {	
	sync_request_t<DatabaseHandler> request;
	request.command = boost::bind(func, obj, boost::ref(arg0));
	execute_sync(request, MFF_WRITE);
};

template<class _This, class A0>
void run_in_main_thread(void (_This::*func)(A0), _This* obj, A0 arg0) {
	sync_request_t<DatabaseHandler> request;
	request.command = boost::bind(func, obj, arg0);
	execute_sync(request, MFF_WRITE);
};

#define RUN_IN_MAIN_THREAD(...)\
	if (!is_main_thread()) {\
		run_in_main_thread(##__VA_ARGS__);\
		return;\
	}\

DatabaseHandler::DatabaseHandler()
{
	// Your initialization goes here
}

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
	constantsIt->id = cid;
	get_enum_member_name(cid, buffer, sizeof(buffer));
	constantsIt->name = buffer;
	constantsIt->value = value;
	constantsIt->serial = get_enum_member_serial(cid);
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
		current->id = id;
		current->name = buffer;
		current->isBitfield = is_bf(id);
		current->constants.resize(get_enum_size(id));
		constantsIt = current->constants.begin();
		for_all_enum_members(id, *this);
	}		
}

void DatabaseHandler::listEnums(std::vector<ida_enum> & _return)
{
	RUN_IN_MAIN_THREAD(&DatabaseHandler::listEnums, this, _return)	
	enumeration_list_copier copier;
	copier.copy_to(_return);	
}

struct delete_enum_members : public enum_member_visitor_t {
	virtual int idaapi visit_enum_member(const_t cid, uval_t value) {
		del_enum_member(get_enum_member_enum(cid), value, get_enum_member_serial(cid), get_enum_member_bmask(cid));
		return 0;
	}
};

void DatabaseHandler::storeEnum( const ida_enum& _enum )
{	
	RUN_IN_MAIN_THREAD(&DatabaseHandler::storeEnum, this, _enum)
	enum_t id = _enum.id;
	if (id != -1) {		
		set_enum_name(id, _enum.name.c_str());
		for_all_enum_members(id, delete_enum_members());		
	} else {
		id = add_enum(BADADDR, _enum.name.c_str(), 0);		
	}
	set_enum_bf(id, _enum.isBitfield);		
	for (std::vector<ida_enum_const>::const_iterator it = _enum.constants.begin(); it != _enum.constants.end(); it++) {		
		add_enum_member(id, it->name.c_str(), it->value, it->mask);
	}	
}

void DatabaseHandler::deleteEnum( const int32_t id )
{
	RUN_IN_MAIN_THREAD(&DatabaseHandler::deleteEnum, this, id)
	del_enum(id);
}

void DatabaseHandler::waitBackgroundTasks()
{
	RUN_IN_MAIN_THREAD(&DatabaseHandler::waitBackgroundTasks, this)
	autoWait();
}

void DatabaseHandler::listStructures( std::vector<ida_struct> & _return )
{
	RUN_IN_MAIN_THREAD(&DatabaseHandler::listStructures, this, _return)
}

void DatabaseHandler::storeStructure( const ida_struct& _struct )
{
	RUN_IN_MAIN_THREAD(&DatabaseHandler::storeStructure, this, _struct)
}

void DatabaseHandler::deleteStruct( const int32_t id )
{
	RUN_IN_MAIN_THREAD(&DatabaseHandler::deleteStruct, this, id)
}


