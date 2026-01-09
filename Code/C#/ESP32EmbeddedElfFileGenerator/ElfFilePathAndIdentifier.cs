namespace ESP32EmbeddedElfFileGenerator
{
    public struct ElfFilePathAndIdentifier
    {
        public string Name { get {
                return $"Elf_{Identifier}";
            } }
        public string Path { get; }
        public int Identifier { get; }
        public ElfFilePathAndIdentifier(string path, int identifier)
        {
            Path = path;
            Identifier = identifier;
        }
    }
}