using System.Collections;
using System.Collections.Generic;
using Idaas;
using System.Linq;

namespace Ida.Client
{
    public class Strings : IEnumerable<ida_string>
    {
        private readonly Idaas.Database.Client _client;
        private IEnumerable<ida_string> _items;

        internal Strings(Idaas.Database.Client client)
        {
            _client = client;
        }

        private IEnumerable<ida_string> Items
        {
            get { return _items ?? (_items = Load()); }
        }

        public IEnumerator<ida_string> GetEnumerator()
        {
            return Items.GetEnumerator();
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return GetEnumerator();
        }

        private IEnumerable<ida_string> Load()
        {
            return _client.listStrings();
        }

        public ida_string this[string value]
        {
            get { return Items.First(i => i.Value == value); }
        }
    }
}