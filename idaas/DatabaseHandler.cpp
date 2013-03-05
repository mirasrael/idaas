#include "DatabaseHandler.h"
#include "Logging.h"
#include <pro.h>

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

template <class A1>
void run_in_main_thread(void (A1::*func)(), A1* obj) {
	sync_request_t<DatabaseHandler> request;
	request.command = boost::bind(func, obj);
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

void DatabaseHandler::listEnums()
{
	RUN_IN_MAIN_THREAD(&DatabaseHandler::listEnums, this)	
	logmsg("listEnums\n");
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
