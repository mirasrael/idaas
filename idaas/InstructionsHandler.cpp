#include "InstructionsHandler.h"
#include <intel.hpp>

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

int InstructionsHandler::calculateOperandSize(const op_t &input) {
	switch (input.dtyp)
	{
	case dt_byte:
		return 1;	
	case dt_word:
		return 2;		
	case dt_dword:
	case dt_float:
		return 4;		
	case dt_qword:
	case dt_double:
		return 8;		
	case dt_byte16:
		return 16;		
	default:
		return -1;
	}
}

void InstructionsHandler::setIndexRegister(IdaOperand &_return, const op_t &input)
{	
	int index = x86_index(input);
	if (index != INDEX_NONE) {
		_return.indexRegister = mapRegister(index);
		_return.indexScale = x86_scale(input);
	} else {
		_return.indexRegister = IdaRegister::None;
	}
}

void InstructionsHandler::fetchOperand(IdaOperand &_return, const op_t &input) 
{
	_return.size = calculateOperandSize(input);
	switch(input.type) {
	case o_reg:		
		_return.type = IdaOperandType::Register;
		_return.register_ = mapRegister(input.reg);
		break;
	case o_imm:
		_return.type = IdaOperandType::Constant;
		_return.value = input.value;
		break;
	case o_mem:
		_return.type = IdaOperandType::Memory;
		_return.address = input.addr;
		setIndexRegister(_return, input);
		break;
	case o_displ:
	case o_phrase:
		_return.type = IdaOperandType::Dislacement;
		_return.address = input.addr;
		_return.baseRegister = mapRegister(x86_base(input));
		setIndexRegister(_return, input);
		break;
	case o_near:
	case o_far:
		_return.type = IdaOperandType::Address;
		_return.address = input.addr;
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
	setRegisterMapping("xmm0", IdaRegister::Xmm0);
	setRegisterMapping("xmm1", IdaRegister::Xmm1);
	setRegisterMapping("xmm2", IdaRegister::Xmm2);
	setRegisterMapping("xmm3", IdaRegister::Xmm3);
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
