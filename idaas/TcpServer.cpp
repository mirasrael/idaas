#include <winsock2.h>
#include <WS2tcpip.h>
#include <BinaryDataObjectReader.h>

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/concurrency/BoostThreadFactory.h>

#include "DatabaseHandler.h"

#include "TcpServer.h"
#include "Logging.h"
#include "CommandProcessor.h"

#include <socketizer/server.h>
#include <boost/lockfree/spsc_queue.hpp>
#include <boost/thread.hpp>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;
using namespace ::apache::thrift::concurrency;

using namespace ::idaas;

char *DEFAULT_PORT = "13044";

socketizer::server* tcpServer = 0;


boost::shared_ptr<DatabaseHandler> handler(new DatabaseHandler());
boost::shared_ptr<TProcessor> processor(new DatabaseProcessor(handler));
boost::shared_ptr<TServerTransport> serverTransport;
boost::shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
boost::shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

boost::shared_ptr<TServer> databaseServer;
boost::shared_ptr<Thread> databaseServerThread;

typedef struct
{
	BinaryDataObjectPtr message;
	shared_ptr<socketizer::connection> connection;

	int execute() {
		logmsg("Execute command: %d\n", *(DatabaseCommands *) message->getInfoRef());
		CommandProcessor commandProcessor;
		BinaryDataObjectPtr output;
		int result = commandProcessor.Handle(message, output);
		if (result == 0) {
			connection->send_async(output);
		} else if (result == CommandProcessor::TERMINATE_CODE) {
			closesocket(*connection);
			term_database();
		} else {
			closesocket(*connection);		
		}		
		return result;
	}
} command_t;

boost::lockfree::spsc_queue<command_t> commands(4096);

struct execute_commands_t : exec_request_t {
	int idaapi execute() {
		command_t command;
		while (commands.pop(command))
		{
			command.execute();			
		}
		return 0;
	}
} execute_commands;

void __stdcall onMessageReceived(shared_ptr<socketizer::connection> connection, BinaryDataObjectPtr& message, void *ctx) {	
	command_t command;
	command.message = message;
	command.connection = connection;
	commands.push(command);
	execute_sync(execute_commands, MFF_WRITE);
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

void __stdcall onConnectionAccepted(shared_ptr<socketizer::connection> connection, void *ctx) {
	logmsg("Connection accepted\n");
	connection->add_message_receive_callback(onMessageReceived, 0);
	connection->add_close_callback(onConnectionClosed, 0);
}

void c_log(const char *message) {
	logmsg(message);
}

int idaapi CreateConnection() {
	int port = atoi(DEFAULT_PORT);

	if (0 != databaseServer) {
		databaseServer->stop();
	}	

	GlobalOutput.setOutputFunction(c_log);

	serverTransport = boost::shared_ptr<TServerTransport>(new TServerSocket(port + 1));
	databaseServer = boost::shared_ptr<TServer>(new TSimpleServer(processor, serverTransport, transportFactory, protocolFactory));
	//databaseServer->serve();
	databaseServerThread = BoostThreadFactory().newThread(databaseServer);
	databaseServerThread->start();
	
	if (tcpServer != 0) {
		CloseConnection();
	}
	
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
	if (0 != databaseServer) {
		databaseServer->stop();
		databaseServerThread->join();
	}
	return 0;
}
