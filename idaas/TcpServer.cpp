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
	//logmsg("Checking port %d\n", port);
	SOCKET s = socket(addr.sin_family, SOCK_STREAM, 0);	
	if (INVALID_SOCKET == s) {
		//logmsg("No socket\n");
		return false;
	}

	u_long iMode = 1;
	if (SOCKET_ERROR == ioctlsocket(s, FIONBIO, &iMode)) {
		//logmsg("Can't set non-blocking mode\n");
		closesocket(s);
		return false;
	}
	if (SOCKET_ERROR != connect(s, (struct sockaddr *) &addr, sizeof(addr))) {
		//logmsg("Connect immediately\n");
		closesocket(s);
		return false;
	}
	if (WSAEWOULDBLOCK != WSAGetLastError()) {
		//logmsg("Connect failed with not wouldn't block: %d\n", WSAGetLastError());
		closesocket(s);
		return true;
	}
	timeval t = { 0, 100 };
	fd_set writeSet;
	FD_ZERO(&writeSet);
	FD_SET(s, &writeSet);
	if (select(NULL, NULL, &writeSet, NULL, &t) > 0) {
		//logmsg("Connection successful\n");
		closesocket(s);
		return false;
	}

	//logmsg("Port is available\n");
	closesocket(s);
	return true;
}

int idaapi GetDatabaseServerPort()
{
	static int port = -1;
	if (port == -1) {
		port = 13044;		
		while(!_CheckPortIsOpen(port) && port < 14000) {
			port++;
		}
		logmsg("Selected port: %d\n", port);
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
