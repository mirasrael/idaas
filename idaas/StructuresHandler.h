#pragma once

#include <vector>
#include <regex>
#include <Database.h>
#include <struct.hpp>

using namespace idaas;

class StructuresHandler
{
private:
	const static std::regex invalidIdentifier;
	std::string escapeTypes( const std::string& decl, std::vector<std::pair<tid_t, std::string>>& escapedTypes );
	void restoreTypes( std::vector<std::pair<tid_t, std::string>>& escapedTypes );
public:
	StructuresHandler(void);
	~StructuresHandler(void);
	bool store( const ida_struct &_struct );
	bool storeAll( const std::vector<ida_struct> &structs );
	void list( std::vector<ida_struct> &_return );
	void _delete( const std::string& name );	
};

