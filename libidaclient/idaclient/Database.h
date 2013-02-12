#pragma once

#include <memory>

#include "Function.h"
#include "Enumeration.h"
#include <WinSock2.h>

using namespace std;

typedef int (*EnumFunctionsCallback)(shared_ptr<IdaFunction> &function, void *userData);

class BinaryDataObjectReader;

class EnumerationsReader {
	shared_ptr<BinaryDataObjectReader> m_reader;
	int m_count;
public:
	EnumerationsReader(shared_ptr<BinaryDataObjectReader> reader);
	shared_ptr<IdaEnumeration> Read();
};

class DatabaseClient;

class Database
{
public:
	Database();
	~Database(void);

	static Database* Open(const char *path);
	int Connect(const char *hostname, int port);
	int EnumFunctions(EnumFunctionsCallback callback, void *ud);
	shared_ptr<EnumerationsReader> GetEnumerationsReader();
private:	
	DatabaseClient *m_client;
};

