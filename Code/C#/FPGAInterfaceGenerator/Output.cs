namespace FPGAInterfaceGenerator
{
    public class Output:IOVariable
    {
        public Output(string name, VariableType variableType) :
            base(name, Direction.Output, variableType)
        { 
            
        }
    }
}
