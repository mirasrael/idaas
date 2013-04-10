#pragma once

#include <vector>
#include <idaas_types.h>

#include <typeinf.hpp>
#include <xref.hpp>

using namespace idaas;

class ReferencesHandler
{	
public:
	ReferencesHandler(void);
	~ReferencesHandler(void);

	void xrefsFrom(std::vector<IdaRef> & _return, const int32_t address, const IdaRefType::type refType);
	void xrefsTo(std::vector<IdaRef> & _return, const int32_t address, const IdaRefType::type refType);
};

