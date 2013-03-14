﻿using System;
using System.IO;
using System.Linq;
using Idaas;
using NUnit.Framework;
using NUnit.Framework.Constraints;

namespace Ida.Client.Test
{
    [TestFixture]
    public class DescribeStructures : DatabaseSpec
    {
        private class HasMemberConstraint : Constraint
        {
            public string Name { private get; set; }
            public string Type { private get; set; }

            public override bool Matches(object oStruct)
            {
                var @struct = oStruct as ida_struct;
                if (@struct == null)
                {
                    actual = "No struct";
                    return false;
                }
                ida_struct_member member = @struct.Members.Find(m => m.Name == Name);
                if (member == null)
                {
                    actual = "No member";
                    return false;
                }
                actual = member;
                return member.Type == Type;
            }

            public override void WriteDescriptionTo(MessageWriter writer)
            {
                writer.WriteExpectedValue(
                    string.Format("Name = {0} Type = {1}", Name, Type));
            }
        }

        private HasMemberConstraint HasMember(string name, string type)
        {
            return new HasMemberConstraint {Name = name, Type = type};
        }

        [Test]
        public void IsShouldAddPrimitiveMembers()
        {
            ida_struct testStructure = Database.Structures.New(GenerateUniqName());
            testStructure.Members.Add(new ida_struct_member {Name = "ByteMember", Type = "unsigned __int8"});
            testStructure.Members.Add(new ida_struct_member {Name = "WordMember", Type = "unsigned __int16"});
            testStructure.Members.Add(new ida_struct_member {Name = "DWordMember", Type = "unsigned __int32"});
            testStructure.Members.Add(new ida_struct_member {Name = "ByteArray", Type = "unsigned __int8[2]"});
            Assert.That(Database.Store(testStructure), Is.True);

            Reconnect();

            testStructure = Database.Structures.First(s => s.Name == testStructure.Name);
            Assert.That(testStructure, HasMember("ByteMember", "unsigned __int8"));
            Assert.That(testStructure, HasMember("WordMember", "unsigned __int16"));
            Assert.That(testStructure, HasMember("DWordMember", "unsigned __int32"));            
            Assert.That(testStructure, HasMember("ByteArray", "unsigned __int8[2]"));
        }

        [Test]
        public void ItShouldCreateStructure()
        {
            string structureName = GenerateUniqName();

            ida_struct testStructure = Database.Structures.New(structureName);
            Assert.That(testStructure.Name, Is.EqualTo(structureName));
            Assert.That(testStructure.Id, Is.EqualTo(-1));
            Assert.That(Database.Structures, Has.Member(testStructure));
            testStructure.Members.Add(new ida_struct_member {Name = "Member1"});
            Assert.That(Database.Structures.Store(testStructure), Is.True);
            Assert.That(testStructure.Id, Is.Not.EqualTo(-1));

            Reconnect();

            testStructure = Database.Structures.First(s => s.Name == structureName);
            Assert.That(testStructure.Members.FirstOrDefault(m => m.Name == "Member1"), Is.Not.Null);
            Assert.That(testStructure.Members.FirstOrDefault(m => m.Name == "Member2"), Is.Null);
            testStructure.Members.RemoveAll(m => m.Name == "Member1");
            testStructure.Members.Add(new ida_struct_member {Name = "Member2"});
            Assert.That(Database.Structures.Store(testStructure), Is.True);

            Reconnect();

            testStructure = Database.Structures.First(s => s.Name == structureName);
            Assert.That(testStructure.Members.FirstOrDefault(m => m.Name == "Member1"), Is.Null);
            Assert.That(testStructure.Members.FirstOrDefault(m => m.Name == "Member2"), Is.Not.Null);
        }

        [Test]
        public void ItShouldGetStructuresList()
        {
            ida_struct devInfo = Database.Structures.First(s => s.Name == "_SP_DEVINFO_DATA");
            Assert.That(devInfo, Is.Not.Null);
            Assert.That(devInfo.Members.FirstOrDefault(m => m.Name == "ClassGuid"), Is.Not.Null);
            var output = new MemoryStream();
            Database.Structures.SaveTo(output);
            output.Seek(0, SeekOrigin.Begin);
            Console.WriteLine(new StreamReader(output).ReadToEnd());
            output.Seek(0, SeekOrigin.Begin);
            Database.Structures.LoadFrom(output);
        }

        [Test]
        public void ItShouldRecreateStructure()
        {
            string structureName = GenerateUniqName();

            ida_struct testStructure = Database.Structures.New(structureName);
            testStructure.Members.Add(new ida_struct_member {Name = "Member1"});
            Database.Store(testStructure);

            testStructure = Database.Structures.New(structureName);
            Assert.That(testStructure.Members.Exists(m => m.Name == "Member1"), Is.False);
            testStructure.Members.Add(new ida_struct_member {Name = "Member2"});
            Database.Store(testStructure);

            Reconnect();

            testStructure = Database.Structures.First(s => s.Name == structureName);
            Assert.That(testStructure.Members.Exists(m => m.Name == "Member1"), Is.False);
            Assert.That(testStructure.Members.Exists(m => m.Name == "Member2"), Is.True);
        }
    }
}