using System;
using System.Diagnostics;
using System.Linq;
using Ida.Client;
using NUnit.Framework;

namespace IDA.Client.Test
{
    [TestFixture]
    class DescribeEnumerations
    {
        private const string DatabaseName = @"d:\games\WoWExt\Wow_5.1.0_16357.idb";
        private Database _database;

        [SetUp]
        public void Connect()
        {
            Database.IdaHome = @"d:\soft\ida61";
            _database = Database.Open(DatabaseName);
            Assume.That(_database, Is.Not.Null);
        }

        void Reconnect()
        {
            if (_database != null)
            {
                _database.Dispose();
            }
            _database = Database.Open(DatabaseName);
        }

        [TearDown]
        public void ReleaseConnection()
        {
            if (_database != null)
            {
                _database.Dispose();
            }
        }

        [Test]
        public void ItShouldEnumEnumerations()
        {            
            Assert.That(_database, Is.Not.Null);            
            var internetScheme = _database.Enumerations.First(e => e.Name == "INTERNET_SCHEME");
            Assert.That(internetScheme.Name, Is.Not.Null);
            Assert.That(internetScheme.IsBitfield, Is.False);
            var defaultScheme = internetScheme.Constants.Find(c => c.Name == "INTERNET_SCHEME_DEFAULT");
            Assert.That(defaultScheme, Is.Not.Null);
            var partialScheme = internetScheme.Constants.Find(c => c.Name == "INTERNET_SCHEME_PARTIAL");
            Assert.That(partialScheme, Is.Not.Null);
            var unknownScheme = internetScheme.Constants.Find(c => c.Name == "INTERNET_SCHEME_UNKNOWN");
            Assert.That(unknownScheme, Is.Not.Null);                            
        }

        private string GenerateUUID()
        {
            return "_" + Guid.NewGuid().ToString("N");
        }

        [Test]
        public void ItShouldBatchCreateEnumerations()
        {
            for (var i = 0; i < 100; i++)
            {
                var @enum = _database.Enumerations.New(GenerateUUID());
                @enum.Constants.Add(new EnumerationConstant { Name = GenerateUUID(), Value = 1 });
                @enum.Constants.Add(new EnumerationConstant { Name = GenerateUUID(), Value = 2 });
                @enum.Constants.Add(new EnumerationConstant { Name = GenerateUUID(), Value = 3 });
                _database.Enumerations.Persist(@enum, true);
            }
            _database.Wait();
        }

        [Test]
        public void ItShouldCreateEnumeration()
        {            
            var testEnumeration = _database.Enumerations.FirstOrDefault(e => e.Name == "Test");
            if (testEnumeration != null)
            {
                _database.Enumerations.Delete(testEnumeration);                
            }

            testEnumeration = _database.Enumerations.New("Test");
            testEnumeration.Constants.Add(new EnumerationConstant { Name = "Test1", Value = 1 });
            Assert.That(_database.Enumerations, Has.Member(testEnumeration));
            Assert.That(_database.Enumerations.Persist(testEnumeration), Is.True);
            
            Reconnect();

            testEnumeration = _database.Enumerations.FirstOrDefault(e => e.Name == "Test");
            Assert.That(testEnumeration, Is.Not.Null);
            Debug.Assert(testEnumeration != null, "testEnumeration != null");
            Assert.That(testEnumeration.Constants.FirstOrDefault(c => c.Name == "Test1"), Is.Not.Null);
            Assert.That(testEnumeration.Constants.FirstOrDefault(c => c.Name == "Test2"), Is.Null);
            
            testEnumeration = _database.Enumerations.New("Test");
            testEnumeration.Constants.Add(new EnumerationConstant { Name = "Test2", Value = 2 });
            Assert.That(_database.Enumerations.Persist(testEnumeration), Is.True);

            Reconnect();

            testEnumeration = _database.Enumerations.FirstOrDefault(e => e.Name == "Test");
            Debug.Assert(testEnumeration != null, "testEnumeration != null");
            Assert.That(testEnumeration.Constants.FirstOrDefault(c => c.Name == "Test1"), Is.Null);
            Assert.That(testEnumeration.Constants.FirstOrDefault(c => c.Name == "Test2"), Is.Not.Null);
            Assert.That(testEnumeration, Is.Not.Null);
        }
    }
}
