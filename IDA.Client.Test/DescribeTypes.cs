using NUnit.Framework;

namespace Ida.Client.Test
{
    [TestFixture]
    public class DescribeTypes : DatabaseSpec
    {
        public void ItShouldParse(string declaration, string formatted = null)
        {
            Assert.That(Database.FormatTypeInfo(Database.ParseTypeDeclaration(declaration)), Is.EqualTo(formatted ?? declaration));
        }

        [Test]
        public void ItShouldParseFunctionDeclaration()
        {            
            ItShouldParse("void __cdecl()");            
            ItShouldParse("void __stdcall()");
            ItShouldParse("void()", "void __cdecl()");
            ItShouldParse("void(*)()", "void (__cdecl *)()");
            ItShouldParse("void(*)(void(*)())", "void (__cdecl *)(void (__cdecl *)())");
        }
    }
}