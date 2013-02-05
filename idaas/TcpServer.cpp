#include <winsock2.h>
#include <WS2tcpip.h>

#include "TcpServer.h"
#include "Logging.h"
#include "CommandProcessor.h"

#include <socketizer/server.h>

char *DEFAULT_PORT = "13044";

socketizer::server* tcpServer = 0;

void __stdcall onMessageReceived(socketizer::connection *connection, BinaryDataObjectPtr message) {
	DatabaseCommands command = *(DatabaseCommands*) message->getInfoRef();
	logmsg("Execute command: %d\n", command);	
	CommandProcessor commandProcessor;
	BinaryDataObjectPtr output;
	int result = commandProcessor.Handle(command, output);
	if (result == 0) {
		connection->send_async(output);
	}	
}

void __stdcall logerror(const char *error) {
	logmsg("%s\n", error);
}

void __stdcall onServerStarted() {
	logmsg("Server started!\n");
}

void __stdcall onConnectionClosed(socketizer::connection *connection) {
	logmsg("Connection closed\n");
}

void __stdcall onConnectionAccepted(socketizer::connection *connection) {
	logmsg("Connection accepted\n");
	connection->add_message_receive_callback(onMessageReceived);
	connection->add_close_callback(onConnectionClosed);
}

int idaapi CreateConnection() {		
	tcpServer = new socketizer::server(atoi(DEFAULT_PORT));
	tcpServer->add_error_callback(logerror);
	tcpServer->add_start_callback(onServerStarted);
	tcpServer->add_connection_accepted_callback(onConnectionAccepted);
	tcpServer->start_async();	
	return 0;
}

int idaapi CloseConnection() {			
	if (0 != tcpServer) {
		tcpServer->stop();
		delete tcpServer;
		tcpServer = 0;
	}
	return 0;
}
