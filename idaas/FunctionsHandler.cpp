#include "FunctionsHandler.h"

FunctionsHandler::FunctionsHandler(void)
{
}


FunctionsHandler::~FunctionsHandler(void)
{
}

void FunctionsHandler::list( std::vector<IdaFunction> & _return )
{
	size_t count = get_func_qty();
	_return.resize(count);
	char buffer[1024];
	for (size_t idx = 0; idx < count; idx++) {
		IdaFunction &func = _return.at(idx);
		func_t* f = getn_func(idx);
		func.startAddress = f->startEA;
		func.endAddress = f->endEA;
		func.type = "";		
		func.name = get_func_name(func.startAddress, buffer, sizeof(buffer));
	}
}
