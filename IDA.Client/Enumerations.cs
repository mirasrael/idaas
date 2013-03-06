using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Idaas;

namespace IDA.Client
{
    public enum ItemState
    {
        New,
        Updated
    };

    public class Enumerations : IEnumerable<ida_enum>
    {
        private readonly Idaas.Database.Client _client;
        private List<ida_enum> _items;
        private Dictionary<ida_enum, ItemState> _itemStates;

        private List<ida_enum> Items
        {
            get { return _items ?? (_items = Load()); }
        }

        private List<ida_enum> Load()
        {
            return _client.listEnums();
        }

        internal Enumerations(Idaas.Database.Client client)
        {
            _client = client;
        }

//			Enumeration^ New(String^ name) { return New(name, false); } 
//			Enumeration^ New(String^ name, bool isBitfield);
//			bool Persist(Enumeration^ enumeration) { return Persist(enumeration, false); }
//			bool Persist(Enumeration^ enumeration, bool async);
//			bool Delete(Enumeration^ enumeration) { return Delete(enumeration, false); }
//			bool Delete(Enumeration^ enumeration, bool async);
        public IEnumerator<ida_enum> GetEnumerator()
        {
            return Items.GetEnumerator();
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return GetEnumerator();
        }
    };
}