using System.IO;

namespace Ida.Client
{
    public interface IExportable
    {
        void SaveTo(Stream output);
        void LoadFrom(Stream input);
    }

    public static class ExportableMethods
    {
        public static void SaveToFile(this IExportable @this, string filePath)
        {
            using (var output = new FileStream(filePath, FileMode.Create, FileAccess.Write))
            {
                @this.SaveTo(output);
            }
        }

        public static void LoadFromFile(this IExportable @this, string filePath)
        {
            using (var input = new FileStream(filePath, FileMode.Open, FileAccess.Read))
            {
                @this.LoadFrom(input);
            }
        }
    }
}