#pragma once

#include <vector>
#include <regex>
#include <Database.h>
#include <struct.hpp>

using namespace idaas;

class StructuresHandler
{	
private:
	typedef std::map<std::string, const ida_struct*> index_t;

	const static std::regex invalidIdentifier;
	const static std::regex typeName;

	std::string escapeTypes( const std::string& decl, std::vector<std::pair<tid_t, std::string>>& escapedTypes );
	void restoreTypes( std::vector<std::pair<tid_t, std::string>>& escapedTypes );
	bool storeWithDependencies( index_t& index, index_t::iterator &who );
public:
	StructuresHandler(void);
	~StructuresHandler(void);
	bool store( const ida_struct &_struct );
	bool storeAll( const std::vector<ida_struct> &structs );
	void list( std::vector<ida_struct> &_return );
	void _delete( const std::string& name );	
};

