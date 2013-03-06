#pragma once

#include "Database.h"

class DatabaseHandler : virtual public idaas::DatabaseIf {
public:
	DatabaseHandler();

	void listEnums(std::vector<idaas::ida_enum> & _return);

	void storeEnum(const idaas::ida_enum& _enum);

	void deleteEnum(const int32_t id);
};
