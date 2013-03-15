#include <winsock2.h>
#include <WS2tcpip.h>

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/concurrency/BoostThreadFactory.h>

#include "DatabaseHandler.h"

#include "TcpServer.h"
#include "Logging.h"

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;
using namespace ::apache::thrift::concurrency;

using namespace ::idaas;

char *DEFAULT_PORT = "13044";

boost::shared_ptr<DatabaseHandler> handler(new DatabaseHandler());
boost::shared_ptr<TProcessor> processor(new DatabaseProcessor(handler));
boost::shared_ptr<TServerTransport> serverTransport;
boost::shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
boost::shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

boost::shared_ptr<TServer> databaseServer;
boost::shared_ptr<Thread> databaseServerThread;

void __stdcall logerror(const char *error) {
	logmsg("%s\n", error);
}

void c_log(const char *message) {
	logmsg(message);
	logmsg("\n");
}

int idaapi CreateConnection() {
	int port = atoi(DEFAULT_PORT);

	if (0 != databaseServer) {
		databaseServer->stop();
	}	

	GlobalOutput.setOutputFunction(c_log);

	serverTransport = boost::shared_ptr<TServerTransport>(new TServerSocket(port));
	databaseServer = boost::shared_ptr<TServer>(new TSimpleServer(processor, serverTransport, transportFactory, protocolFactory));	
	databaseServerThread = BoostThreadFactory().newThread(databaseServer);
	databaseServerThread->start();
				
	return 0;
}

int idaapi CloseConnection() {	
	if (0 != databaseServer) {
		databaseServer->stop();
		databaseServerThread->join();
	}
	return 0;
}
