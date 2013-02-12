#pragma once

#include <memory>

#include "Function.h"
#include "Enumeration.h"
#include <WinSock2.h>

using namespace std;

typedef int (*EnumFunctionsCallback)(shared_ptr<IdaFunction> &function, void *userData);
typedef int (*EnumEnumerationsCallback)(shared_ptr<IdaEnumeration> &function, void *userData);

class DatabaseClient;

class Database
{
public:
	Database();
	~Database(void);

	static Database* Open(const char *path);
	int Connect(const char *hostname, int port);
	int EnumFunctions(EnumFunctionsCallback callback, void *ud);
	int EnumEnumerations(EnumEnumerationsCallback callback, void *ud);
private:	
	DatabaseClient *m_client;
};

