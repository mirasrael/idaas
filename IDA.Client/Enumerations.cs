﻿using System.Collections;
using System.Collections.Generic;
using System.Linq;
using Idaas;

namespace Ida.Client
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

		public ida_enum New(string name)
		{
		    return New(name, false);
		}

        public ida_enum New(string name, bool isBitfield)
		{
		    var @enum = this.FirstOrDefault(e => e.Name == name);
            if (@enum == null)
            {            
               @enum = new ida_enum {Name = name};
               Items.Add(@enum);
            }
            @enum.Constants = new List<ida_enum_const>();
		    @enum.IsBitfield = isBitfield;            
		    return @enum;
		}

        public bool Store(ida_enum @enum)
        {
            @enum.Id = _client.storeEnum(@enum);
            return true;
        }        

        public void Delete(ida_enum @enum)
        {
            if (_items != null)
            {
                _items.Remove(@enum);
            }
            if (@enum.Id != 0)
            {
                _client.send_deleteEnum(@enum.Id);
            }
        }        

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