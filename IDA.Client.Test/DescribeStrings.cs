using NUnit.Framework;

namespace Ida.Client.Test
{
    [TestFixture]
    public class DescribeStrings : DatabaseSpec
    {
         [Test]
         public void ItShouldGetListOfStrings()
         {
             var strings = Database.Strings;
             Assert.That(strings["Local variable used before initialization"], Is.Not.Null);
         }
    }
}