using NUnit.Framework;

namespace IDA.Client.Test
{    
    [TestFixture]
    public class DescribeFunctions
    {        
        [Test]
        public void ItShouldReturnFunctionsList()
        {
            var database = Database.Open(@"d:\games\WoWExt\Wow-4.1.0-13914.idb");
            //Assert.That(database.Functions.Count(), Is.EqualTo(100));
        }
    }
}
