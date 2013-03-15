#pragma once

#include <vector>
#include <Database.h>

using namespace idaas;

class StructureHandler
{
public:
	StructureHandler(void);
	~StructureHandler(void);
	bool store( const ida_struct &_struct );
	void list( std::vector<ida_struct> &_return );
	void _delete( const std::string& name );
};

