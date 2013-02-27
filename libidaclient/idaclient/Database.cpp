#include "boost/date_time/posix_time/posix_time_types.hpp"
#include "boost/thread.hpp"

#include "Database.h"
#include <string>
#include <fstream>

#include <DatabaseCommands.h>
#include <BinaryDataObjectBuilder.h>
#include <BinaryDataObjectReader.h>

#include "DatabaseClient.h"

using namespace boost::posix_time;

std::string Database::m_idaExecutablePath;

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

void Database::Close() {
	BinaryDataObjectBuilder builder;
	builder.Write(unsigned __int32(DatabaseCommands::Exit));
	m_client->send(builder.Build());
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
	int membersCount = m_reader->ReadUInt32();
	enumeration->costants.reserve(membersCount);
	for (int memberIndex = 0; memberIndex < membersCount; memberIndex++) {		
		shared_ptr<IdaEnumerationConstant> constant(new IdaEnumerationConstant());
		constant->id = m_reader->ReadUInt32();		
		constant->name = m_reader->ReadString();	
		constant->value = m_reader->ReadUInt32();
		constant->serial = m_reader->ReadByte();
		constant->bit_mask = m_reader->ReadUInt32();
		enumeration->costants.push_back(constant);
	}
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

bool Database::Wait() {
	BinaryDataObjectPtr request(new BinaryDataObject(sizeof(DatabaseCommands)));
	request->DWordAt(0) = DatabaseCommands::Wait;
	return 0 == m_client->ExecuteCommand(request, DatabaseCommands::Wait);
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

Database* Database::Open(std::string& path) {
	std::string infFilePath(path);
	infFilePath += ".idaas";
	std::ifstream infFile;
	infFile.open(infFilePath);
	if (!infFile.is_open())
    {
		if (!m_idaExecutablePath.empty())
		{		
			STARTUPINFO si;
			ZeroMemory(&si, sizeof(si));
			si.cb = sizeof(si);
			PROCESS_INFORMATION pi;			
			ZeroMemory(&pi, sizeof(pi));

			std::string commandLine = m_idaExecutablePath;
			commandLine.append(" ").append(path);			

			std::string workingDirectory = m_idaExecutablePath.substr(0, m_idaExecutablePath.rfind('/'));
			char *_commandLine = new char[commandLine.size() + 1];
			commandLine.copy(_commandLine, commandLine.size());			
			CreateProcess(0, _commandLine, 0, 0, 0, 0, 0, workingDirectory.c_str(), &si, &pi);						
			delete _commandLine;
			
			ptime start = second_clock::local_time();
			while (second_clock::local_time() - start < seconds(10)) {
				infFile.open(infFilePath);
				if (infFile.is_open()) 
					break;
				boost::this_thread::sleep_for(boost::chrono::milliseconds(100));
			}
		}
		if (!infFile.is_open()) {
			return 0;
		}
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

bool Database::CreateEnum(shared_ptr<IdaEnumeration> enumeration, bool async)
{
	if (!m_client->is_connected())
		return false;
	
	BinaryDataObjectBuilder builder;
	builder.Write(unsigned __int32(DatabaseCommands::CreateEnumeration));	
	WriteEnum(enumeration, builder);	
	m_client->ExecuteCommand(builder.Build(), async);	
	return true;
}

bool Database::UpdateEnum(shared_ptr<IdaEnumeration> enumeration, bool async)
{
	if (!m_client->is_connected())
		return false;
	
	BinaryDataObjectBuilder builder;
	builder.Write(unsigned __int32(DatabaseCommands::UpdateEnumeration));
	builder.Write(unsigned __int32(enumeration->id));
	WriteEnum(enumeration, builder);
	m_client->ExecuteCommand(builder.Build(), async);
	return true;
}

bool Database::DeleteEnum(shared_ptr<IdaEnumeration> enumeration, bool async)
{	
	return DeleteEnum(enumeration->id, async);
}

bool Database::DeleteEnum(unsigned __int32 id, bool async)
{
	if (!m_client->is_connected())
		return false;

	BinaryDataObjectBuilder builder;
	builder.Write(unsigned __int32(DatabaseCommands::DeleteEnumeration));
	builder.Write(id);
	m_client->ExecuteCommand(builder.Build(), async);
	return true;
}

void Database::WriteEnum(shared_ptr<IdaEnumeration> enumeration, BinaryDataObjectBuilder& output)
{
	output.Write(enumeration->name.c_str());
	output.Write(unsigned __int8(enumeration->is_bitfield ? 1 : 0));
	output.Write(unsigned __int32(enumeration->costants.size()));
	for (vector<shared_ptr<IdaEnumerationConstant>>::iterator it = enumeration->costants.begin(); it != enumeration->costants.end(); it++)
	{
		shared_ptr<IdaEnumerationConstant> &constant = *it;
		output.Write(constant->name.c_str());		
		output.Write(unsigned __int32(constant->value));		
		output.Write(unsigned __int32(constant->bit_mask));
	}
}

void Database::SetIdaHome( std::string& idaHome )
{
	m_idaExecutablePath = idaHome;
	m_idaExecutablePath.append("/idag.exe");
}
