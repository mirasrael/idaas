using System.Collections;
using System.Collections.Generic;
using Idaas;

namespace Ida.Client
{
    public class Names : IEnumerable<IdaNamedAddress>
    {
        private readonly Idaas.Database.Client _client;
        private List<IdaNamedAddress> _items;

        public Names(Idaas.Database.Client client)
        {
            _client = client;
        }

        public List<IdaNamedAddress> Items
        {
            get { return _items ?? (_items = Load()); }            
        }

        private List<IdaNamedAddress> Load()
        {
            return _client.listNamedAddresses();
        }

        public IEnumerator<IdaNamedAddress> GetEnumerator()
        {
            return Items.GetEnumerator();
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return GetEnumerator();
        }
    }
}