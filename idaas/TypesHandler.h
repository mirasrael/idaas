#pragma once

#include <string>
#include <regex>

#include <idaas_types.h>

#include <typeinf.hpp>

using namespace std;
using namespace idaas;

class TypesHandler
{
private:
	const static std::regex functionDeclaration;
	const static std::regex functionPtrDeclaration;
public:
	TypesHandler(void);
	~TypesHandler(void);

	void parseTypeDeclaration( IdaTypeInfo& _return, const std::string &typeDeclaration );
	void formatTypeInfo( std::string& _return, const IdaTypeInfo& typeInfo );	
	void listTypeLibraries(vector<IdaTypeLibrary>& _return);
private:
	void listTypes(vector<IdaTypeInfo>& _return, til_t *ti);
	void addTypeLibrary(vector<IdaTypeLibrary>& _return, til_t *ti);
	size_t calcTotalLibrariesCount(til_t *ti);
};

