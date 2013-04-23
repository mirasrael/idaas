#pragma once

#include <idaas_types.h>
#include <typeinf.hpp>
#include <ua.hpp>

using namespace idaas;

class InstructionsHandler
{
private:
	char buffer[256];
	size_t registerMappingSize;
	IdaRegister::type *registerMapping;
	void prepareMapping();
	void setRegisterMapping(const char *registerName, IdaRegister::type idaRegister);
	void fetchOperand(IdaOperand &_return, op_t &input);
	IdaRegister::type mapRegister(int reg);
public:
	InstructionsHandler(void);
	~InstructionsHandler(void);

	void fetch(IdaInstruction &_return, int32_t address);
};

