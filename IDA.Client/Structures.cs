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
        public static void AddMember(this ida_struct @struct, string name, string type)
        {
            @struct.Members.Add(new ida_struct_member { Name = name, Type = type });
        }
        
        public static ida_struct_member Get(this ida_struct @struct, string name)
        {
            return @struct.Members.Find(m => m.Name == name);
        }
    }

    public static class StructuresDatabaseExtensions
    {
        public static bool Store(this Database database, ida_struct @struct)
        {
            return database.Structures.Store(@struct);
        }

        public static ida_struct NewStructure(this Database database, string name,
                                              IEnumerable<KeyValuePair<string, string>> members = null)
        {
            return database.Structures.New(name, false, members);
        }

        public static ida_struct NewUnion(this Database database, string name, IEnumerable<KeyValuePair<string, string>> members = null)
        {
            return database.Structures.New(name, true, members);
        }
    }

    public class Structures : IEnumerable<ida_struct>,  IExportable
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

        public IEnumerator<ida_struct> GetEnumerator()
        {
            return Items.GetEnumerator();
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return GetEnumerator();
        }

        private List<ida_struct> Load()
        {
            return _client.listStructures();
        }

        public ida_struct New(string name, bool isUnion = false, IEnumerable<KeyValuePair<string, string>> members = null)
        {
            ida_struct @struct = this.FirstOrDefault(e => e.Name == name);
            if (@struct == null)
            {
                @struct = new ida_struct {Name = name};
                Items.Add(@struct);
            }
            @struct.IsUnion = isUnion;
            @struct.Members = new List<ida_struct_member>();
            if (members != null)
            {
                foreach (var pair in members)
                {
                    @struct.AddMember(pair.Key, pair.Value);
                }
            }
            return @struct;
        }

        public bool Store(ida_struct @struct)
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
            foreach (ida_struct item in Items)
            {
                writer.WriteStartElement("Structure");
                writer.WriteAttributeString("Name", item.Name);
                writer.WriteAttributeString("IsUnion", item.IsUnion.ToString());
                foreach (ida_struct_member member in item.Members)
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
            var loadedStructures = new List<ida_struct>();
            for (bool hasStructure = reader.ReadToDescendant("Structure");
                 hasStructure;
                 hasStructure = reader.ReadToNextSibling("Structure"))
            {
                var isUnion = bool.TrueString.Equals(reader.GetAttribute("IsUnion"), StringComparison.CurrentCultureIgnoreCase);
                ida_struct @structure = New(reader.GetAttribute("Name"), isUnion);
                for (bool hasField = reader.ReadToDescendant("Field");
                     hasField;
                     hasField = reader.ReadToNextSibling("Field"))
                {
                    @structure.Members.Add(new ida_struct_member
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

        public bool Store(IEnumerable<ida_struct> structures)
        {
            return _client.storeStructures(structures.ToList());
        }

        public bool StoreAll()
        {
            return _client.storeStructures(Items);
        }

        public ida_struct this[string name]
        {
            get { return this.First(s => s.Name == name); }
        }

        public bool Has(string name)
        {
            return this.Any(s => s.Name == name);
        }

        public void Delete(ida_struct structure)
        {
            _client.deleteStruct(structure.Name);
        }
    }
}