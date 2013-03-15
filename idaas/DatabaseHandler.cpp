#include "DatabaseHandler.h"
#include "Logging.h"
#include <pro.h>
#include <enum.hpp>
#include <auto.hpp>

using namespace idaas;

#include <boost/bind.hpp>
#include <boost/function.hpp>

template<class _Ret>
struct sync_request_with_result_t : exec_request_t {
	_Ret result;
	boost::function<_Ret()> command;

	int idaapi execute() {
		result = command();
		return 0;
	}
};

struct sync_request_t : exec_request_t {	
	boost::function<void()> command;

	int idaapi execute() {
		command();
		return 0;
	}
};

inline void run_in_main_thread(boost::function<void()> &func) {
	sync_request_t request;
	request.command = func;
	execute_sync(request, MFF_WRITE);
}

template<typename _Ret>
inline _Ret run_in_main_thread(boost::function<_Ret()> &func) {
	sync_request_with_result_t<_Ret> request;
	request.command = func;
	execute_sync(request, MFF_WRITE);	
	return request.result;
}

template<class _Ret, class _This>
_Ret run_in_main_thread(_Ret (_This::*func)(), _This* obj) {	
	return (_Ret) run_in_main_thread(boost::function<_Ret()>(
		boost::bind(func, obj)
		));
};

template<class _Ret, class _This, class A0>
_Ret run_in_main_thread(_Ret (_This::*func)(A0&), _This* obj, A0& arg0) {	
	return (_Ret) run_in_main_thread(boost::function<_Ret()>(
		boost::bind(func, obj, boost::ref(arg0))
		));
};

template<class _Ret, class _This, class A0>
_Ret run_in_main_thread(_Ret (_This::*func)(A0), _This* obj, A0 arg0) {
	return (_Ret) run_in_main_thread(boost::function<_Ret()>(
		boost::bind(func, obj, arg0)
		));
};

#define RUN_IF_IN_MAIN_THREAD(RET, ...)\
	if (!is_main_thread()) {\
	return (RET)run_in_main_thread(##__VA_ARGS__);\
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

void DatabaseHandler::listEnums(std::vector<ida_enum> & _return)
{
	RUN_IF_IN_MAIN_THREAD(void, &DatabaseHandler::listEnums, this, _return);
	enumeration_list_copier copier;
	copier.copy_to(_return);	
}

struct delete_enum_members : public enum_member_visitor_t {
	virtual int idaapi visit_enum_member(const_t cid, uval_t value) {
		del_enum_member(get_enum_member_enum(cid), value, get_enum_member_serial(cid), get_enum_member_bmask(cid));
		return 0;
	}
};

bool DatabaseHandler::storeEnum( const ida_enum& _enum )
{		
	RUN_IF_IN_MAIN_THREAD(int32_t, &DatabaseHandler::storeEnum, this, _enum);
	enum_t id = get_enum(_enum.name.c_str());
	if (BADADDR != id) {		
		for_all_enum_members(id, delete_enum_members());		
	} else {
		id = add_enum(BADADDR, _enum.name.c_str(), 0);
		if (id == BADADDR)
			return false;
	}	
	set_enum_bf(id, _enum.isBitfield);		
	for (std::vector<ida_enum_const>::const_iterator it = _enum.constants.begin(); it != _enum.constants.end(); it++) {
		if (0 != add_enum_member(id, it->name.c_str(), it->value, it->mask)) {
			return false;
		}
	}
	return true;
}

void DatabaseHandler::deleteEnum( const std::string& name )
{
	RUN_IF_IN_MAIN_THREAD(void, &DatabaseHandler::deleteEnum, this, name);
	del_enum(get_enum(name.c_str()));
}

void DatabaseHandler::waitBackgroundTasks()
{
	RUN_IF_IN_MAIN_THREAD(void, &DatabaseHandler::waitBackgroundTasks, this);
	autoWait();
}

void DatabaseHandler::listStructures( std::vector<ida_struct> & _return )
{
	RUN_IF_IN_MAIN_THREAD(void, &StructureHandler::list, &structureHandler, _return);	
	structureHandler.list(_return);
}

bool DatabaseHandler::storeStructure( const ida_struct& _struct )
{
	RUN_IF_IN_MAIN_THREAD(bool, &StructureHandler::store, &structureHandler, _struct);
	return structureHandler.store(_struct);
}

void DatabaseHandler::deleteStruct( const std::string& name )
{	
	RUN_IF_IN_MAIN_THREAD(void, &StructureHandler::_delete, &structureHandler, name);
	structureHandler._delete(name);
}


