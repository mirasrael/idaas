#pragma once

#include <vector>
#include <regex>
#include <Database.h>
#include <struct.hpp>

#include "struct_copiers.h"

using namespace idaas;

class StructuresHandler
{	
private:
	typedef std::map<std::string, const IdaStruct*> index_t;

	const static std::regex invalidIdentifier;
	const static std::regex typeName;

	std::string escapeTypes( const std::string& decl, std::vector<std::pair<tid_t, std::string>>& escapedTypes );
	void restoreTypes( std::vector<std::pair<tid_t, std::string>>& escapedTypes );
	bool storeWithDependencies( index_t& index, index_t::iterator &who );
public:
	StructuresHandler(void);
	~StructuresHandler(void);
	bool store( const IdaStruct &_struct );
	bool storeAll( const std::vector<IdaStruct> &structs );
	void list( std::vector<IdaStruct> &_return );
	void _delete( const std::string& name );
};

