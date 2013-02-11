#pragma once

#include "Function.h"
#include "Enumeration.h"
#include <WinSock2.h>

typedef int (*EnumFunctionsCallback)(IdaFunction *function, void *userData);
typedef int (*EnumEnumerationsCallback)(IdaEnumeration *function, void *userData);

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

