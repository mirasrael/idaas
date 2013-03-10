namespace cpp idaas
namespace csharp Idaas

struct ida_enum_const {
	1: i32 id = -1
	2: string name
	3: i32 value
	4: byte serial
	5: i32 mask = -1
}

struct ida_enum {
	1: i32 id = -1
	2: string name
	3: bool isBitfield
	4: list<ida_enum_const> constants
}

struct ida_struct_member {
	1: i32 id = -1
	2: string name
}

struct ida_struct {
	1: i32 id = -1
	2: string name
	3: list<ida_struct_member> members
}

service Database {
	list<ida_enum> listEnums(),
	i32 storeEnum(1: ida_enum _enum),
	void deleteEnum(1: i32 id)	

	list<ida_struct> listStructures()
	i32 storeStructure(1: ida_struct _struct)
	void deleteStruct(1: i32 id)

	void waitBackgroundTasks()
}