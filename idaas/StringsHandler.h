#pragma once

#include <string>
#include <vector>
#include <idaas_types.h>

using namespace std;
using namespace idaas;

class StringsHandler
{
public:
	StringsHandler(void);
	~StringsHandler(void);

	void list(vector<ida_string> &_result);
};

