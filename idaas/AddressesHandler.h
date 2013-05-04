#pragma once

#include <idaas_types.h>

#include <vector>

using namespace std;
using namespace idaas;

class AddressesHandler
{
private:
	IdaSegmentType::type mapSegmentType(int segmentType);
public:
	AddressesHandler(void);
	~AddressesHandler(void);
	void list( std::vector<IdaNamedAddress> &_return );	
};

