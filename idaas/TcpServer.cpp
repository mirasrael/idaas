#include <winsock2.h>
#include <WS2tcpip.h>
#include <BinaryDataObjectReader.h>

#include "TcpServer.h"
#include "Logging.h"
#include "CommandProcessor.h"

#include <socketizer/server.h>

char *DEFAULT_PORT = "13044";

socketizer::server* tcpServer = 0;

void __stdcall onMessageReceived(socketizer::connection *connection, BinaryDataObjectPtr& message, void *ctx) {	
	logmsg("Execute command: %d\n", *(DatabaseCommands *) message->getInfoRef());
	CommandProcessor commandProcessor;
	BinaryDataObjectPtr output;
	int result = commandProcessor.Handle(message, output);
	if (result == 0) {
		connection->send_async(output);
	} else {
		closesocket(*connection);
	}
}

void __stdcall logerror(const char *error) {
	logmsg("%s\n", error);
}

void __stdcall onServerStarted() {
	logmsg("Server started!\n");
}

void __stdcall onConnectionClosed(socketizer::connection *connection, void *ctx) {
	logmsg("Connection closed\n");
}

void __stdcall onConnectionAccepted(socketizer::connection *connection, void *ctx) {
	logmsg("Connection accepted\n");
	connection->add_message_receive_callback(onMessageReceived, 0);
	connection->add_close_callback(onConnectionClosed, 0);
}

int idaapi CreateConnection() {		
	tcpServer = new socketizer::server(atoi(DEFAULT_PORT));
	tcpServer->add_error_callback(logerror);
	tcpServer->add_start_callback(onServerStarted);
	tcpServer->add_connection_accepted_callback(onConnectionAccepted, 0);
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
