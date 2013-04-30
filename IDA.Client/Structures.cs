using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Xml;
using Idaas;

namespace Ida.Client
{
    public static class IdaStructExtensions
    {
        public static void AddMember(this IdaStruct @struct, string name, string type)
        {
            @struct.Members.Add(new IdaStructMember { Name = name, Type = type });
        }

        public static IdaStructMember Get(this IdaStruct @struct, string name)
        {
            return @struct.Members.Find(m => m.Name == name);
        }
    }

    public static class StructuresDatabaseExtensions
    {
        public static bool Store(this Database database, IdaStruct @struct)
        {
            return database.Structures.Store(@struct);
        }

        public static IdaStruct NewStructure(this Database database, string name,
                                              IEnumerable<KeyValuePair<string, string>> members = null)
        {
            return database.Structures.New(name, false, members);
        }

        public static IdaStruct NewUnion(this Database database, string name, IEnumerable<KeyValuePair<string, string>> members = null)
        {
            return database.Structures.New(name, true, members);
        }
    }

    public class Structures : IEnumerable<IdaStruct>,  IExportable
    {
        private readonly Idaas.Database.Client _client;
        private List<IdaStruct> _items;

        internal Structures(Idaas.Database.Client client)
        {
            _client = client;
        }

        private List<IdaStruct> Items
        {
            get { return _items ?? (_items = Load()); }
        }

        public IEnumerator<IdaStruct> GetEnumerator()
        {
            return Items.GetEnumerator();
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return GetEnumerator();
        }

        private List<IdaStruct> Load()
        {
            return _client.listStructures();
        }

        public IdaStruct New(string name, bool isUnion = false, IEnumerable<KeyValuePair<string, string>> members = null)
        {
            IdaStruct @struct = this.FirstOrDefault(e => e.Name == name);
            if (@struct == null)
            {
                @struct = new IdaStruct {Name = name};
                Items.Add(@struct);
            }
            @struct.IsUnion = isUnion;
            @struct.Members = new List<IdaStructMember>();
            if (members != null)
            {
                foreach (var pair in members)
                {
                    @struct.AddMember(pair.Key, pair.Value);
                }
            }
            return @struct;
        }

        public bool Store(IdaStruct @struct)
        {
            return _client.storeStructure(@struct);
        }

        public void SaveTo(Stream output)
        {
            XmlWriter writer = XmlWriter.Create(output, new XmlWriterSettings
                {
                    Indent = true
                });
            writer.WriteStartElement("Structures");
            foreach (IdaStruct item in Items)
            {
                writer.WriteStartElement("Structure");
                writer.WriteAttributeString("Name", item.Name);
                writer.WriteAttributeString("IsUnion", item.IsUnion.ToString());
                foreach (IdaStructMember member in item.Members)
                {
                    writer.WriteStartElement("Field");
                    writer.WriteAttributeString("Name", member.Name);
                    writer.WriteAttributeString("Type", member.Type);
                    writer.WriteEndElement();
                }
                writer.WriteEndElement();
            }
            writer.WriteEndElement();
            writer.Close();
        }

        public void LoadFrom(Stream input)
        {
            XmlReader reader = XmlReader.Create(input);
            reader.ReadToFollowing("Structures");
            var loadedStructures = new List<IdaStruct>();
            for (bool hasStructure = reader.ReadToDescendant("Structure");
                 hasStructure;
                 hasStructure = reader.ReadToNextSibling("Structure"))
            {
                var isUnion = bool.TrueString.Equals(reader.GetAttribute("IsUnion"), StringComparison.CurrentCultureIgnoreCase);
                IdaStruct @structure = New(reader.GetAttribute("Name"), isUnion);
                for (bool hasField = reader.ReadToDescendant("Field");
                     hasField;
                     hasField = reader.ReadToNextSibling("Field"))
                {
                    @structure.Members.Add(new IdaStructMember
                        {
                            Name = reader.GetAttribute("Name"),
                            Type = reader.GetAttribute("Type")
                        });
                }
                loadedStructures.Add(@structure);
            }
            reader.Close();
            Store(loadedStructures);
        }

        public bool Store(IEnumerable<IdaStruct> structures)
        {
            return _client.storeStructures(structures.ToList());
        }

        public bool StoreAll()
        {
            return _client.storeStructures(Items);
        }

        public IdaStruct this[string name]
        {
            get { return this.First(s => s.Name == name); }
        }

        public bool Has(string name)
        {
            return this.Any(s => s.Name == name);
        }

        public void Delete(IdaStruct structure)
        {
            _client.deleteStruct(structure.Name);
        }
    }
}