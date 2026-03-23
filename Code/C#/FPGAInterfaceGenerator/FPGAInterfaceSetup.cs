namespace FPGAInterfaceGenerator
{
    public class FPGAInterfaceSetup
    {
        public string Name { get; }
        public IOVariable[] AllVariables{get;}
        public Input[] Inputs { get;}
        public Output[] Outputs { get;}
        public Bidirectional[] Bidirectionals { get;}

        public FPGAInterfaceSetup(string name, params IOVariable[] variables)
        {
            Name = name;
            AllVariables = variables;
            Inputs = variables.Where(v => v.Direction == Direction.Input)
                .Cast<Input>().ToArray();
            Outputs = variables.Where(v => v.Direction == Direction.Output)
                .Cast<Output>().ToArray();
            Bidirectionals = variables.Where(v => v.Direction == Direction.Bidirectional)
                .Cast<Bidirectional>().ToArray();
        }
    }
}
