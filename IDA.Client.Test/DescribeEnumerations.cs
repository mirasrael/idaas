using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using Idaas;
using NUnit.Framework;

namespace Ida.Client.Test
{
    [TestFixture]
    internal class DescribeEnumerations : DatabaseSpec
    {        
        [Test]
        public void ItShouldBatchCreateEnumerations()
        {
            int batchSize = 1000;
            int constantsCount = 3;
            int initialEnumsCount = Database.Enumerations.Count();
            int initialConstantsCount = Database.Enumerations.Aggregate(0, (cnt, e) => cnt + e.Constants.Count);
            DateTime start = DateTime.Now;
            var enums = new List<ida_enum>();
            for (int i = 0; i < batchSize; i++)
            {
                ida_enum @enum = Database.Enumerations.New(GenerateUniqName());
                for (int contantIndex = 0; contantIndex < constantsCount; contantIndex++)
                {
                    @enum.Constants.Add(new ida_enum_const {Name = GenerateUniqName(), Value = contantIndex});
                }   
                enums.Add(@enum);
            }
            Assert.That(Database.Enumerations.Store(enums), Is.True);
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
            var enumName = GenerateUniqName();
            var testEnumeration = Database.Enumerations.New(enumName);
            testEnumeration.Constants.Add(new ida_enum_const { Name = enumName + "_1", Value = 1 });                        
            Assert.That(Database.Enumerations, Has.Member(testEnumeration));
            Assert.That(Database.Enumerations.Store(testEnumeration), Is.True);            

            Reconnect();

            testEnumeration = Database.Enumerations.FirstOrDefault(e => e.Name == enumName);
            Assert.That(testEnumeration, Is.Not.Null);
            Debug.Assert(testEnumeration != null, "testEnumeration != null");
            Assert.That(testEnumeration.Constants.FirstOrDefault(c => c.Name == enumName + "_1"), Is.Not.Null);
            Assert.That(testEnumeration.Constants.FirstOrDefault(c => c.Name == enumName + "_2"), Is.Null);

            testEnumeration = Database.Enumerations.New(enumName);
            testEnumeration.Constants.Add(new ida_enum_const { Name = enumName + "_2", Value = 2 });
            Assert.That(Database.Enumerations.Store(testEnumeration), Is.True);

            Reconnect();

            testEnumeration = Database.Enumerations.FirstOrDefault(e => e.Name == enumName);
            Debug.Assert(testEnumeration != null, "testEnumeration != null");
            Assert.That(testEnumeration.Constants.FirstOrDefault(c => c.Name == enumName + "_1"), Is.Null);
            Assert.That(testEnumeration.Constants.FirstOrDefault(c => c.Name == enumName + "_2"), Is.Not.Null);
            Assert.That(testEnumeration, Is.Not.Null);
        }

        [Test]
        public void ItShouldDeleteEnumeration()
        {
            var name = GenerateUniqName();
            var testEnum = Database.Enumerations.New(name);
            Database.Enumerations.Store(testEnum);
            Database.Enumerations.Delete(testEnum);
            Assert.That(Database.Enumerations, Has.No.Member(testEnum));

            Reconnect();

            Assert.That(Database.Enumerations, Has.No.Member(testEnum));
        }

        [Test]
        public void ItShouldGetEnumerationsList()
        {
            Assert.That(Database, Is.Not.Null);
            ida_enum testEnumeration = Database.Enumerations.First(e => e.Name == "TestEnumeration");
            Assert.That(testEnumeration.Name, Is.Not.Null);
            Assert.That(testEnumeration.IsBitfield, Is.False);
            ida_enum_const constant0 = testEnumeration.Constants.Find(c => c.Name == "TestConstant_0");
            Assert.That(constant0, Is.Not.Null);
            ida_enum_const constant1 = testEnumeration.Constants.Find(c => c.Name == "TestConstant_1");
            Assert.That(constant1, Is.Not.Null);            
        }        

        [Test]
        public void ItShouldExportAndImportEnumerations()
        {
            var @enum = Database.NewEnumeration(GenerateUniqName());
            @enum.AddConstant(@enum.Name + "_0", 0);

            var flagsEnum = Database.NewEnumeration(GenerateUniqName(), true);
            flagsEnum.AddConstant(flagsEnum.Name + "_1", 1);

            Database.Enumerations.Store(new[] {@enum, flagsEnum});

            var output = new MemoryStream();
            Database.Enumerations.SaveTo(output);
            output.Seek(0, SeekOrigin.Begin);
            Console.WriteLine(new StreamReader(output).ReadToEnd());
            output.Seek(0, SeekOrigin.Begin);

            Database.Enumerations.Delete(@enum);
            Database.Enumerations.Delete(flagsEnum);

            Reconnect();

            Database.Enumerations.LoadFrom(output);
            Assert.That(Database.Enumerations.Has(@enum.Name), Is.True);
            Assert.That(Database.Enumerations.Has(flagsEnum.Name), Is.True);
            Assert.That(Database.Enumerations[flagsEnum.Name].IsBitfield, Is.True);
            Assert.That(Database.Enumerations[flagsEnum.Name].Get(flagsEnum.Name + "_1").Value, Is.EqualTo(1));
        }
    }
}