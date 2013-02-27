#pragma once

#include <socketizer/client.h>
#include <DatabaseCommands.h>
#include <string>

class DatabaseClient : public socketizer::client
{
private:	
	static void __stdcall RedirectToOnConnect(shared_ptr<socketizer::connection> connection, void *context) {
		reinterpret_cast<DatabaseClient *>(context)->OnConnect(connection);
	}

	static void __stdcall RedirectToOnDisconnect(socketizer::connection *connection, void *context) { 
		reinterpret_cast<DatabaseClient *>(context)->OnDisconnect(connection); 
	}

	static void __stdcall RedirectToOnMessageReceive(shared_ptr<socketizer::connection> connection, BinaryDataObjectPtr &message, void *context) {
		reinterpret_cast<DatabaseClient *>(context)->OnMessageReceive(connection, message); 
	}
	
	HANDLE m_resultReceivedEvent;
	HANDLE m_connectionClosedEvent;
	DatabaseCommands m_expectedResultCode;
	BinaryDataObjectPtr m_commandResult;
protected:
	void OnDisconnect(socketizer::connection *connection);
	void OnConnect(shared_ptr<socketizer::connection> connection);
	void OnMessageReceive(shared_ptr<socketizer::connection> connection, BinaryDataObjectPtr &message);
public:
	DatabaseClient(std::string hostname, unsigned __int16 port);
	~DatabaseClient(void);
	
	void ExecuteCommand(BinaryDataObjectPtr& command, bool async) { 
		if (async) {
			send(command);
		} else {
			ExecuteCommand(command); 
		}
	}
	BinaryDataObjectPtr ExecuteCommand(BinaryDataObjectPtr& command) { return ExecuteCommand(command, *(DatabaseCommands *) command->getInfoRef()); }
	BinaryDataObjectPtr ExecuteCommand(BinaryDataObjectPtr& command, DatabaseCommands resultCode);
};

