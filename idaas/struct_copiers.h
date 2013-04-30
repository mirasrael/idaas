#pragma once

#pragma warning(disable: 4996)
#include <struct.hpp>
#pragma warning(default: 4996)

#include <idaas_types.h>

using namespace idaas;

class struct_copier {
public:
	static void copy(const struc_t *from, IdaStruct& to);
};

class struct_member_copier {
public:
	static void copy(const member_t *from, IdaStructMember& to);
};

class struct_list_copier {
public:	
	void copy_to(std::vector<IdaStruct>& _result);	
};

