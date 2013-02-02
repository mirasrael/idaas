#pragma once

#include "Function.h"
#include <WinSock2.h>

typedef int (*EnumFunctionsCallback)(IdaFunction *function, void *userData);

namespace socketizer {
	class client;
}

class Database
{
public:
	Database();
	~Database(void);

	static Database* Open(const char *path);
	int Connect(const char *hostname, int port);
	int EnumFunctions(EnumFunctionsCallback callback, void *ud);
private:	
	socketizer::client *m_client;
};

