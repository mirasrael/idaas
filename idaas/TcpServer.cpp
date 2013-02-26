#include <winsock2.h>
#include <WS2tcpip.h>
#include <BinaryDataObjectReader.h>

#include "TcpServer.h"
#include "Logging.h"
#include "CommandProcessor.h"

#include <socketizer/server.h>
#include <boost/thread.hpp>
#include <boost/thread/condition_variable.hpp>

char *DEFAULT_PORT = "13044";

socketizer::server* tcpServer = 0;
boost::thread serve_clients_signaler;
boost::mutex mutex;
boost::condition_variable wakeup_signaler;

int gCount = 0;
struct serve_clients_sync_t : exec_request_t {
	int idaapi execute() {		
		tcpServer->serve_clients();		
		wakeup_signaler.notify_one();
		return 0;
	}
} serve_clients_sync;

struct serve_clients_signaler_proc {
	void operator()() {
		boost::unique_lock<boost::mutex> lock(mutex);
		while (0 != tcpServer)
		{			
			wakeup_signaler.wait(lock);			
			if (0 != tcpServer)
			{
				execute_sync(serve_clients_sync, MFF_NOWAIT | MFF_WRITE);
			}			
		}		
	};
};

struct term_kernel_sync_t : exec_request_t {
	int idaapi execute() {
		//term_kernel();
		return 0;
	}
} term_kernel_sync;

struct term_database_sync_t : exec_request_t {
	int idaapi execute() {
		term_database();
		CloseConnection();
		execute_sync(term_kernel_sync, MFF_NOWAIT | MFF_WRITE);
		return 0;
	}
} term_database_sync;

struct handle_command_sync_t : exec_request_t {	
	shared_ptr<socketizer::connection> connection;
	BinaryDataObjectPtr message;	

	handle_command_sync_t(shared_ptr<socketizer::connection> connection, BinaryDataObjectPtr& message) :
		connection(connection), message(message) {};

	int idaapi execute() {
		logmsg("Execute command: %d\n", *(DatabaseCommands *) message->getInfoRef());
		CommandProcessor commandProcessor;
		BinaryDataObjectPtr output;
		int result = commandProcessor.Handle(message, output);
		if (result == 0) {
			connection->send_async(output);
		} else if (result == CommandProcessor::TERMINATE_CODE) {
			closesocket(*connection);
			execute_sync(term_database_sync, MFF_NOWAIT);				
		} else {
			closesocket(*connection);		
		}
		return result;
	}
};

void __stdcall onMessageReceived(shared_ptr<socketizer::connection> connection, BinaryDataObjectPtr& message, void *ctx) {	
	handle_command_sync_t(connection, message).execute();
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

int idaapi CreateConnection() {		
	if (tcpServer != 0) {
		CloseConnection();
	}

	//serve_clients_signaler = boost::thread(serve_clients_signaler_proc());
	
	tcpServer = new socketizer::server(atoi(DEFAULT_PORT));
	tcpServer->add_error_callback(logerror);
	tcpServer->add_start_callback(onServerStarted);
	tcpServer->add_connection_accepted_callback(onConnectionAccepted, 0);
	tcpServer->start_async();
	//tcpServer->setup();
	//wakeup_signaler.notify_one();	
		
	return 0;
}

int idaapi CloseConnection() {
	{	
		boost::lock_guard<boost::mutex> lock(mutex);
		if (0 != tcpServer) {
			tcpServer->stop();		
			delete tcpServer;
			tcpServer = 0;		
		}
	}
	msg("Send notification on connection close\n");
	wakeup_signaler.notify_one();
	serve_clients_signaler.join();
	return 0;
}
