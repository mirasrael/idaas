#pragma once

#include <Database.h>
#include "StructuresHandler.h"
#include "EnumerationsHandler.h"
#include "StringsHandler.h"
#include "ReferencesHandler.h"
#include "FunctionsHandler.h"

class DatabaseHandler : virtual public idaas::DatabaseIf {
private:
	StructuresHandler structureHandler;
	EnumerationsHandler enumerationsHandler;
	StringsHandler stringsHandler;
	ReferencesHandler referencesHandler;
	FunctionsHandler functionsHandler;
	
public:
	DatabaseHandler();

	void listEnums(std::vector<idaas::ida_enum> & _return);

	bool storeEnum(const idaas::ida_enum& _enum);

	bool storeEnums(const std::vector<ida_enum> & enums);

	void deleteEnum(const std::string& name);

	void listStructures(std::vector<idaas::ida_struct> & _return);

	bool storeStructure(const idaas::ida_struct& _struct);

	bool storeStructures(const std::vector<ida_struct> & structs);

	void deleteStruct(const std::string& name);

	void listStrings(std::vector<ida_string> & _return);

	void listFunctions(std::vector<IdaFunction> & _return);

	void xrefsFrom(std::vector<IdaRef> & _return, const int32_t address, const IdaRefType::type refType);

	void xrefsTo(std::vector<IdaRef> & _return, const int32_t address, const IdaRefType::type refType);

	void waitBackgroundTasks();
};
