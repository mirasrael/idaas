using System;
using System.Diagnostics;
using System.Linq;
using Idaas;
using NUnit.Framework;

namespace Ida.Client.Test
{
    [TestFixture]
    internal class DescribeEnumerations : DatabaseSpec
    {
        private string GenerateUUID()
        {
            return "_" + Guid.NewGuid().ToString("N");
        }

        [Test]
        public void ItShouldBatchCreateEnumerations()
        {
            int batchSize = 1000;
            int constantsCount = 3;
            int initialEnumsCount = Database.Enumerations.Count();
            int initialConstantsCount = Database.Enumerations.Aggregate(0, (cnt, e) => cnt + e.Constants.Count);
            DateTime start = DateTime.Now;
            for (int i = 0; i < batchSize; i++)
            {
                ida_enum @enum = Database.Enumerations.New(GenerateUUID());
                for (int contantIndex = 0; contantIndex < constantsCount; contantIndex++)
                {
                    @enum.Constants.Add(new ida_enum_const {Name = GenerateUUID(), Value = contantIndex});
                }
                Database.Enumerations.Store(@enum);
            }
            Console.WriteLine("Time gained: {0} ms", (DateTime.Now - start).TotalMilliseconds);
            Database.Wait();
            Reconnect();
            Assert.That(Database.Enumerations.Count(), Is.EqualTo(initialEnumsCount + batchSize));
            Assert.That(Database.Enumerations.Aggregate(0, (cnt, e) => cnt + e.Constants.Count),
                        Is.EqualTo(initialConstantsCount + batchSize*constantsCount));
        }

        [Test]
        public void ItShouldCreateEnumeration()
        {
            ida_enum testEnumeration = Database.Enumerations.FirstOrDefault(e => e.Name == "Test");
            if (testEnumeration != null)
            {
                Database.Enumerations.Delete(testEnumeration);
            }

            testEnumeration = Database.Enumerations.New("Test");
            testEnumeration.Constants.Add(new ida_enum_const {Name = "Test1", Value = 1});
            Assert.That(Database.Enumerations, Has.Member(testEnumeration));
            Assert.That(Database.Enumerations.Store(testEnumeration), Is.True);

            Reconnect();

            testEnumeration = Database.Enumerations.FirstOrDefault(e => e.Name == "Test");
            Assert.That(testEnumeration, Is.Not.Null);
            Debug.Assert(testEnumeration != null, "testEnumeration != null");
            Assert.That(testEnumeration.Constants.FirstOrDefault(c => c.Name == "Test1"), Is.Not.Null);
            Assert.That(testEnumeration.Constants.FirstOrDefault(c => c.Name == "Test2"), Is.Null);

            testEnumeration = Database.Enumerations.New("Test");
            testEnumeration.Constants.Add(new ida_enum_const {Name = "Test2", Value = 2});
            Assert.That(Database.Enumerations.Store(testEnumeration), Is.True);

            Reconnect();

            testEnumeration = Database.Enumerations.FirstOrDefault(e => e.Name == "Test");
            Debug.Assert(testEnumeration != null, "testEnumeration != null");
            Assert.That(testEnumeration.Constants.FirstOrDefault(c => c.Name == "Test1"), Is.Null);
            Assert.That(testEnumeration.Constants.FirstOrDefault(c => c.Name == "Test2"), Is.Not.Null);
            Assert.That(testEnumeration, Is.Not.Null);
        }

        [Test]
        public void ItShouldGetEnumerationsList()
        {
            Assert.That(Database, Is.Not.Null);
            ida_enum internetScheme = Database.Enumerations.First(e => e.Name == "INTERNET_SCHEME");
            Assert.That(internetScheme.Name, Is.Not.Null);
            Assert.That(internetScheme.IsBitfield, Is.False);
            ida_enum_const defaultScheme = internetScheme.Constants.Find(c => c.Name == "INTERNET_SCHEME_DEFAULT");
            Assert.That(defaultScheme, Is.Not.Null);
            ida_enum_const partialScheme = internetScheme.Constants.Find(c => c.Name == "INTERNET_SCHEME_PARTIAL");
            Assert.That(partialScheme, Is.Not.Null);
            ida_enum_const unknownScheme = internetScheme.Constants.Find(c => c.Name == "INTERNET_SCHEME_UNKNOWN");
            Assert.That(unknownScheme, Is.Not.Null);
        }
    }
}