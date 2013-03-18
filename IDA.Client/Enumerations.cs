using System.Collections;
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

    public static class IdaEnumExtensions
    {
        public static void AddConstant(this ida_enum @enum, string name, int value)
        {
            @enum.Constants.Add(new ida_enum_const {Name = name, Value = value});
        }
    }

    public static class EnumerationsDatabaseExtensions
    {
        public static bool Store(this Database database, ida_enum @enum)
        {
            return database.Enumerations.Store(@enum);
        }

        public static ida_enum NewEnumeration(this Database database, string name, bool isBitfield = false,
                                              IEnumerable<KeyValuePair<string, int>> constants = null)
        {
            return database.Enumerations.New(name, isBitfield, constants);
        }
    }

    public class Enumerations : IEnumerable<ida_enum>
    {
        private readonly Idaas.Database.Client _client;
        private List<ida_enum> _items;

        internal Enumerations(Idaas.Database.Client client)
        {
            _client = client;
        }

        private List<ida_enum> Items
        {
            get { return _items ?? (_items = Load()); }
        }

        public IEnumerator<ida_enum> GetEnumerator()
        {
            return Items.GetEnumerator();
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return GetEnumerator();
        }

        private List<ida_enum> Load()
        {
            return _client.listEnums();
        }

        public ida_enum New(string name, bool isBitfield = false,
                            IEnumerable<KeyValuePair<string, int>> constants = null)
        {
            ida_enum @enum = this.FirstOrDefault(e => e.Name == name);
            if (@enum == null)
            {
                @enum = new ida_enum {Name = name};
                Items.Add(@enum);
            }
            @enum.Constants = new List<ida_enum_const>();
            @enum.IsBitfield = isBitfield;
            if (constants != null)
            {
                foreach (var pair in constants)
                {
                    @enum.Constants.Add(new ida_enum_const {Name = pair.Key, Value = pair.Value});
                }
            }
            return @enum;
        }

        public bool Store(ida_enum @enum)
        {
            return _client.storeEnum(@enum);
        }

        public void Delete(ida_enum @enum)
        {
            if (_items != null)
            {
                _items.Remove(@enum);
            }
            _client.deleteEnum(@enum.Name);
        }

        public bool Store(IEnumerable<ida_enum> enums)
        {
            return _client.storeEnums(enums.ToList());
        }

        public bool StoreAll()
        {
            return _client.storeEnums(Items);
        }
    };
}