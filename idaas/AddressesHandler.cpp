#include "AddressesHandler.h"

#include <typeinf.hpp>
#include <name.hpp>

AddressesHandler::AddressesHandler(void)
{
}


AddressesHandler::~AddressesHandler(void)
{
}

IdaSegmentType::type AddressesHandler::mapSegmentType(int segmentType)
{
	switch (segmentType)
	{
	case SEG_DATA:
		return IdaSegmentType::Data;
	case SEG_CODE:
		return IdaSegmentType::Code;
	case SEG_XTRN:
		return IdaSegmentType::Import;
	default:
		return IdaSegmentType::Unknown;
	}
}

void AddressesHandler::list( std::vector<IdaNamedAddress> &_return )
{		
	char buffer[4096];
	qtype type, fields;
	_return.resize(get_nlist_size());
	for (size_t i = 0; i < _return.size(); i++) {
		bool hasTypeInfo = false;
		IdaNamedAddress &addr = _return.at(i);
		ea_t address = get_nlist_ea(i);
		IdaSegmentType::type segment = mapSegmentType(getseg(address)->type);
		const char *name = get_nlist_name(i);
		if (segment == IdaSegmentType::Data) {
			hasTypeInfo = get_tinfo(address, &type, &fields) || guess_tinfo(address, &type, &fields);
		} else if (segment == IdaSegmentType::Code) {
			if (isFunc(getFlags(address))) {
				hasTypeInfo = get_tinfo(address, &type, &fields) || guess_func_tinfo(get_func(address), &type, &fields) != GUESS_FUNC_FAILED;				
			}
		}		
		addr.address = address;
		addr.name = name;
		addr.segment = segment;		
		if (hasTypeInfo) {
			if (T_NORMAL == print_type_to_one_line(buffer, sizeof(buffer), idati, type.c_str(), 0, 0, fields.c_str(), 0)) {
				addr.type = buffer;
			}
		}
		if (addr.type.empty()) {
			addr.type = "unsigned char";
		}
	}	
}
