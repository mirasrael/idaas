#pragma once

#include <idaas_types.h>
#include <vector>

#include <typeinf.hpp>
#include <funcs.hpp>

using namespace idaas;

class FunctionsHandler
{
public:
	FunctionsHandler(void);
	~FunctionsHandler(void);
	void list( std::vector<IdaFunction> & _return );
};

