#include "DatabaseHandler.h"
#include "Logging.h"
#include <pro.h>
#include <enum.hpp>

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

#define RUN_IN_MAIN_THREAD(...)\
	if (!is_main_thread()) {\
		logmsg("pass to main thread\n");\
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
	logmsg("listEnums with elements\n");

	enumeration_list_copier copier;
	copier.copy_to(_return);	
}

void DatabaseHandler::storeEnum( const ida_enum& _enum )
{

	// Your implementation goes here
	logmsg("storeEnum\n");
}

void DatabaseHandler::deleteEnum( const int32_t id )
{
	// Your implementation goes here
	logmsg("deleteEnum\n");
}
