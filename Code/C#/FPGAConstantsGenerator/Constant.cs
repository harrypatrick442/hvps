namespace FPGAConstantsGenerator
{
    public class Constant
    {
        public string Name { get; }
        public int Value { get; }
        public int NBits { get; }
        public Format WriteFormat{ get; }
        public Constant(string name, int value, int nBits, Format writeFormat) { 
            Name = name;
            Value = value;
            NBits = nBits;
            WriteFormat = writeFormat;
        }
    }
}
