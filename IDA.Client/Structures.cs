using System.Collections;
using System.Collections.Generic;
using System.Linq;
using Idaas;

namespace Ida.Client
{
    public class Structures : IEnumerable<ida_struct>
    {
        private readonly Idaas.Database.Client _client;
        private List<ida_struct> _items;

        internal Structures(Idaas.Database.Client client)
        {
            _client = client;
        }        

        private List<ida_struct> Items
        {
            get { return _items ?? (_items = Load()); }
        }

        private List<ida_struct> Load()
        {
            return _client.listStructures();
        }

        public IEnumerator<ida_struct> GetEnumerator()
        {
            return Items.GetEnumerator();
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return GetEnumerator();
        }

        public ida_struct New(string name)
        {
            var @struct = this.FirstOrDefault(e => e.Name == name);
            if (@struct == null)
            {
                @struct = new ida_struct { Name = name };
                Items.Add(@struct);
            }
            @struct.Members = new List<ida_struct_member>();            
            return @struct;
        }

        public bool Store(ida_struct @struct)
        {
            @struct.Id = _client.storeStructure(@struct);
            return true;
        }
    }
}
