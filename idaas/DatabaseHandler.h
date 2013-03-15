#pragma once

#include <Database.h>
#include "StructureHandler.h"

class DatabaseHandler : virtual public idaas::DatabaseIf {
private:
	StructureHandler structureHandler;
public:
	DatabaseHandler();

	void listEnums(std::vector<idaas::ida_enum> & _return);

	bool storeEnum(const idaas::ida_enum& _enum);

	void deleteEnum(const std::string& name);

	void listStructures(std::vector<idaas::ida_struct> & _return);

	bool storeStructure(const idaas::ida_struct& _struct);

	void deleteStruct(const std::string& name);

	void waitBackgroundTasks();
};
