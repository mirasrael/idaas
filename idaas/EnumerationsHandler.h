#pragma once

#include <idaas_types.h>

#include <enum.hpp>

using namespace idaas;

class EnumerationsHandler
{
private:
	void deleteEnumMembers( const enum_t enumId );
public:
	EnumerationsHandler(void);
	~EnumerationsHandler(void);

	bool store( const ida_enum &_enum );
	bool storeAll( const std::vector<ida_enum> &enums );
	void list( std::vector<ida_enum> &_return );
	void _delete( const std::string& name );	
};

