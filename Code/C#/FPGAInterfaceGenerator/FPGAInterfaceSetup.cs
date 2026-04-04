namespace FPGAInterfaceGenerator
{
    public class FPGAInterfaceSetup
    {
        public string Name { get; }
        public IOVariable[] AllVariables{get;}
        public Input[] Inputs { get;}
        public Output[] Outputs { get;}
        public Bidirectional[] Bidirectionals { get;}
        public int SleepPeriodMs { get; }
        public GetMultipleVariableCPlusPlusMethod[]? GetMultipleVariableCPlusPlusMethods { get; }

        public FPGAInterfaceSetup(
            string name, IOVariable[] variables,
            GetMultipleVariableCPlusPlusMethod[]? getMultipleVariableCPlusPlusMethods = null,
            int sleepPeriodMs = 10)
        {
            Name = name;
            AllVariables = variables;
            Inputs = variables.Where(v => v.Direction == Direction.Input)
                .Cast<Input>().ToArray();
            Outputs = variables.Where(v => v.Direction == Direction.Output)
                .Cast<Output>().ToArray();
            Bidirectionals = variables.Where(v => v.Direction == Direction.Bidirectional)
                .Cast<Bidirectional>().ToArray();
            GetMultipleVariableCPlusPlusMethods = getMultipleVariableCPlusPlusMethods;
            SleepPeriodMs = sleepPeriodMs;
        }
    }
}
