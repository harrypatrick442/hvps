using Core.Strings;
using System.Text;

namespace FPGAInterfaceGenerator
{
    public class FPGACPlusPlusInterfaceGenerator
    {
        public static void GenerateCPlusPlusInterface(
            FPGAInterfaceSetup setup,
            string outputDirectory)
        {
            string className = setup.Name;
            string hppFilePath = Path.Combine(outputDirectory, $"{className}.hpp");
            string cppFilePath = Path.Combine(outputDirectory, $"{className}.cpp");
            StringBuilder sbHpp = new StringBuilder();
            StringBuilder sbCpp = new StringBuilder();
            StringBuilder sbOutputsHpp = new StringBuilder();
            StringBuilder sbOutputsCpp = new StringBuilder();
            StringBuilder sbInputsHpp = new StringBuilder();
            StringBuilder sbInputsCpp = new StringBuilder();

            sbHpp.AppendLine("#pragma once");
            sbHpp.AppendLine("#include \"FPGA/FPGAInterface.hpp\"");
            sbHpp.AppendLine("#include \"FPGA/IFPGABus.hpp\"");
            sbHpp.Append("class ");
            sbHpp.Append(className);
            sbHpp.AppendLine(" {");
            sbHpp.AppendLine("private:");
            sbHpp.Append("    FPGAInterface _fpgaInterface;");
            sbHpp.AppendLine();
            sbCpp.Append("#include \"");
            sbCpp.Append(className);
            sbCpp.AppendLine(".hpp\"");
            var appendInput = Create_AppendInput(className, sbInputsHpp, sbInputsCpp,
                out Func<int> getInputsLength);
            foreach (var input in setup.Inputs)
            {
                appendInput(input);
            }
            var appendOutput = Create_AppendOutput(className, sbOutputsHpp, sbOutputsCpp,
                getInputsLength(),
                out Func<int> getOutputsLength);
            foreach (var output in setup.Outputs)
            {
                appendOutput(output);
            }
            CreateConstructor(className, sbHpp, sbCpp,
                getInputsLength(), getOutputsLength());
            sbHpp.Append(sbInputsHpp);
            sbCpp.Append(sbInputsCpp);
            sbHpp.Append(sbOutputsHpp);
            sbCpp.Append(sbOutputsCpp);
            sbHpp.AppendLine("};");
            File.Delete(cppFilePath);
            File.WriteAllText(cppFilePath, sbCpp.ToString());
            File.Delete(hppFilePath);
            File.WriteAllText(hppFilePath, sbHpp.ToString());
        }
        private static void CreateConstructor(
            string className,
            StringBuilder sbHpp,
            StringBuilder sbCpp,
            int inputsLength,
            int outputsLength)
        {
            sbHpp.AppendLine("public:");
            sbHpp.Append("    ");
            sbHpp.Append(className);
            sbHpp.AppendLine("(IFPGABus& fpgaBus);");
            sbCpp.Append(className);
            sbCpp.Append("::");
            sbCpp.Append(className);
            sbCpp.Append("(IFPGABus& fpgaBus):");
            sbCpp.Append("_fpgaInterface(");
            sbCpp.Append(inputsLength);
            sbCpp.Append(",");
            sbCpp.Append(outputsLength);
            sbCpp.Append(", fpgaBus){");
            sbCpp.AppendLine("}");
        }
        private static Action<Output> Create_AppendOutput(
            string className,
            StringBuilder sbHpp,
            StringBuilder sbCpp,
            int inputsLength,
            out Func<int> getOutputsLength) {
            int nextOutputIndex = 0;
            getOutputsLength = () => nextOutputIndex;
            return (output) =>
            {
                string returnTypeName = GetTypeName(output.VariableType);

                sbHpp.Append("    ");
                sbHpp.Append(returnTypeName);
                sbCpp.Append(returnTypeName);
                sbCpp.Append(" ");
                sbCpp.Append(className);
                string methodName = $"get{StringHelper.UpperCamelCase(output.Name)}";
                sbCpp.Append("::");
                sbCpp.Append(methodName);
                sbHpp.Append(" ");
                sbHpp.Append(methodName);
                sbHpp.AppendLine("();");
                sbCpp.AppendLine("(){");
                sbCpp.Append("     return _fpgaInterface.");
                sbCpp.Append(GetGetMethodName(output.VariableType));
                sbCpp.Append("(");
                int fullOutputsIndex = inputsLength + nextOutputIndex;
                sbCpp.Append(fullOutputsIndex.ToString());
                sbCpp.AppendLine(");");
                sbCpp.AppendLine("}");
                IncrementIndexForType(ref nextOutputIndex, output.VariableType);
            };
        }
        static Action<Input> Create_AppendInput(
            string className,
            StringBuilder sbHpp,
            StringBuilder sbCpp,
            out Func<int> getInputsLength)
        {
            int nextInputIndex = 0;
            getInputsLength = () => nextInputIndex;
            return (input) =>
            {
                sbCpp.Append("void ");
                sbHpp.Append("    void ");
                sbCpp.Append(className);
                sbCpp.Append("::");
                string methodName = $"set{StringHelper.UpperCamelCase(input.Name)}";
                sbCpp.Append(methodName);
                sbHpp.Append(methodName);
                string valueTypeName = GetTypeName(input.VariableType);
                sbCpp.Append("(");
                sbCpp.Append(valueTypeName);
                sbCpp.AppendLine(" value){");
                sbHpp.Append("(");
                sbHpp.Append(valueTypeName);
                sbHpp.AppendLine(" value);");
                sbCpp.Append("     return _fpgaInterface.");
                sbCpp.Append(GetSetMethodName(input.VariableType));
                sbCpp.Append("(");
                sbCpp.Append(nextInputIndex.ToString());
                sbCpp.Append(", value");
                sbCpp.AppendLine(");");
                sbCpp.AppendLine("}");
                IncrementIndexForType(ref nextInputIndex, input.VariableType);
            };
        }
        private static void IncrementIndexForType(ref int index, VariableType variableType) {
            switch (variableType) {
                case VariableType.Bit:
                    index++;
                    return;
                case VariableType.Byte:
                    index += 8;
                    return;
                case VariableType.UInt16:
                    index += 16;
                    return;
                default:
                    throw new NotImplementedException();
            }
        }
        private static string GetTypeName(VariableType variableType) { 
            switch(variableType)
            {
                case VariableType.Bit:
                    return "bool";
                case VariableType.Byte:
                    return "uint8_t";
                case VariableType.UInt16:
                    return "uint16_t";
                default:
                    throw new NotImplementedException();
            }
        }
        private static string GetGetMethodName(VariableType variableType) {

            switch (variableType)
            {
                case VariableType.Bit:
                    return "getBit";
                case VariableType.Byte:
                    return "getByte";
                case VariableType.UInt16:
                    return "getUInt16";
                default:
                    throw new NotImplementedException();
            }
        }
        private static string GetSetMethodName(VariableType variableType)
        {

            switch (variableType)
            {
                case VariableType.Bit:
                    return "setBit";
                case VariableType.Byte:
                    return "setByte";
                case VariableType.UInt16:
                    return "setUInt16";
                default:
                    throw new NotImplementedException();
            }
        }
    }
}
