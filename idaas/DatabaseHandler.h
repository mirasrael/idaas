#pragma once

#include <Database.h>
#include "StructureHandler.h"

class DatabaseHandler : virtual public idaas::DatabaseIf {
private:
	StructureHandler structureHandler;
public:
	DatabaseHandler();

	void listEnums(std::vector<idaas::ida_enum> & _return);

	int32_t storeEnum(const idaas::ida_enum& _enum);

	void deleteEnum(const int32_t id);

	void listStructures(std::vector<idaas::ida_struct> & _return);

	int32_t storeStructure(const idaas::ida_struct& _struct);

	void deleteStruct(const int32_t id);

	void waitBackgroundTasks();
};
