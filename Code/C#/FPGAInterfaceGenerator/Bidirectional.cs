namespace FPGAInterfaceGenerator
{
    public class Bidirectional:IOVariable
    {
        public Bidirectional(string name, VariableType variableType, int? customLength) :
            base(name, Direction.Bidirectional, variableType, customLength)
        { 
            
        }
    }
}
