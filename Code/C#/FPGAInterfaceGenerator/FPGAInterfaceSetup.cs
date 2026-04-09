namespace FPGAInterfaceGenerator
{
    public class FPGAInterfaceSetup
    {
        public string Name { get; }
        public IOVariable[] AllVariables{get;}
        public Input[] Inputs { get;}
        public Output[] Outputs { get;}
        public Bidirectional[] Bidirectionals { get;}
        public int SleepPeriodUs { get; }
        public GetMultipleVariableCPlusPlusMethod[]? GetMultipleVariableCPlusPlusMethods { get; }
        public bool Singleton { get; }
        public FPGAInterfaceSetup(
            string name, IOVariable[] variables,
            GetMultipleVariableCPlusPlusMethod[]? getMultipleVariableCPlusPlusMethods = null,
            int sleepPeriodUs = 150/*was 50*/,
            bool singleton = true)
        {
            variables = variables.Reverse().ToArray();//This means most important first.
            Name = name;
            AllVariables = variables;
            Inputs = variables.Where(v => v.Direction == Direction.Input)
                .Cast<Input>().ToArray();
            Outputs = variables.Where(v => v.Direction == Direction.Output)
                .Cast<Output>().ToArray();
            Bidirectionals = variables.Where(v => v.Direction == Direction.Bidirectional)
                .Cast<Bidirectional>().ToArray();
            GetMultipleVariableCPlusPlusMethods = getMultipleVariableCPlusPlusMethods;
            SleepPeriodUs = sleepPeriodUs;
            Singleton = singleton;
        }
    }
}
