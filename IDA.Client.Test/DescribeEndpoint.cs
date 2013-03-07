using NUnit.Framework;

namespace Ida.Client.Test
{
    [TestFixture]
    public class DescribeEndpoint
    {
        [Test]
        public void ItShouldConnectToIdaDatabase()
        {
            Database.IdaHome = @"d:\soft\ida61";
            using (var database = Database.Open(@"d:\games\WoWExt\Wow_5.1.0_16357.idb"))
            {
                Assert.That(database, Is.Not.Null);                
            }
        }
    }
}
