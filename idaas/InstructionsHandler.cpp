#include "InstructionsHandler.h"

InstructionsHandler::InstructionsHandler(void)
{	
	registerMapping = NULL;
}


InstructionsHandler::~InstructionsHandler(void)
{
	if (registerMapping != NULL) {
		delete[] registerMapping;
	}
}

void InstructionsHandler::fetch( IdaInstruction &_return, int32_t address )
{
	_return.address = address;
	_return.size = decode_insn(address);
	if (_return.size == 0) {
		_return.size = create_insn(address);
	}
	_return.mnemonic = ua_mnem(address, buffer, sizeof(buffer));	
	if (cmd.auxpref & 0x0001) {
		_return.prefixes.push_back("lock");
	}
	if (cmd.auxpref & 0x0002) {
		_return.prefixes.push_back("rep");
	}
	if (cmd.auxpref & 0x0004) {
		_return.prefixes.push_back("repnz");
	}
	_return.operands.resize(UA_MAXOP);
	size_t n = 0;
	while ( n < UA_MAXOP && cmd.Operands[n].type != o_void) {		
		fetchOperand(_return.operands[n], cmd.Operands[n]);
		n++;
	}
	_return.operands.resize(n);
}

void InstructionsHandler::fetchOperand(IdaOperand &_return, op_t &input) 
{
	switch (input.dtyp)
	{
	case dt_byte:
		_return.size = 1;
		break;
	case dt_word:
		_return.size = 2;
		break;
	case dt_dword:
	case dt_float:
		_return.size = 4;
		break;
	case dt_qword:
	case dt_double:
		_return.size = 8;
		break;
	case dt_byte16:
		_return.size = 16;
		break;
	default:
		break;
	}
	switch(input.type) {
	case o_reg:		
		_return.type = IdaOperandType::Register;
		_return.baseRegister = mapRegister(input.reg);
		break;
	case o_imm:
		_return.type = IdaOperandType::Constant;
		break;
	case o_mem:
		_return.type = IdaOperandType::Memory;
		break;
	case o_displ:
	case o_phrase:
		_return.type = IdaOperandType::Dislacement;
		break;
	case o_near:
	case o_far:
		_return.type = IdaOperandType::Address;
		break;
	case o_idpspec3:
		_return.type = IdaOperandType::FPRegister;
		break;
	default:
		_return.type = IdaOperandType::Unknown;
		break;
	}
}

IdaRegister::type InstructionsHandler::mapRegister( int reg )
{	
	if (registerMapping == NULL) {
		prepareMapping();
	}
	if (reg < 0 || reg >= (int) registerMappingSize)
		return IdaRegister::Unknown;
	return registerMapping[reg];
}

void InstructionsHandler::prepareMapping()
{
	registerMappingSize = 1024;
	registerMapping = new IdaRegister::type[registerMappingSize];
	for (size_t i = 0; i < registerMappingSize; i++) {
		registerMapping[i] = IdaRegister::Unknown;
	}

	setRegisterMapping("al", IdaRegister::Al);
	setRegisterMapping("ah", IdaRegister::Ah);
	setRegisterMapping("ax", IdaRegister::Ax);
	setRegisterMapping("eax", IdaRegister::Eax);
	setRegisterMapping("bl", IdaRegister::Bl);
	setRegisterMapping("bh", IdaRegister::Bh);
	setRegisterMapping("bx", IdaRegister::Bx);
	setRegisterMapping("ebx", IdaRegister::Ebx);
	setRegisterMapping("cl", IdaRegister::Cl);
	setRegisterMapping("ch", IdaRegister::Ch);
	setRegisterMapping("cx", IdaRegister::Cx);
	setRegisterMapping("ecx", IdaRegister::Ecx);
	setRegisterMapping("dl", IdaRegister::Dl);
	setRegisterMapping("dh", IdaRegister::Dh);
	setRegisterMapping("dx", IdaRegister::Dx);
	setRegisterMapping("edx", IdaRegister::Edx);
	setRegisterMapping("esi", IdaRegister::Esi);
	setRegisterMapping("edi", IdaRegister::Edi);
	setRegisterMapping("ebp", IdaRegister::Ebp);	
	setRegisterMapping("esp", IdaRegister::Esp);
}

void InstructionsHandler::setRegisterMapping( const char *registerName, IdaRegister::type idaRegister )
{	
	int reg = str2reg(registerName);
	if (reg < 0 || reg >= (int) registerMappingSize) {
		msg("Can't make mapping for %s\n", registerName);
		return;
	}
	registerMapping[reg] = idaRegister;
}
