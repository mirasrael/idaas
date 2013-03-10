﻿using NUnit.Framework;

namespace Ida.Client.Test
{
    public class DatabaseSpec
    {
        private const string DatabaseName = @"d:\games\WoWExt\Wow_5.1.0_16357.idb";
        protected Database Database;

        [SetUp]
        public void Connect()
        {
            Database.IdaHome = @"d:\soft\ida61";
            Database = Database.Open(DatabaseName);
            Assume.That(Database, Is.Not.Null);
        }

        protected void Reconnect()
        {
            if (Database != null)
            {
                Database.Dispose();
            }
            Database = Database.Open(DatabaseName);
        }

        [TearDown]
        public void ReleaseConnection()
        {
            if (Database != null)
            {
                Database.Dispose();
            }
        }
    }
}