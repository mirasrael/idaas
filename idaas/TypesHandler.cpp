#include "TypesHandler.h"

#include <typeinf.hpp>

const std::regex TypesHandler::functionPtrDeclaration("^[^(]+\\([^()]*\\*\\s*\\)\\s*\\(");
const std::regex TypesHandler::functionDeclaration("^([^(]+?)((?:\\s+(?:__stdcall|__cdecl|__thiscall|__fastcall|__usercall))?)(\\(.*\\);)$");

TypesHandler::TypesHandler(void)
{
}


TypesHandler::~TypesHandler(void)
{
}

void TypesHandler::parseTypeDeclaration( IdaTypeInfo& _return, const std::string &typeDeclaration )
{
	std::string refinedTypeDeclaration;
	refinedTypeDeclaration.reserve(typeDeclaration.size() + 1);
	refinedTypeDeclaration.append(typeDeclaration);
	if (refinedTypeDeclaration.at(refinedTypeDeclaration.size()- 1) != ';') {
		refinedTypeDeclaration.append(";");
	}

	bool skipPtr = false;
	smatch match;	
	if (regex_search(refinedTypeDeclaration, match, functionDeclaration) && !regex_search(refinedTypeDeclaration, functionPtrDeclaration)) {
		refinedTypeDeclaration = match.format("$1($2*)$3");
		skipPtr = true;
	}
		
	qstring name;
	qtype type, fields;
	parse_decl(idati, refinedTypeDeclaration.c_str(), &name, &type, &fields, 0);
	_return.name = name.c_str();
	const type_t *pType = type.c_str();
	if (skipPtr) {
		pType = skip_ptr_type_header(pType);
	}
	_return.type = reinterpret_cast<const char *>(pType);
	_return.fields = reinterpret_cast<const char *>(fields.c_str());
}

void TypesHandler::formatTypeInfo( std::string& _return, const IdaTypeInfo& typeInfo )
{
	char buffer[4096];
	print_type_to_one_line(buffer, sizeof(buffer), idati, 
		reinterpret_cast<const type_t *>(typeInfo.type.c_str()), typeInfo.name.c_str(), 0, 
		reinterpret_cast<const type_t *>(typeInfo.fields.c_str()), 0);
	_return.append(buffer);
}