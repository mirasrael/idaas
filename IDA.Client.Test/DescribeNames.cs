using Idaas;
using NUnit.Framework;

namespace Ida.Client.Test
{
    [TestFixture]
    internal class DescribeNames : DatabaseSpec
    {
        [Test]
        public void ItShouldListNamedAddresses()
        {
            Assert.That(Database.Names, Is.Not.Empty);
            Assert.That(Database.Names, Has.Some.Matches<IdaNamedAddress>(n => n.Address == 0x00419138 && n.Name == "hInst" && n.Type == "HINSTANCE" && n.Segment == IdaSegmentType.Data));
            Assert.That(Database.Names, Has.Some.Matches<IdaNamedAddress>(n => n.Address == 0x00416C7C && n.Name == "aALocalVariable" && n.Type.StartsWith("char[") && n.Segment == IdaSegmentType.Data));
        }
    }
}