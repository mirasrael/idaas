#pragma once

#include <string>
#include <vector>
#include <memory>

using namespace std;

struct IdaEnumerationConstant {
	unsigned __int32 id;
	string name;	
	unsigned __int32 value;
	unsigned __int8 serial;
	unsigned __int32 bit_mask;	
};

struct IdaEnumeration
{
	unsigned __int32 id;
	string name;
	bool is_bitfield;	

	vector<shared_ptr<IdaEnumerationConstant>> costants;
};

