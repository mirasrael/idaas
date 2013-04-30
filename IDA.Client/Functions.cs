using System.Collections;
using System.Collections.Generic;
using System.Linq;
using Idaas;

namespace Ida.Client
{
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

        public IdaFunction FindByName(string name)
        {
            return Items.FirstOrDefault(f => f.Name == name);
        }

        public IdaFunctionFrame GetFrame(int address)
        {
            return _client.getFunctionFrame(address);
        }
    }
}