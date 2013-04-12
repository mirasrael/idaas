using NUnit.Framework;
using System.Linq;

namespace Ida.Client.Test
{    
    [TestFixture]
    public class DescribeFunctions : DatabaseSpec
    {        
        [Test]
        public void ItShouldReturnFunctionsList()
        {
            Assert.That(Database.Functions.Count(), Is.GreaterThan(0));
            Assert.That(Database.Functions.First(function => function.Name == "_WinMain@16"), Is.Not.Null);
        }
    }
}
