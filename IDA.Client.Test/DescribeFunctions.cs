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

            var initInstance = Database.Functions.FindByName("InitInstance");
            Assert.That(initInstance, Is.Not.Null);
            Assert.That(initInstance.StartAddress, Is.EqualTo(0x00411580));
            Assert.That(initInstance.EndAddress, Is.EqualTo(0x0041162E));
            Assert.That(initInstance.Type, Is.EqualTo("int __cdecl(HINSTANCE hInstance, int nCmdShow)"));            
        }

        [Test]
        public void ItShouldGuessFunctionType()
        {
            var checkEsp = Database.Functions.FindByName("_RTC_CheckEsp");
            Assert.That(checkEsp, Is.Not.Null);            
            Assert.That(checkEsp.Type, Is.EqualTo("int()"));
        }
    }
}
