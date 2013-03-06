using System;
using System.Diagnostics;
using System.Linq;
using NUnit.Framework;
using Ida.Client;

namespace IDA.Client.Test
{
    [TestFixture]
    class DescribeEnumerations
    {
        private const string DatabaseName = @"d:\games\WoWExt\Wow_5.1.0_16357.idb";
        private Ida.Client.Database _oldDatabaseAPI;
        private IDA.Client.Database _database;

        [SetUp]
        public void Connect()
        {
            Ida.Client.Database.IdaHome = @"d:\soft\ida61";
            _oldDatabaseAPI = Ida.Client.Database.Open(DatabaseName);
            _database = IDA.Client.Database.Open(DatabaseName);
            Assume.That(_oldDatabaseAPI, Is.Not.Null);
        }

        void Reconnect()
        {
            if (_oldDatabaseAPI != null)
            {
                _oldDatabaseAPI.Dispose();
            }
            if (_database != null)
            {
                _database.Dispose();                
            }
            _oldDatabaseAPI = Ida.Client.Database.Open(DatabaseName);
            _database = IDA.Client.Database.Open(DatabaseName);
        }

        [TearDown]
        public void ReleaseConnection()
        {
            if (_oldDatabaseAPI != null)
            {
                _oldDatabaseAPI.Dispose();
            }
            if (_database != null)
            {
                _database.Dispose();
            }
        }

        [Test]
        public void ItShouldGetEnumerationsList()
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
                var @enum = _oldDatabaseAPI.Enumerations.New(GenerateUUID());
                @enum.Constants.Add(new EnumerationConstant { Name = GenerateUUID(), Value = 1 });
                @enum.Constants.Add(new EnumerationConstant { Name = GenerateUUID(), Value = 2 });
                @enum.Constants.Add(new EnumerationConstant { Name = GenerateUUID(), Value = 3 });
                _oldDatabaseAPI.Enumerations.Persist(@enum, true);
            }
            _oldDatabaseAPI.Wait();
        }
        
        [Test]
        public void ItShouldCreateEnumeration()
        {            
            var testEnumeration = _oldDatabaseAPI.Enumerations.FirstOrDefault(e => e.Name == "Test");
            if (testEnumeration != null)
            {
                _oldDatabaseAPI.Enumerations.Delete(testEnumeration);                
            }

            testEnumeration = _oldDatabaseAPI.Enumerations.New("Test");
            testEnumeration.Constants.Add(new EnumerationConstant { Name = "Test1", Value = 1 });
            Assert.That(_oldDatabaseAPI.Enumerations, Has.Member(testEnumeration));
            Assert.That(_oldDatabaseAPI.Enumerations.Persist(testEnumeration), Is.True);
            
            Reconnect();

            testEnumeration = _oldDatabaseAPI.Enumerations.FirstOrDefault(e => e.Name == "Test");
            Assert.That(testEnumeration, Is.Not.Null);
            Debug.Assert(testEnumeration != null, "testEnumeration != null");
            Assert.That(testEnumeration.Constants.FirstOrDefault(c => c.Name == "Test1"), Is.Not.Null);
            Assert.That(testEnumeration.Constants.FirstOrDefault(c => c.Name == "Test2"), Is.Null);
            
            testEnumeration = _oldDatabaseAPI.Enumerations.New("Test");
            testEnumeration.Constants.Add(new EnumerationConstant { Name = "Test2", Value = 2 });
            Assert.That(_oldDatabaseAPI.Enumerations.Persist(testEnumeration), Is.True);

            Reconnect();

            testEnumeration = _oldDatabaseAPI.Enumerations.FirstOrDefault(e => e.Name == "Test");
            Debug.Assert(testEnumeration != null, "testEnumeration != null");
            Assert.That(testEnumeration.Constants.FirstOrDefault(c => c.Name == "Test1"), Is.Null);
            Assert.That(testEnumeration.Constants.FirstOrDefault(c => c.Name == "Test2"), Is.Not.Null);
            Assert.That(testEnumeration, Is.Not.Null);
        }
    }
}
