using System.Reflection.Metadata;

namespace FPGAConstantsGenerator
{
    public class Constant
    {
        public string Name { get; }
        public int Value { get; }
        public int? NBits { get; }
        public Format WriteFormat{ get; }
        public Constant(string name, int value, Format writeFormat, int? nBits = null) { 
            Name = name;
            Value = value;
            NBits = nBits;
            WriteFormat = writeFormat;
            if ((!writeFormat.Equals(Format.RawInteger)) && (!nBits.HasValue))
            {
                throw new ArgumentNullException($"{nameof(nBits)} was null for {nameof(name)} \"{name}\"");
            }
        }
    }
}
