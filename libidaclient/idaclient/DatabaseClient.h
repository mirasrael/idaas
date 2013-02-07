#pragma once

#include <socketizer/client.h>
#include <string>

class DatabaseClient : public socketizer::client
{
private:
	static void __stdcall RedirectToOnConnect(socketizer::connection *connection, void *context) {
		reinterpret_cast<DatabaseClient *>(context)->OnConnect(connection);
	}

	static void __stdcall RedirectToOnDisconnect(socketizer::connection *connection, void *context) { 
		reinterpret_cast<DatabaseClient *>(context)->OnDisconnect(connection); 
	}

	static void __stdcall RedirectToOnMessageReceive(socketizer::connection *connection, BinaryDataObjectPtr &message, void *context) {
		reinterpret_cast<DatabaseClient *>(context)->OnMessageReceive(connection, message); 
	}
	
	HANDLE m_resultReceivedEvent;
	HANDLE m_connectionClosedEvent;
	int m_expectedResultCode;
	BinaryDataObjectPtr m_commandResult;
protected:
	void OnDisconnect(socketizer::connection *connection);
	void OnConnect(socketizer::connection *connection);
	void OnMessageReceive(socketizer::connection *connection, BinaryDataObjectPtr &message);
public:
	DatabaseClient(std::string hostname, unsigned __int16 port);
	~DatabaseClient(void);

	BinaryDataObjectPtr ExecuteCommand(BinaryDataObjectPtr& command, int resultCode);
};

