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

struct ida_struct_member {	
	1: required string name
	2: required string type	
}

struct ida_struct {	
	1: required string name
	2: bool isUnion = false
	3: list<ida_struct_member> members
}

service Database {
	list<ida_enum> listEnums(),
	bool storeEnum(1: ida_enum _enum),
	void deleteEnum(1: string name)	

	list<ida_struct> listStructures()
	bool storeStructure(1: ida_struct _struct)
	void deleteStruct(1: string name)

	void waitBackgroundTasks()
}