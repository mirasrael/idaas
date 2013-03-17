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

bool _CheckPortIsOpen(int port) {
	TWinsockSingleton::create();

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");	
	SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == s)
		return false;	
	bool result = SOCKET_ERROR != bind(s, (struct sockaddr *) &addr, sizeof(addr));
	closesocket(s);
	return result;
}

int idaapi GetDatabaseServerPort()
{
	static int port = -1;
	if (port == -1) {
		port = 13044;
		while(!_CheckPortIsOpen(port) && port < 14000) {
			port++;
		}		
	}
	return port;
}

boost::shared_ptr<TServer> databaseServer;
boost::shared_ptr<Thread> databaseServerThread;

WSADATA wsaData;

void __stdcall logerror(const char *error) {
	logmsg("%s\n", error);
}

void c_log(const char *message) {
	logmsg(message);
	logmsg("\n");
}

int idaapi CreateConnection() {	
	if (0 != databaseServer) {
		CloseConnection();
	}		

	GlobalOutput.setOutputFunction(c_log);

	TWinsockSingleton::create();

	boost::shared_ptr<DatabaseHandler> handler(new DatabaseHandler());
	boost::shared_ptr<DatabaseProcessor> processor(new DatabaseProcessor(handler));
	boost::shared_ptr<TServerSocket> serverTransport(new TServerSocket(GetDatabaseServerPort()));
	boost::shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
	boost::shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());
	databaseServer = boost::shared_ptr<TServer>(new TSimpleServer(processor, serverTransport, transportFactory, protocolFactory));	
	databaseServerThread = BoostThreadFactory(false).newThread(databaseServer);
	databaseServerThread->start();	
				
	return 0;
}

int idaapi CloseConnection() {	
	if (0 != databaseServer) {	
		databaseServer->stop();		
		databaseServerThread->join();		
		databaseServer = 0;		
	}
	return 0;
}
