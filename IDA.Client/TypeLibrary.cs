using System.Collections.Generic;
using Idaas;
using System.Linq;

namespace Ida.Client
{
    public class TypeLibraries
    {
        private readonly Idaas.Database.Client _client;
        private IList<TypeLibrary> _items;

        public TypeLibraries(Idaas.Database.Client client)
        {
            _client = client;
        }

        public IEnumerable<TypeLibrary> Items
        {
            get { return _items ?? (_items = Load()); }
        }

        public TypeLibrary this[string name]
        {
            get { return Items.First(lib => lib.Name == name); }
        }

        private IList<TypeLibrary> Load()
        {
            var idaLibs = _client.listTypeLibraries();
            var libs = idaLibs.Select(lib => new TypeLibrary { Name = lib.Name, Types = lib.Types }).ToList();
            foreach (var lib in libs)
            {
                var baseLibraryNames = idaLibs.Find(l => l.Name == lib.Name).BaseLibraries;
                lib.BaseLibraries = baseLibraryNames.Select(baseLibName => libs.First(l => l.Name == baseLibName));
            }
            return libs;
        }
    }

    public class TypeLibrary
    {
        public string Name { get; internal set; }
        public IEnumerable<IdaTypeInfo> Types { get; internal set; }
        public IEnumerable<TypeLibrary> BaseLibraries { get; internal set; }

        public IEnumerable<IdaTypeInfo> GetTypes(bool inherited = false)
        {
            foreach (var type in Types)
            {
                yield return type;
            }
            if (!inherited) yield break;
            foreach (var type in BaseLibraries.SelectMany(baseLib => baseLib.GetTypes()))
            {
                yield return type;
            }
        }
    }
}