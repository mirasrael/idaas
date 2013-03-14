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

        public ida_struct New(string name)
        {
            ida_struct @struct = this.FirstOrDefault(e => e.Name == name);
            if (@struct == null)
            {
                @struct = new ida_struct {Name = name};
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

        public void SaveTo(Stream output)
        {
            XmlWriter writer = XmlWriter.Create(output);
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
            _items = new List<ida_struct>();
            XmlReader reader = XmlReader.Create(input);
            reader.ReadToFollowing("Structures");
            for (bool hasStructure = reader.ReadToDescendant("Structure");
                 hasStructure;
                 hasStructure = reader.ReadToNextSibling("Structure"))
            {
                var @structure = new ida_struct
                    {
                        Name = reader.GetAttribute("Name"),
                        Members = new List<ida_struct_member>()
                    };
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
                _items.Add(@structure);
            }
            reader.Close();
        }
    }
}