using System;
using System.Collections.Generic;
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
            Assert.That(Database.Structures, Has.Member(testStructure));
            testStructure.Members.Add(new ida_struct_member {Name = "Member1", Type = "int"});
            Assert.That(Database.Structures.Store(testStructure), Is.True);

            Reconnect();

            testStructure = Database.Structures.First(s => s.Name == structureName);
            Assert.That(testStructure.Members.FirstOrDefault(m => m.Name == "Member1"), Is.Not.Null);
            Assert.That(testStructure.Members.FirstOrDefault(m => m.Name == "Member2"), Is.Null);
            testStructure.Members.RemoveAll(m => m.Name == "Member1");
            testStructure.Members.Add(new ida_struct_member {Name = "Member2", Type = "double"});
            Assert.That(Database.Structures.Store(testStructure), Is.True);

            Reconnect();

            testStructure = Database.Structures.First(s => s.Name == structureName);
            Assert.That(testStructure.Members.FirstOrDefault(m => m.Name == "Member1"), Is.Null);
            Assert.That(testStructure.Members.FirstOrDefault(m => m.Name == "Member2"), Is.Not.Null);
        }

        [Test]
        public void ItShouldCreateStructureMembersWithEnumType()
        {
            string structureName = GenerateUniqName();
            string enumName = GenerateUniqName();

            ida_enum testEnum = Database.NewEnumeration(enumName, constants: new Dictionary<string, int>
                {
                    {GenerateUniqName(), 0}
                });
            Database.Store(testEnum);

            ida_struct testStructure = Database.Structures.New(structureName);
            testStructure.Members.Add(new ida_struct_member {Name = "EnumMember", Type = testEnum.Name});
            Database.Store(testStructure);

            Reconnect();

            Assert.That(Database.Structures[structureName].Members.First(m => m.Name == "EnumMember").Type,
                        Is.EqualTo(testEnum.Name));
        }

        [Test]
        public void ItShouldCreateStructuresWithSpecialSymbols()
        {
            string fieldTypeName = "_MY_SYSTEM_INFO::$1593C2ABA4C275C0FBEC2498FA3B0937::$2";
            ida_struct fieldType = Database.Structures.New(fieldTypeName);
            fieldType.Members.Add(new ida_struct_member {Name = "SubMember1", Type = "int"});
            Assert.That(Database.Store(fieldType), Is.True);

            string ownerName = "_MY_SYSTEM_INFO::$1593C2ABA4C275C0FBEC2498FA3B0937";
            ida_struct owner = Database.Structures.New(ownerName);
            owner.Members.Add(new ida_struct_member {Name = "Member1", Type = fieldType.Name});
            Assert.That(Database.Store(owner), Is.True);

            Reconnect();

            Assert.That(
                Database.Structures.First(s => s.Name == ownerName).Members.FirstOrDefault(m => m.Name == "Member1"),
                Is.Not.Null);
        }

        [Test]
        public void ItShouldLoadUnionInfo()
        {
            var subStructure1 = Database.NewStructure(GenerateUniqName(),
                                                      new Dictionary<string, string>
                                                          {
                                                              {"IntMember", "int"},
                                                              {"FloatMember", "float"}
                                                          });
            var subStructure2 = Database.NewStructure(GenerateUniqName(), new Dictionary<string, string>
                {
                    {"IntMember", "int"}
                });
            var subStructure3 = Database.NewStructure(GenerateUniqName(), new Dictionary<string, string>
                {
                    {"DoubleMember", "double"}
                });
            var @enum = Database.NewUnion(GenerateUniqName(), new Dictionary<string, string>
                {
                    {"Member1", "struct " + subStructure1.Name },
                    {"Member2", "struct " + subStructure2.Name },
                    {"Member3", "struct " + subStructure3.Name },
                });
            Database.Structures.Store(new[] { subStructure1, subStructure2, subStructure3, @enum });

            Reconnect();

            @enum = Database.Structures[@enum.Name];
            Assert.That(@enum.Get("Member1").Type, Is.EqualTo("struct " + subStructure1.Name));
            Assert.That(@enum.Get("Member2").Type, Is.EqualTo("struct " + subStructure2.Name));
            Assert.That(@enum.Get("Member3").Type, Is.EqualTo("struct " + subStructure3.Name));
        }

        [Test]
        public void ItShouldCreateUnion()
        {
            string unionName = GenerateUniqName();

            ida_struct testStructure = Database.Structures.New(unionName, isUnion: true);
            testStructure.Members.Add(new ida_struct_member {Name = "Member1", Type = "int"});
            Database.Store(testStructure);

            testStructure = Database.Structures.New(unionName, isUnion: true);
            Assert.That(testStructure.Members.Exists(m => m.Name == "Member1"), Is.False);
            testStructure.Members.Add(new ida_struct_member {Name = "Member2", Type = "double"});
            Database.Store(testStructure);

            Reconnect();

            testStructure = Database.Structures.First(s => s.Name == unionName);
            Assert.That(testStructure.IsUnion, Is.True);
            Assert.That(testStructure.Members.Exists(m => m.Name == "Member1"), Is.False);
            Assert.That(testStructure.Members.Exists(m => m.Name == "Member2"), Is.True);
        }

        [Test]
        public void ItShouldDeleteStructures()
        {
            var @structure = Database.NewStructure(GenerateUniqName());
            Database.Store(@structure);
            Database.Structures.Delete(@structure);

            Reconnect();

            Assert.That(Database.Structures.Has(@structure.Name), Is.False);
        }

        [Test]
        public void ItShouldExportAndImportStructures()
        {
            var @structure = Database.NewStructure(GenerateUniqName(),
                                                   new Dictionary<string, string>
                                                       {
                                                           {"IntMember", "int"},
                                                           {"FloatMember", "float"}
                                                       });
            var @union = Database.NewUnion(GenerateUniqName(),
                                           new Dictionary<string, string>
                                               {
                                                   {"IntMember", "int"},
                                                   {"FloatMember", "float"}
                                               });

            var output = new MemoryStream();
            Database.Structures.SaveTo(output);
            output.Seek(0, SeekOrigin.Begin);
            Console.WriteLine(new StreamReader(output).ReadToEnd());
            output.Seek(0, SeekOrigin.Begin);

            Database.Structures.Delete(@structure);
            Database.Structures.Delete(@union);

            Reconnect();

            Database.Structures.LoadFrom(output);
            Assert.That(Database.Structures.Has(@structure.Name), Is.True);
            Assert.That(Database.Structures.Has(@union.Name), Is.True);
            Assert.That(Database.Structures[@union.Name].IsUnion, Is.True);
        }

        [Test]
        public void ItShouldGetStructuresList()
        {
            ida_struct devInfo = Database.Structures.First(s => s.Name == "TestStructure");
            Assert.That(devInfo, Is.Not.Null);
            Assert.That(devInfo.Members.FirstOrDefault(m => m.Name == "IntField"), Is.Not.Null);
        }

        [Test]
        public void ItShouldRecreateStructure()
        {
            string structureName = GenerateUniqName();

            ida_struct testStructure = Database.Structures.New(structureName);
            testStructure.Members.Add(new ida_struct_member {Name = "Member1", Type = "int"});
            Database.Store(testStructure);

            testStructure = Database.Structures.New(structureName);
            Assert.That(testStructure.Members.Exists(m => m.Name == "Member1"), Is.False);
            testStructure.Members.Add(new ida_struct_member {Name = "Member2", Type = "double"});
            Database.Store(testStructure);

            Reconnect();

            testStructure = Database.Structures.First(s => s.Name == structureName);
            Assert.That(testStructure.Members.Exists(m => m.Name == "Member1"), Is.False);
            Assert.That(testStructure.Members.Exists(m => m.Name == "Member2"), Is.True);
        }

        [Test]
        public void ItShouldCreateStructureWithGapSpace()
        {
            var testStructure = Database.NewStructure(GenerateUniqName(), members: new Dictionary<string, string>
                {
                    {"__gap__0", "byte[20]"},
                    {"IntMember", "int"}
                });
            Database.Store(testStructure);

            Reconnect();

            testStructure = Database.Structures[testStructure.Name];
            Assert.That(testStructure.Members.First(m => m.Name.StartsWith("__gap__")).Type, Is.EqualTo("byte[20]"));
        }

        [Test]
        public void ItShouldNotStoreGapAtTheEnd()
        {
            var testStructure = Database.NewStructure(GenerateUniqName(), members: new Dictionary<string, string>
                {
                    {"IntMember", "int"},
                    {"__gap__0", "byte[20]"},
                });
            Database.Store(testStructure);

            Reconnect();

            testStructure = Database.Structures[testStructure.Name];
            Assert.That(testStructure.Members.FirstOrDefault(m => m.Name.StartsWith("__gap__")), Is.Null);
        }

        [Test]
        public void ItShouldResolveStructureDependencies()
        {
            var ownerStructure = Database.NewStructure(GenerateUniqName());
            var nestedStructure = Database.NewStructure(GenerateUniqName(), members: new Dictionary<string, string>
                {
                    {"IntMember", "int"}
                });
            ownerStructure.AddMember("NestedMember", nestedStructure.Name);
            Database.Structures.Store(new[] {ownerStructure, nestedStructure});

            Reconnect();

            Assert.That(Database.Structures[ownerStructure.Name].Get("NestedMember").Type,
                        Is.EqualTo(nestedStructure.Name));
            Assert.That(Database.Structures[nestedStructure.Name].Get("IntMember"), Is.Not.Null);
        }
    }
}