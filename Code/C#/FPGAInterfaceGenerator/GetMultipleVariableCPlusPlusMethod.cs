namespace FPGAInterfaceGenerator
{
    public class GetMultipleVariableCPlusPlusMethod
    {
        public string Name { get; }
        public Output[] Outputs { get; }
        public GetMultipleVariableCPlusPlusMethod(string name, params Output[] outputs)
        {
            Name = name;
            Outputs = outputs;
        }
    }
}
