namespace cpp idaas
namespace csharp Idaas

struct ida_enum_const {
	1: i32 id
	2: string name
	3: i32 value
	4: byte serial
	5: i32 mask
}

struct ida_enum {
	1: i32 id
	2: string name
	3: bool isBitfield
	4: list<ida_enum_const> constants
}

service Database {
	list<ida_enum> listEnums(),
	void storeEnum(1: ida_enum _enum),
	void deleteEnum(1: i32 id)
}