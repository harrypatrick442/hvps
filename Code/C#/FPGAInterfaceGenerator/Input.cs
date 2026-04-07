namespace FPGAInterfaceGenerator
{
    public class Input:IOVariable
    {
        public Input(string name, VariableType variableType, int? customLength = null) :
            base(name, Direction.Input, variableType, customLength)
        { 
            
        }
    }
}
