#pragma once

#include <Database.h>
#include "StructuresHandler.h"
#include "EnumerationsHandler.h"
#include "StringsHandler.h"

class DatabaseHandler : virtual public idaas::DatabaseIf {
private:
	StructuresHandler structureHandler;
	EnumerationsHandler enumerationsHandler;
	StringsHandler stringsHandler;
	
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

	void waitBackgroundTasks();
};
