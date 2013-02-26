using System.Linq;
using NUnit.Framework;

namespace IDA.Client.Test
{
    [TestFixture]
    public class DescribeEndpoint
    {
        [Test]
        public void ItShouldConnectToIdaDatabase()
        {
            Ida.Client.Database.IdaHome = @"d:\soft\ida61";
            using (var database = Ida.Client.Database.Open(@"d:\games\WoWExt\Wow_5.1.0_16357.idb"))
            {
                Assert.That(database, Is.Not.Null);
                Assert.That(database.Enumerations.Count(), Is.EqualTo(1));                
                Assert.That(database.Functions.Count(), Is.EqualTo(4));
                Assert.That(database.Functions.Count(), Is.EqualTo(100));
            }
        }
    }
}
