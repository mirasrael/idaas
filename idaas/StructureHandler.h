#pragma once

#include <vector>
#include <regex>
#include <Database.h>
#include <struct.hpp>

using namespace idaas;

class StructureHandler
{
private:
	const static std::regex invalidIdentifier;
	std::string escapeTypes( const std::string& decl, std::vector<std::pair<tid_t, std::string>>& escapedTypes );
	void restoreTypes( std::vector<std::pair<tid_t, std::string>>& escapedTypes );
public:
	StructureHandler(void);
	~StructureHandler(void);
	bool store( const ida_struct &_struct );
	void list( std::vector<ida_struct> &_return );
	void _delete( const std::string& name );	
};

