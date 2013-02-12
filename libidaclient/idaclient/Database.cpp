#include "Database.h"
#include <string>
#include <fstream>

#include <DatabaseCommands.h>
#include <BinaryDataObjectBuilder.h>
#include <BinaryDataObjectReader.h>

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

EnumerationsReader::EnumerationsReader(shared_ptr<BinaryDataObjectReader> reader) : m_reader(reader) {
	m_count = m_reader->ReadUInt32();
}

shared_ptr<IdaEnumeration> EnumerationsReader::Read() {
	if (m_count <= 0) {
		return shared_ptr<IdaEnumeration>();
	}

	m_count--;
	shared_ptr<IdaEnumeration> enumeration(new IdaEnumeration());
	enumeration->id = m_reader->ReadUInt32();		
	enumeration->name = m_reader->ReadString();		
	enumeration->is_bitfield = m_reader->ReadBoolean();
	return enumeration;
}

shared_ptr<EnumerationsReader> Database::GetEnumerationsReader() {
	if (!m_client->is_connected())
		throw "Client is not connected";

	unsigned __int32 command = DatabaseCommands::EnumerationsList;	

	BinaryDataObjectBuilder builder;
	builder.Write(command);	
		
	BinaryDataObjectPtr result = m_client->ExecuteCommand(builder.Build(), DatabaseCommands::EnumerationsList);	
	if (0 == result) {
		throw "Command failed";
	}
	
	shared_ptr<BinaryDataObjectReader> reader(new BinaryDataObjectReader(result));
	reader->SkipUInt32(); // Command	
	return shared_ptr<EnumerationsReader>(new EnumerationsReader(reader));
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
		shared_ptr<IdaFunction> function(new IdaFunction());
		callback(function, ud);
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