namespace FPGAInterfaceGenerator
{
    public class Input:IOVariable
    {
        public Input(string name, VariableType variableType) :
            base(name, Direction.Input, variableType)
        { 
            
        }
    }
}
