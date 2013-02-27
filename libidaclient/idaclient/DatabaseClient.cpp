#include "DatabaseClient.h"

void DatabaseClient::OnConnect(shared_ptr<socketizer::connection> connection)
{		
	ResetEvent(m_connectionClosedEvent);	
	connection->add_close_callback(&DatabaseClient::RedirectToOnDisconnect, this);
	connection->add_message_receive_callback(&DatabaseClient::RedirectToOnMessageReceive, this);
}

void DatabaseClient::OnDisconnect(socketizer::connection *connection)
{	
	SetEvent(m_connectionClosedEvent);
}

void DatabaseClient::OnMessageReceive(shared_ptr<socketizer::connection> connection, BinaryDataObjectPtr &message) {	
	if (message->DWordAt(0) == m_expectedResultCode) {
		m_commandResult = message;
		SetEvent(m_resultReceivedEvent);
	}
}

BinaryDataObjectPtr DatabaseClient::ExecuteCommand(BinaryDataObjectPtr& command, DatabaseCommands resultCode) {
	m_expectedResultCode = resultCode;		
	ResetEvent(m_resultReceivedEvent);
	send(command);	
	
	HANDLE events[] = { m_resultReceivedEvent, m_connectionClosedEvent };
	DWORD waitResult = WaitForMultipleObjects(2, events, FALSE, INFINITE);
	
	if (waitResult != WAIT_OBJECT_0)
		return BinaryDataObjectPtr();
		
	return m_commandResult;
}

DatabaseClient::DatabaseClient(std::string hostname, unsigned __int16 port) : socketizer::client(hostname, port)
{
	this->add_connect_callback(&DatabaseClient::RedirectToOnConnect, this);
	m_resultReceivedEvent = CreateEvent(0, FALSE, TRUE, 0);
	m_connectionClosedEvent = CreateEvent(0, TRUE, TRUE, 0);
}

DatabaseClient::~DatabaseClient(void)
{	
	CloseHandle(m_resultReceivedEvent);
	CloseHandle(m_connectionClosedEvent);
}
