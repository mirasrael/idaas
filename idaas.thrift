namespace cpp idaas
namespace csharp Idaas

struct ida_enum_const {	
	1: required string name
	2: required i32 value	
	3: i32 mask = -1
}

struct ida_enum {	
	1: required string name
	2: bool isBitfield = false
	3: list<ida_enum_const> constants
}

struct IdaStructMember {	
	1: required string name
	2: required string type	
}

struct IdaStruct {	
	1: required string name
	2: bool isUnion = false
	3: list<IdaStructMember> members
}

struct ida_string {
	1: required i32 address
	2: required string value
}

enum IdaRefType {
	Data = 1
}

struct IdaRef {
	1: required i32 refFrom
	2: required i32 refTo
}

struct IdaFunction {
	1: required i32 startAddress
	2: required i32 endAddress
	3: required string name
	4: required string type
}

struct IdaFunctionFrame {
	1: required i32 address
	2: required list<IdaStructMember> arguments
	3: required list<IdaStructMember> variables
}

enum IdaOperandType {
	Unknown = 0
	Register = 1
	Constant = 2
	Memory = 3
	Dislacement = 4
	Address = 5
	FPRegister = 6
}

enum IdaRegister {
	Unknown = 0	
	Al
	Ah
	Ax
	Eax
	Bl
	Bh
	Bx
	Ebx
	Cl
	Ch
	Cx
	Ecx
	Dl
	Dh
	Dx
	Edx
	Esi
	Edi
	Ebp
	Esp
	Xmm0
	Xmm1
	Xmm2
	Xmm3
	None
}

struct IdaOperand {
	1: required IdaOperandType type
	2: IdaRegister register_	
	3: i32 address
	4: IdaRegister baseRegister
	5: IdaRegister indexRegister
	6: i32 indexScale
	7: required byte size
	8: i32 value
}

struct IdaInstruction {
	1: required i32 address
	2: required i32 size
	3: required string mnemonic
	4: required list<IdaOperand> operands	
	5: required list<string> prefixes
}

struct IdaTypeInfo {
	1: string name
	2: string type
	3: string fields
}

service Database {
	list<ida_enum> listEnums()
	bool storeEnum(1: ida_enum _enum)	
	bool storeEnums(1: list<ida_enum> enums)
	void deleteEnum(1: string name)

	list<IdaStruct> listStructures()
	bool storeStructure(1: IdaStruct _struct)
	bool storeStructures(1: list<IdaStruct> structs)
	void deleteStruct(1: string name)

	list<ida_string> listStrings()

	list<IdaRef> xrefsTo(1: i32 address, 2: IdaRefType refType)
	list<IdaRef> xrefsFrom(1: i32 address, 2: IdaRefType refType)

	IdaInstruction fetchInstruction(1: i32 address)

	list<IdaFunction> listFunctions()
	IdaFunctionFrame getFunctionFrame(1: i32 address)

	IdaTypeInfo parseTypeDeclaration(string typeDeclaration)
	string formatTypeInfo(IdaTypeInfo typeInfo)

	void waitBackgroundTasks()
}