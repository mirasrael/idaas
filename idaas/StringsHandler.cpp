#include "StringsHandler.h"
#include <typeinf.hpp>
#include <strlist.hpp>

StringsHandler::StringsHandler(void)
{
}


StringsHandler::~StringsHandler(void)
{
}

void StringsHandler::list( vector<ida_string> &_result )
{	
	strwinsetup_t setup;
	memset(&setup, 0, sizeof(setup));
	setup.strtypes = 1 << ASCSTR_C;
	setup.ea1 = inf.minEA;
	setup.ea2 = inf.maxEA;	
	setup.minlen = 5;
	setup.only_7bit = true;
	setup.display_only_existing_strings = false;
	set_strlist_options(&setup);	

	refresh_strlist(inf.minEA, inf.maxEA);
	size_t qty = get_strlist_qty();	
	_result.resize(qty);
	string_info_t si;
	
	char buffer[8192];
	for (size_t i = 0; i < qty; i++) {
		get_strlist_item(i, &si);
		_result[i].address = si.ea;		
		get_ascii_contents(si.ea, si.length, si.type, buffer, sizeof(buffer));
		_result[i].value = buffer;
	}
}
