using System;
using System.Diagnostics;
using System.IO;
using System.Net;
using System.Threading;
using Thrift.Protocol;
using Thrift.Transport;

namespace Ida.Client
{
    public class Database : IDisposable
    {
        private static string _idaExecutablePath;
        private Idaas.Database.Client _client;
        private TTransport _transport;

        public static string IdaHome
        {
            set { _idaExecutablePath = Path.Combine(value, "idag.exe"); }
        }

        public static Database Open(String path)
        {
            string infFilePath = string.Format("{0}.idaas", path);
	        if (!File.Exists(infFilePath))
	        {
	            if (_idaExecutablePath != null)
	            {
	                Process.Start(_idaExecutablePath, path);
	                DateTime start = DateTime.Now;	                
	                while (!File.Exists(infFilePath) && (DateTime.Now - start).Seconds < 10)
	                {
	                    Thread.Sleep(100);
	                }
	            }
                if (!File.Exists(infFilePath))
                {
                    return null;
                }
	        }
            var port = int.Parse(File.ReadAllText(infFilePath));
            var database = new Database();
            if (!database.Connect(new IPEndPoint(IPAddress.Loopback, port)))
            {
                return null;
            }
            return database;		
        }

        protected bool Connect(IPEndPoint endPoint)
        {
            _transport = new TSocket(endPoint.Address.ToString(), endPoint.Port);
            var proto = new TBinaryProtocol(_transport);
            _client = new Idaas.Database.Client(proto);
            Enumerations = new Enumerations(_client);
            Structures = new Structures(_client);
            Strings = new Strings(_client);

            _transport.Open();
            return true;
        }

        public void Wait()
        {
            _client.waitBackgroundTasks();
        }

        public Enumerations Enumerations { get; private set; }

        public Structures Structures { get; private set; }

        public Strings Strings { get; private set; }        

        public void Dispose()
        {
            _transport.Close();
        }
    }
}
