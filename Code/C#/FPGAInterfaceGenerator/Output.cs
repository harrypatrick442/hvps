namespace FPGAInterfaceGenerator
{
    public class Output:IOVariable
    {
        public Output(string name, VariableType variableType, int? customLength = null) :
            base(name, Direction.Output, variableType, customLength)
        {
        }
    }
}
