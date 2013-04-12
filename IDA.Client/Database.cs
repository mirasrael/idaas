using System;
using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Net;
using System.Threading;
using Idaas;
using Thrift.Protocol;
using Thrift.Transport;
using System.Linq;

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
            Functions = new Functions(_client);

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

        public Functions Functions { get; private set; }

        public List<IdaRef> GetDataRefsTo(int address)
        {
            return _client.xrefsTo(address, IdaRefType.Data);
        }

        public List<IdaRef> GetDataRefsFrom(int address)
        {
            return _client.xrefsFrom(address, IdaRefType.Data);
        }

        public void Dispose()
        {
            _transport.Close();
        }
    }

    public class Functions : IEnumerable<IdaFunction>
    {
        private IEnumerable<IdaFunction> _items;
        private readonly Idaas.Database.Client _client;

        internal Functions(Idaas.Database.Client client)
        {
            _client = client;
        }

        public IEnumerator<IdaFunction> GetEnumerator()
        {
            return Items.GetEnumerator();
        }

        private IEnumerable<IdaFunction> Items
        {
            get { return _items ?? (_items = Load()); }
        }

        private IEnumerable<IdaFunction> Load()
        {
            return _client.listFunctions();
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return GetEnumerator();
        }

        public IdaFunction FindByAddress(int address)
        {
            return Items.FirstOrDefault(f => f.StartAddress <= address && f.EndAddress > address);
        }
    }
}
