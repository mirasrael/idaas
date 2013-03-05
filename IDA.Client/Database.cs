using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Idaas;
using Thrift.Protocol;
using Thrift.Transport;

namespace IDA.Client
{
    public class Database
    {
        public void Test()
        {
            var transport = new TSocket("localhost", 13045);
            var proto = new TBinaryProtocol(transport);
            var client = new Idaas.Database.Client(proto);
            transport.Open();

            client.listEnums();
            client.storeEnum(new ida_enum {Id = 1, IsBitfield = true, Name = "ololo"});
        }
    }
}
