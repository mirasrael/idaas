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
             Assert.That(strings["!This program cannot be run in DOS mode"], Is.Not.Null);
         }
    }
}