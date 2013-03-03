namespace cpp idaas

struct ida_enum {
	1: i32 id
	2: string name
	3: bool isBitfield
}

struct ida_enum_const {
	1: i32 id
	2: string name
	3: i32 value
	4: byte serial
	5: bool mask
}

service Database {
	void listEnums(),
	void storeEnum(1: ida_enum _enum),
	void deleteEnum(1: i32 id)
}