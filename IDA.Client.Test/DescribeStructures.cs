using System.Linq;
using NUnit.Framework;

namespace Ida.Client.Test
{
    [TestFixture]
    public class DescribeStructures : DatabaseSpec
    {
        [Test]
        public void ItShouldGetStructuresList()
        {
            Assert.That(Database, Is.Not.Null);
            var devInfo = Database.Structures.First(s => s.Name == "_SP_DEVINFO_DATA");
            Assert.That(devInfo, Is.Not.Null);            
        }
    }
}
