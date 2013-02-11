#include "Database.h"
#include <string>
#include <fstream>
#include <DatabaseCommands.h>
#include <BinaryDataObjectBuilder.h>

#include "DatabaseClient.h"

Database::Database() : m_client(0)
{		
}


Database::~Database(void)
{
	if (0 != m_client) {
		delete m_client;
	}
}

int Database::Connect(const char *hostname, int port) {
	try {
		m_client = new DatabaseClient(hostname, port);
		m_client->run_async();
		return 0;
	} catch (const char *) {
		return -1;
	}	
}

int Database::EnumEnumerations(EnumEnumerationsCallback callback, void *ud) {
	if (!m_client->is_connected())
		return -1;

	unsigned __int32 command = DatabaseCommands::EnumerationsList;	

	BinaryDataObjectBuilder builder;
	builder.Write(command);	
		
	BinaryDataObjectPtr result = m_client->ExecuteCommand(builder.Build(), DatabaseCommands::EnumerationsList);	
	if (0 == result) {
		return -1;
	}

	__int32 count = result->DWordAt(4);
	char *ptr = (char *) result->getInfoRef() + 8;
	for (int i = 0; i < count; i++) {
		IdaEnumeration enumeration;
		enumeration.name = ptr;
		ptr += enumeration.name.size() + 1;
		enumeration.is_bitfield = *ptr;
		callback(&enumeration, ud);
	}
	return 0;
}

int Database::EnumFunctions(EnumFunctionsCallback callback, void *ud) {
	if (!m_client->is_connected())
		return -1;

	unsigned __int32 command = DatabaseCommands::FunctionsList;	

	BinaryDataObjectBuilder builder;
	builder.Write(command);	
		
	BinaryDataObjectPtr result = m_client->ExecuteCommand(builder.Build(), DatabaseCommands::FunctionsList);	
	if (0 == result) {
		return -1;
	}

	__int32 count = result->DWordAt(4);
	for (int i = 0; i < count; i++) {
		IdaFunction function;
		callback(&function, ud);
	}
	return 0;
}

Database* Database::Open(const char *path) {
	std::string infFilePath(path);
	infFilePath += ".idaas";
	std::ifstream infFile;
	infFile.open(infFilePath);
	if (!infFile.is_open())
    {
		return 0;		
	}
	char portString[30];	
	infFile.getline(portString, sizeof(portString));
	int port = atoi(portString);
    //return Open(new IPEndPoint(IPAddress.Loopback, port));
	infFile.close();
	Database *database = new Database();
	if (0 != database->Connect("127.0.0.1", port)) {
		delete database;
		return 0;
	}
	return database;
}