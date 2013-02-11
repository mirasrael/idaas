using System;
using System.Linq;
using NUnit.Framework;

namespace IDA.Client.Test
{
    [TestFixture]
    class DescribeEnumerations
    {
        [Test]
        public void ItShouldEnumEnumerations()
        {
            using (var database = Ida.Client.Database.Open(@"d:\games\WoWExt\Wow_5.1.0_16357.idb"))
            {
                Assert.That(database, Is.Not.Null);
                Assert.That(database.Enumerations.Count(), Is.EqualTo(1));
                var enumeration = database.Enumerations.First();
                Assert.That(enumeration.Name, Is.EqualTo("INTERNET_SCHEME"));
                Console.WriteLine(enumeration.Name);
                Assert.That(enumeration.IsBitfield, Is.False);
            }
        }
    }
}
