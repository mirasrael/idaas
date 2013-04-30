#include "FunctionsHandler.h"

#include <struct.hpp>
#include <frame.hpp>
#include <vector>

#include "struct_copiers.h"

using namespace std;

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
		qtype type, fields;		
		if (!get_tinfo(func.startAddress, &type, &fields)) {					
			guess_func_tinfo(f, &type, &fields);
		}
		print_type_to_one_line(buffer, sizeof(buffer), idati, type.c_str(), 0, 0, fields.c_str(), 0);
		func.type = buffer;	
		func.name = get_func_name(func.startAddress, buffer, sizeof(buffer));
	}
}

void FunctionsHandler::getFunctionFrame( IdaFunctionFrame& _return, const int32_t address )
{
	struc_t *frame = get_frame(address);
	IdaStruct temp;
	struct_copier::copy(frame, temp);
	typedef vector<IdaStructMember>::iterator members_iterator;
	members_iterator it = temp.members.end();
	while (it != temp.members.begin() && (--it)->name != " r");	
	for (members_iterator mit = it + 1; mit != temp.members.end(); mit++) {
		_return.arguments.push_back(*mit);
	}
	--it; // move iterator to " s"
	while (it != temp.members.begin())
	{
		--it;
		_return.variables.push_back(*it);
	}
}