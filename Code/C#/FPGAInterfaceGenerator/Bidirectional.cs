namespace FPGAInterfaceGenerator
{
    public class Bidirectional:IOVariable
    {
        public Bidirectional(string name, VariableType variableType) :
            base(name, Direction.Bidirectional, variableType)
        { 
            
        }
    }
}
