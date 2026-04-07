namespace FPGAInterfaceGenerator
{
    public class IOVariable
    {
        public string Name { get; }
        public Direction Direction { get; }

        public VariableType VariableType { get; }
        public int? CustomLength { get; }
        public IOVariable(string name, Direction direction, VariableType variableType, int? customLength) { 
            Name = name;
            Direction = direction;
            VariableType = variableType;
            CustomLength = customLength;
            CheckArguments(variableType, customLength);
        }

        private static void CheckArguments(VariableType variableType, int? customLength)
        {

            switch (variableType)
            {
                case VariableType.CustomLengthBytes:
                    if (customLength == null || customLength <= 0)
                    {
                        throw new ArgumentException($"When using {nameof(VariableType)}.{nameof(VariableType.CustomLengthBytes)} you must provide a non null >0 value for argument {nameof(customLength)}");
                    }
                    break;
                case VariableType.CustomLengthBits:
                    if (customLength == null || customLength <= 0)
                    {
                        throw new ArgumentException($"When using {nameof(VariableType)}.{nameof(VariableType.CustomLengthBits)} you must provide a non null >0 value for argument {nameof(customLength)}");
                    }
                    break;
                default:
                    if (customLength != null&&customLength!=0)
                    {
                        throw new ArgumentException($"You should not provide a {nameof(customLength)} for this {nameof(VariableType)}");
                    }
                    break;
            }
        }
    }
}
