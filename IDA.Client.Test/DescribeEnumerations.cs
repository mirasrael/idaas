using System;
using System.Diagnostics;
using System.Linq;
using Idaas;
using NUnit.Framework;

namespace Ida.Client.Test
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
            var batchSize = 1000;
            var constantsCount = 3;
            var initialEnumsCount = _database.Enumerations.Count();
            var initialConstantsCount = _database.Enumerations.Aggregate(0, (cnt, e) => cnt + e.Constants.Count);
            var start = DateTime.Now;
            for (var i = 0; i < batchSize; i++)
            {
                var @enum = _database.Enumerations.New(GenerateUUID());
                for (var contantIndex = 0; contantIndex < constantsCount; contantIndex++)
                {
                    @enum.Constants.Add(new ida_enum_const { Name = GenerateUUID(), Value = contantIndex });
                }                                    
                _database.Enumerations.Store(@enum);
            }
            Console.WriteLine("Time gained: {0} ms", (DateTime.Now - start).TotalMilliseconds);
            _database.Wait();            
            Reconnect();
            Assert.That(_database.Enumerations.Count(), Is.EqualTo(initialEnumsCount + batchSize));
            Assert.That(_database.Enumerations.Aggregate(0, (cnt, e) => cnt + e.Constants.Count), Is.EqualTo(initialConstantsCount + batchSize * constantsCount));
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
            testEnumeration.Constants.Add(new ida_enum_const { Name = "Test1", Value = 1 });
            Assert.That(_database.Enumerations, Has.Member(testEnumeration));
            Assert.That(_database.Enumerations.Store(testEnumeration), Is.True);
            
            Reconnect();

            testEnumeration = _database.Enumerations.FirstOrDefault(e => e.Name == "Test");
            Assert.That(testEnumeration, Is.Not.Null);
            Debug.Assert(testEnumeration != null, "testEnumeration != null");
            Assert.That(testEnumeration.Constants.FirstOrDefault(c => c.Name == "Test1"), Is.Not.Null);
            Assert.That(testEnumeration.Constants.FirstOrDefault(c => c.Name == "Test2"), Is.Null);
            
            testEnumeration = _database.Enumerations.New("Test");
            testEnumeration.Constants.Add(new ida_enum_const { Name = "Test2", Value = 2 });
            Assert.That(_database.Enumerations.Store(testEnumeration), Is.True);

            Reconnect();

            testEnumeration = _database.Enumerations.FirstOrDefault(e => e.Name == "Test");
            Debug.Assert(testEnumeration != null, "testEnumeration != null");
            Assert.That(testEnumeration.Constants.FirstOrDefault(c => c.Name == "Test1"), Is.Null);
            Assert.That(testEnumeration.Constants.FirstOrDefault(c => c.Name == "Test2"), Is.Not.Null);
            Assert.That(testEnumeration, Is.Not.Null);
        }
    }
}
