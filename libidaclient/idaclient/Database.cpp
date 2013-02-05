#include "Database.h"
#include <string>
#include <fstream>
#include <DatabaseCommands.h>

#include <socketizer\client.h>

Database::Database()
{	
	m_client = new socketizer::client();
}


Database::~Database(void)
{
	delete m_client;
}

int Database::Connect(const char *hostname, int port) {
	try {
		m_client->connect(hostname, port);		
		return 0;
	} catch (const char *) {
		return -1;
	}	
}

int Database::EnumFunctions(EnumFunctionsCallback callback, void *ud) {
	if (!m_client->is_connected())
		return -1;

	__int32 commandSize = 4;
	m_client->send((char *) &commandSize, sizeof(commandSize));
	__int32 command = DatabaseCommands::FunctionsList;
	m_client->send((char *) &command, sizeof(command));
	char header[4];
	int result = m_client->recieve(header, sizeof(header));
	if (result != 4) {
		return result;
	}
	__int32 count = *(__int32 *) header;
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