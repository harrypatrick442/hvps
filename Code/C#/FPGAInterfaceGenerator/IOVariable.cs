namespace FPGAInterfaceGenerator
{
    public class IOVariable
    {
        public string Name { get; }
        public Direction Direction { get; }

        public VariableType VariableType { get; }
        public IOVariable(string name, Direction direction, VariableType variableType) { 
            Name = name;
            Direction = direction;
            VariableType = variableType;
        } 
    }
}
