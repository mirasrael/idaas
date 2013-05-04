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
	if (is_main_thread()) {
		func();
		return;
	}

	sync_request_t request;
	request.command = func;
	execute_sync(request, MFF_WRITE);
}

template<typename _Ret>
inline _Ret run_in_main_thread(boost::function<_Ret()> &func) {
	if (is_main_thread()) {
		return func();
	}

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

DatabaseHandler::DatabaseHandler()
{
	// Your initialization goes here
}

void DatabaseHandler::listEnums(std::vector<ida_enum> & _return)
{	
	run_in_main_thread(&EnumerationsHandler::list, &enumerationsHandler, _return);
}

bool DatabaseHandler::storeEnum( const ida_enum& _enum )
{		
	return run_in_main_thread(&EnumerationsHandler::store, &enumerationsHandler, _enum);	
}

void DatabaseHandler::deleteEnum( const std::string& name )
{
	run_in_main_thread(&EnumerationsHandler::_delete, &enumerationsHandler, name);
}

void DatabaseHandler::waitBackgroundTasks()
{			
	run_in_main_thread(boost::function<bool()>(autoWait));		
}

void DatabaseHandler::listStructures( std::vector<IdaStruct> & _return )
{
	run_in_main_thread(&StructuresHandler::list, &structureHandler, _return);	
}

bool DatabaseHandler::storeStructure( const IdaStruct& _struct )
{
	return run_in_main_thread(&StructuresHandler::store, &structureHandler, _struct);	
}

void DatabaseHandler::deleteStruct( const std::string& name )
{	
	run_in_main_thread(&StructuresHandler::_delete, &structureHandler, name);	
}

bool DatabaseHandler::storeStructures( const std::vector<IdaStruct> & structs )
{
	return run_in_main_thread(&StructuresHandler::storeAll, &structureHandler, structs);	
}

bool DatabaseHandler::storeEnums( const std::vector<ida_enum> & enums )
{
	return run_in_main_thread(&EnumerationsHandler::storeAll, &enumerationsHandler, enums);
	
}

void DatabaseHandler::listStrings( std::vector<ida_string> & _return )
{
	run_in_main_thread(&StringsHandler::list, &stringsHandler, _return);
}

void DatabaseHandler::xrefsFrom( std::vector<IdaRef> & _return, const int32_t address, const IdaRefType::type refType )
{
	run_in_main_thread(boost::function<void()>(boost::bind(&ReferencesHandler::xrefsFrom, &referencesHandler, boost::ref(_return), address, refType)));
}

void DatabaseHandler::xrefsTo( std::vector<IdaRef> & _return, const int32_t address, const IdaRefType::type refType )
{
	run_in_main_thread(boost::function<void()>(boost::bind(&ReferencesHandler::xrefsTo, &referencesHandler, boost::ref(_return), address, refType)));
}

void DatabaseHandler::listFunctions( std::vector<IdaFunction> & _return )
{
	run_in_main_thread(&FunctionsHandler::list, &functionsHandler, _return);
}

void DatabaseHandler::fetchInstruction( IdaInstruction& _return, const int32_t address )
{
	run_in_main_thread(boost::function<void()>(
		boost::bind(&InstructionsHandler::fetch, &instructionsHandler, boost::ref(_return), address)));
}

void DatabaseHandler::parseTypeDeclaration( IdaTypeInfo& _return, const std::string& typeDeclaration )
{
	typesHandler.parseTypeDeclaration(_return, typeDeclaration);	
}

void DatabaseHandler::formatTypeInfo( std::string& _return, const IdaTypeInfo& typeInfo )
{
	typesHandler.formatTypeInfo(_return, typeInfo);
}

void DatabaseHandler::getFunctionFrame( IdaFunctionFrame& _return, const int32_t address )
{
	run_in_main_thread(boost::function<void()>(
		boost::bind(&FunctionsHandler::getFunctionFrame, &functionsHandler, boost::ref(_return), address)));
}


void DatabaseHandler::listNamedAddresses( std::vector<IdaNamedAddress> &_return ) {
	run_in_main_thread(boost::function<void()>(
		boost::bind(&AddressesHandler::list, &addressesHandler, boost::ref(_return))));
}