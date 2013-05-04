#pragma once

#include <Database.h>
#include "StructuresHandler.h"
#include "EnumerationsHandler.h"
#include "StringsHandler.h"
#include "ReferencesHandler.h"
#include "FunctionsHandler.h"
#include "InstructionsHandler.h"
#include "TypesHandler.h"
#include "AddressesHandler.h"

class DatabaseHandler : virtual public idaas::DatabaseIf {
private:
	StructuresHandler structureHandler;
	EnumerationsHandler enumerationsHandler;
	StringsHandler stringsHandler;
	ReferencesHandler referencesHandler;
	FunctionsHandler functionsHandler;
	InstructionsHandler instructionsHandler;
	TypesHandler typesHandler;
	AddressesHandler addressesHandler;
	
public:
	DatabaseHandler();

	void listEnums(std::vector<idaas::ida_enum> & _return);

	bool storeEnum(const idaas::ida_enum& _enum);

	bool storeEnums(const std::vector<ida_enum> & enums);

	void deleteEnum(const std::string& name);

	void listStructures(std::vector<idaas::IdaStruct> & _return);

	bool storeStructure(const idaas::IdaStruct& _struct);

	bool storeStructures(const std::vector<IdaStruct> & structs);

	void deleteStruct(const std::string& name);

	void listStrings(std::vector<ida_string> & _return);

	void listFunctions(std::vector<IdaFunction> & _return);

	void getFunctionFrame(IdaFunctionFrame& _return, const int32_t address);

	void xrefsFrom(std::vector<IdaRef> & _return, const int32_t address, const IdaRefType::type refType);

	void xrefsTo(std::vector<IdaRef> & _return, const int32_t address, const IdaRefType::type refType);

	void fetchInstruction(IdaInstruction& _return, const int32_t address);

	void parseTypeDeclaration(IdaTypeInfo& _return, const std::string& typeDeclaration);

	void formatTypeInfo(std::string& _return, const IdaTypeInfo& typeInfo);
	
	void listNamedAddresses( std::vector<IdaNamedAddress> &_return );

	void waitBackgroundTasks();
};
