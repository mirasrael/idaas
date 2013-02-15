#pragma once

#include <memory>

#include "Function.h"
#include "Enumeration.h"
#include <WinSock2.h>

using namespace std;

typedef int (*EnumFunctionsCallback)(shared_ptr<IdaFunction> &function, void *userData);

class BinaryDataObjectReader;
class BinaryDataObjectBuilder;

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
private: //fields
	DatabaseClient *m_client;

private: //methods
	void WriteEnum(shared_ptr<IdaEnumeration> enumeration, BinaryDataObjectBuilder& output);

public:
	Database();
	~Database(void);

	static Database* Open(const char *path);
	int Connect(const char *hostname, int port);
	int EnumFunctions(EnumFunctionsCallback callback, void *ud);
	shared_ptr<EnumerationsReader> GetEnumerationsReader();
	bool CreateEnum(shared_ptr<IdaEnumeration> enumeration);
	bool UpdateEnum(shared_ptr<IdaEnumeration> enumeration);
	bool DeleteEnum(unsigned __int32 id);
	bool DeleteEnum(shared_ptr<IdaEnumeration> enumeration);
};

