#pragma once

#include <string>
#include <regex>

#include <idaas_types.h>

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
};

