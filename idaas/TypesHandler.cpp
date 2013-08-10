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

void TypesHandler::listTypes(vector<IdaTypeInfo>& _return, til_t *ti) {
	uint32 qty = get_ordinal_qty(ti);
	if (qty != uint32(-1)) {		
		_return.reserve(qty);
		const type_t* type;
		const p_list* fields;
		for (uint32 ordinal = 1; ordinal <= qty; ordinal++) {			
			const char* name = get_numbered_type_name(ti, ordinal);
			if (name != NULL) {
				_return.resize(_return.size() + 1);
				IdaTypeInfo& typeInfo = _return.back();
				typeInfo.name = name;				
				if (get_numbered_type(ti, ordinal, &type, &fields)) {
					typeInfo.type = reinterpret_cast<const char*>(type);
					if (fields != NULL) {
						typeInfo.fields = reinterpret_cast<const char*>(fields);
					}
				}
			}
		}
	} else {		
		const type_t *type;
		const p_list *fields;
		for (const char *name = first_named_type(ti, NTF_TYPE); name != NULL; name = next_named_type(ti, name, NTF_TYPE)) {
			_return.resize(_return.size() + 1);
			IdaTypeInfo& typeInfo = _return.back();
			typeInfo.name = name;
			if (get_named_type(ti, name, NTF_TYPE, &type, &fields) == 1) {
				typeInfo.type = reinterpret_cast<const char*>(type);
				if (fields != NULL) {
					typeInfo.fields = reinterpret_cast<const char*>(fields);
				}
			}
		}
	}
}

size_t TypesHandler::calcTotalLibrariesCount(til_t *ti) {
	size_t count = 1;
	for (int idx = 0; idx < ti->nbases; idx++) {
		count += calcTotalLibrariesCount(ti->base[idx]);
	}
	return count;
}

void TypesHandler::addTypeLibrary(vector<IdaTypeLibrary>& _return, til_t *ti) {
	for (int idx = 0; idx < ti->nbases; idx++) {		
		addTypeLibrary(_return, ti->base[idx]);
	}
	_return.resize(_return.size() + 1);
	IdaTypeLibrary& lib = _return.back();
	lib.name = ti == idati ? "_main" : ti->name;
	lib.baseLibraries.resize(ti->nbases);
	for (int idx = 0; idx < ti->nbases; idx++) {
		lib.baseLibraries[idx] = ti->base[idx]->name;
	}
	listTypes(lib.types, ti); 
}

void TypesHandler::listTypeLibraries(vector<IdaTypeLibrary>& _return) {
	_return.reserve(calcTotalLibrariesCount(idati));
	addTypeLibrary(_return, idati);
}