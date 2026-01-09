using System.Text;
using System.Linq;
namespace ESP32EmbeddedElfFileGenerator
{
    public static class EmbeddedElfFileGenerator
    {
        public static void GenerateClass(string? nameSpace, string className, string filePath, params ElfFilePathAndIdentifier[] elfFilePathAndIdentifiers)
        {
            ValidateNoDuplicatedIdentifiers(elfFilePathAndIdentifiers);
            StringBuilder sb = new StringBuilder();
            if (nameSpace != null)
            {
                sb.Append("namespace ");
                sb.Append(nameSpace);
                sb.Append(" {");
            }
            sb.Append("public static class ");
            sb.Append(className);
            sb.AppendLine(" {");
            WriteStaticStrings(sb, elfFilePathAndIdentifiers);
            WriteGetFromIdentifier(sb, elfFilePathAndIdentifiers);
            sb.AppendLine("    }");

            if (nameSpace != null)
            {
                sb.Append("}");
            }
            File.WriteAllText(filePath, sb.ToString());
        }
        private static void WriteStaticStrings(StringBuilder sb, ElfFilePathAndIdentifier[] elfFilePathAndIdentifiers) {


            foreach (ElfFilePathAndIdentifier elfFilePathAndIdentifier in elfFilePathAndIdentifiers)
            {
                sb.Append($"    public const string ");
                sb.Append(elfFilePathAndIdentifier.Name);
                sb.Append(" \"");
                byte[] elfBytes = File.ReadAllBytes(elfFilePathAndIdentifier.Path);
                StringBuilder sbElf = ByteArrayToHexString(elfBytes);
                sb.Append(sbElf);
                sb.AppendLine(" \"");
            }
        }
        private static StringBuilder ByteArrayToHexString(byte[] data)
        {
            var sb = new StringBuilder(data.Length * 2);
            foreach (byte b in data)
                sb.Append(b.ToString("X2")); // uppercase hex, e.g., "AF"
            return sb;
        }
        private static void WriteGetFromIdentifier(StringBuilder sb, ElfFilePathAndIdentifier[] elfFilePathAndIdentifiers) {
            sb.AppendLine("public static string GetElfHexStringFromIdentifier(int identifier){");
            sb.AppendLine("switch(identifier){");
            foreach (ElfFilePathAndIdentifier elfFilePathAndIdentifier in elfFilePathAndIdentifiers) {

                sb.Append("case ");
                sb.Append(elfFilePathAndIdentifier.Identifier.ToString());
                sb.Append(": return ");
                sb.Append(elfFilePathAndIdentifier.Name);
                sb.AppendLine(";");
            }
            sb.AppendLine("default: throw new Exception($\"Not implemented for identifier {identifier}\"); ");
            sb.AppendLine("}");
        }
        private static void ValidateNoDuplicatedIdentifiers(ElfFilePathAndIdentifier[] entries)
        {
            var duplicates = entries
                .Select((e, idx) => new { e.Identifier, Index = idx })
                .GroupBy(e => e.Identifier)
                .Where(g => g.Count() > 1)
                .ToList();

            if (duplicates.Count == 0) return;

            var message = "Duplicated Identifiers:\n" + string.Join("\n", duplicates.Select(g =>
                $"Identifier: {g.Key} at indices: {string.Join(", ", g.Select(e => e.Index))}"));

            throw new Exception(message);
        }

    }
}