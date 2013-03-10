using System.Linq;
using Idaas;
using NUnit.Framework;

namespace Ida.Client.Test
{
    [TestFixture]
    public class DescribeStructures : DatabaseSpec
    {
        [Test]
        public void ItShouldGetStructuresList()
        {            
            var devInfo = Database.Structures.First(s => s.Name == "_SP_DEVINFO_DATA");
            Assert.That(devInfo, Is.Not.Null);
            Assert.That(devInfo.Members.FirstOrDefault(m => m.Name == "ClassGuid"), Is.Not.Null);
        }

        [Test]
        public void ItShouldCreateStructure()
        {
            var structureName = GenerateUniqName();

            var testStructure = Database.Structures.New(structureName);
            Assert.That(testStructure.Name, Is.EqualTo(structureName));
            Assert.That(testStructure.Id, Is.EqualTo(-1));
            Assert.That(Database.Structures, Has.Member(testStructure));
            testStructure.Members.Add(new ida_struct_member { Name = "Member1" });
            Assert.That(Database.Structures.Store(testStructure), Is.True);
            Assert.That(testStructure.Id, Is.Not.EqualTo(-1));
            
            Reconnect();

            testStructure = Database.Structures.First(s => s.Name == structureName);                        
            Assert.That(testStructure.Members.FirstOrDefault(m => m.Name == "Member1"), Is.Not.Null);
            Assert.That(testStructure.Members.FirstOrDefault(m => m.Name == "Member2"), Is.Null);
            testStructure.Members.RemoveAll(m => m.Name == "Member1");
            testStructure.Members.Add(new ida_struct_member { Name = "Member2" });
            Assert.That(Database.Structures.Store(testStructure), Is.True);
            
            Reconnect();

            testStructure = Database.Structures.First(s => s.Name == structureName);            
            Assert.That(testStructure.Members.FirstOrDefault(m => m.Name == "Member1"), Is.Null);
            Assert.That(testStructure.Members.FirstOrDefault(m => m.Name == "Member2"), Is.Not.Null);            
        }

        [Test]
        public void ItShouldRecreateStructure()
        {
            var structureName = GenerateUniqName();

            var testStructure = Database.Structures.New(structureName);
            testStructure.Members.Add(new ida_struct_member { Name = "Member1" });
            Database.Store(testStructure);

            testStructure = Database.Structures.New(structureName);
            Assert.That(testStructure.Members.Exists(m => m.Name == "Member1"), Is.False);
            testStructure.Members.Add(new ida_struct_member { Name = "Member2" });
            Database.Store(testStructure);

            Reconnect();

            testStructure = Database.Structures.First(s => s.Name == structureName);
            Assert.That(testStructure.Members.Exists(m => m.Name == "Member1"), Is.False);
            Assert.That(testStructure.Members.Exists(m => m.Name == "Member2"), Is.True);            
        }
    }
}
