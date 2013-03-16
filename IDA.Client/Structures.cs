using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Xml;
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

        public ida_struct New(string name, bool isUnion = false)
        {
            ida_struct @struct = this.FirstOrDefault(e => e.Name == name);
            if (@struct == null)
            {
                @struct = new ida_struct {Name = name};
                Items.Add(@struct);
            }
            @struct.IsUnion = isUnion;
            @struct.Members = new List<ida_struct_member>();
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
                ida_struct @structure = New(reader.GetAttribute("Name"));
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
            StoreAll(loadedStructures);
        }

        public void StoreAll(IEnumerable<ida_struct> structures)
        {
            foreach (ida_struct @structure in structures)
            {
                Store(@structure);
            }
        }
    }
}