#include "ReferencesHandler.h"


ReferencesHandler::ReferencesHandler(void)
{
}


ReferencesHandler::~ReferencesHandler(void)
{
}

void ReferencesHandler::xrefsFrom( std::vector<IdaRef> & _return, const int32_t address, const IdaRefType::type refType )
{
	switch (refType) {
	case IdaRefType::Data:
		{
			for (ea_t to = get_first_dref_from(address); to != BADADDR; to = get_next_dref_from(address, to)) {
				IdaRef ref;
				ref.refFrom = address;
				ref.refTo = to;
				_return.push_back(ref);
			}
		}
		break;
	default:
		break;
	}
}

void ReferencesHandler::xrefsTo( std::vector<IdaRef> & _return, const int32_t address, const IdaRefType::type refType )
{
	switch (refType) {
	case IdaRefType::Data:
		{
			for (ea_t from = get_first_dref_to(address); from != BADADDR; from = get_next_dref_to(address, from)) {
				IdaRef ref;
				ref.refFrom = from;
				ref.refTo = address;			
				_return.push_back(ref);
			}
		}
		break;
	default:
		break;
	}
}
