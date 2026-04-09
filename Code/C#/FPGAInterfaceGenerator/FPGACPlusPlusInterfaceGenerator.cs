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
            if (setup.Singleton) {
                sbHpp.AppendLine("#include \"Core/SingletonBase.hpp\"");
                sbCpp.AppendLine("#include \"Macros/GetFileName.hpp\"");
            }
            sbHpp.Append("class ");
            sbHpp.Append(className);
            if(setup.Singleton)
            {
                sbHpp.Append(" final : public SingletonBase<");
                sbHpp.Append(className);
                sbHpp.Append(">");
            }
            sbHpp.AppendLine(" {");
            if (setup.Singleton)
            {
                sbHpp.AppendLine("public:");
                sbHpp.AppendLine("    static const char* getTag();");
            }
            sbHpp.AppendLine("private:");
            if (setup.Singleton)
            {
                sbHpp.Append("    friend class SingletonBase<");
                sbHpp.Append(className);
                sbHpp.AppendLine(">;");
            }
            sbHpp.AppendLine("    FPGAInterface _fpgaInterface;");
            sbCpp.Append("#include \"");
            sbCpp.Append(className);
            sbCpp.AppendLine(".hpp\"");
            sbCpp.AppendLine("#include \"Timing/TimeHelper.hpp\"");
            sbCpp.Append("const char* ");
            sbCpp.Append(className);
            sbCpp.AppendLine("::getTag() {return GET_FILE_NAME;}");
            var appendInput = Create_AppendInput(className, sbInputsHpp, sbInputsCpp,
                out Func<int> getInputsLength);
            foreach (var input in setup.Inputs)
            {
                appendInput(input);
            }
            var appendOutput = Create_AppendOutput(className, sbOutputsHpp, sbOutputsCpp,
                getInputsLength(),
                out Func<int> getOutputsLength,
                out Func<string, int> getOutputFullIndex);
            foreach (var output in setup.Outputs)
            {
                appendOutput(output);
            }
            sbHpp.AppendLine("public:");
            CreateConstructor(className, sbHpp, sbCpp,
                getInputsLength(), getOutputsLength(), setup.SleepPeriodUs);

            sbHpp.AppendLine("    uint64_t getLastUpdateTimeUs();");
            sbCpp.Append("uint64_t ");
            sbCpp.Append(className);
            sbCpp.AppendLine("::getLastUpdateTimeUs(){ return _fpgaInterface.getLastUpdateTimeUs();}");
            sbHpp.Append(sbInputsHpp);
            sbCpp.Append(sbInputsCpp);
            sbHpp.Append(sbOutputsHpp);
            sbCpp.Append(sbOutputsCpp);

            // Generate GetMultiple methods if any
            if (setup.GetMultipleVariableCPlusPlusMethods != null)
            {
                foreach (var method in setup.GetMultipleVariableCPlusPlusMethods)
                {
                    AppendGetMultipleMethod(className, method, getOutputFullIndex, sbHpp, sbCpp);
                }
            }

            sbHpp.AppendLine("};");
            File.Delete(cppFilePath);
            File.WriteAllText(cppFilePath, sbCpp.ToString());
            File.Delete(hppFilePath);
            File.WriteAllText(hppFilePath, sbHpp.ToString());
        }

        private static void AppendGetMultipleMethod(
            string className,
            GetMultipleVariableCPlusPlusMethod method,
            Func<string, int> getOutputFullIndex,
            StringBuilder sbHpp,
            StringBuilder sbCpp)
        {
            // Build parameter list
            var parameters = method.Outputs
                .Select(o => $"{GetTypeName(o.VariableType)}& {StringHelper.LowerCamelCase(o.Name)}")
                .ToList();
            string paramList = string.Join(", ", parameters);

            // hpp declaration
            sbHpp.Append("    void ");
            sbHpp.Append(method.Name);
            sbHpp.Append("(");
            sbHpp.Append(paramList);
            sbHpp.AppendLine(");");

            // cpp definition
            sbCpp.AppendLine($"void {className}::{method.Name}({paramList}){{");
            sbCpp.AppendLine("    _fpgaInterface.usingLocked([&](LockedFPGAInterface locked){");
            foreach (var output in method.Outputs)
            {
                string varName = StringHelper.LowerCamelCase(output.Name);
                string getMethod = GetLockedGetMethodName(output.VariableType);
                int index = getOutputFullIndex(output.Name);
                sbCpp.AppendLine($"        {varName} = locked.{getMethod}({index});");
            }
            sbCpp.AppendLine("    });");
            sbCpp.AppendLine("}");
        }

        private static void CreateConstructor(
            string className,
            StringBuilder sbHpp,
            StringBuilder sbCpp,
            int inputsLength,
            int outputsLength,
            int sleepPeriodUs)
        {
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
            sbCpp.Append(", fpgaBus, ");
            sbCpp.Append(sleepPeriodUs);
            sbCpp.Append("){");
            sbCpp.AppendLine("}");
        }

        private static Action<Output> Create_AppendOutput(
            string className,
            StringBuilder sbHpp,
            StringBuilder sbCpp,
            int inputsLength,
            out Func<int> getOutputsLength,
            out Func<string, int> getOutputFullIndex)
        {
            int nextOutputIndex = 0;
            var outputIndices = new Dictionary<string, int>();
            getOutputsLength = () => nextOutputIndex;
            getOutputFullIndex = (name) => outputIndices[name];
            return (output) =>
            {
                int fullOutputsIndex = inputsLength + nextOutputIndex;
                outputIndices[output.Name] = fullOutputsIndex;
                if (output.VariableType.Equals(VariableType.CustomLengthBits)
                || output.VariableType.Equals(VariableType.CustomLengthBytes))
                {
                    AppendOutputArray(sbHpp, sbCpp, output, inputsLength,
                        ref nextOutputIndex, className, fullOutputsIndex);
                    return;
                }
                AppendOutputNonArray(
                    sbHpp,  sbCpp,  output,
                    inputsLength, ref nextOutputIndex,  
                    className,  fullOutputsIndex
                );
            };
        }
        private static void AppendOutputNonArray(
            StringBuilder sbHpp, StringBuilder sbCpp, Output output,
            int inputsLength, ref int nextOutputIndex, string className, 
            int fullOutputsIndex) {

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
            sbCpp.Append(fullOutputsIndex.ToString());
            sbCpp.AppendLine(");");
            sbCpp.AppendLine("}");
            IncrementIndexForType(ref nextOutputIndex, output.VariableType, null);
        }
        private static void AppendOutputArray(
          StringBuilder sbHpp, StringBuilder sbCpp, Output output,
          int inputsLength, ref int nextOutputIndex, string className,
          int fullOutputsIndex)
        {
                string returnTypeName = GetTypeName(output.VariableType);

                string methodName = $"get{StringHelper.UpperCamelCase(output.Name)}";

                sbHpp.Append("    void  ");
                sbHpp.Append(methodName);
                sbHpp.Append("(");
                sbHpp.Append(returnTypeName);
                sbHpp.Append(" (&value)[");
                sbHpp.Append(output.CustomLength);
                sbHpp.Append("]");
                sbHpp.AppendLine(");");

                sbCpp.Append("void ");
                sbCpp.Append(className);
                sbCpp.Append("::");
                sbCpp.Append(methodName);
                sbCpp.Append("(");
                sbCpp.Append(returnTypeName);
                sbCpp.Append(" (&value)[");
                sbCpp.Append(output.CustomLength);
                sbCpp.Append("]");
                sbCpp.AppendLine("){");
                sbCpp.Append("    _fpgaInterface.");
                sbCpp.Append(GetGetMethodName(output.VariableType));
                sbCpp.Append("(");
                sbCpp.Append(fullOutputsIndex.ToString());
                sbCpp.Append(", value, ");
                sbCpp.Append(output.CustomLength);
                sbCpp.AppendLine(");");
                sbCpp.AppendLine("}");

                IncrementIndexForType(ref nextOutputIndex, output.VariableType, output.CustomLength);
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
                if (input.VariableType.Equals(VariableType.CustomLengthBits)|| input.VariableType.Equals(VariableType.CustomLengthBytes)) {
                    AppendInputArray(sbCpp, sbHpp, className, input, ref nextInputIndex);
                    return;
                }
                AppendInputNonArray(sbCpp,  sbHpp, className, input, ref nextInputIndex);
            };
        }
        private static void AppendInputNonArray(StringBuilder sbCpp, StringBuilder sbHpp, string className, Input input, ref int nextInputIndex) {

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
            IncrementIndexForType(ref nextInputIndex, input.VariableType, null);
        }
        private static void AppendInputArray(StringBuilder sbCpp, StringBuilder sbHpp, string className, Input input, ref int nextInputIndex)
        {

            sbCpp.Append("void ");
            sbCpp.Append(className);
            sbCpp.Append("::");
            string methodName = $"set{StringHelper.UpperCamelCase(input.Name)}";
            sbCpp.Append(methodName);
            string valueTypeName = GetTypeName(input.VariableType);
            sbCpp.Append("(");
            sbCpp.Append(valueTypeName);
            sbCpp.Append(" (&value)[");
            sbCpp.Append(input.CustomLength!.Value);
            sbCpp.AppendLine("]){");
            sbCpp.Append("     return _fpgaInterface.");
            sbCpp.Append(GetSetMethodName(input.VariableType));
            sbCpp.Append("(");
            sbCpp.Append(nextInputIndex.ToString());
            sbCpp.Append(", value, ");
            sbCpp.Append(input.CustomLength!.Value);
            sbCpp.AppendLine(");");
            sbCpp.AppendLine("}");

            sbHpp.Append("    void ");
            sbHpp.Append(methodName);
            sbHpp.Append("(");
            sbHpp.Append(valueTypeName);
            sbHpp.Append(" (&value)[");
            sbHpp.Append(input.CustomLength!.Value);
            sbHpp.AppendLine("]);");
            IncrementIndexForType(ref nextInputIndex, input.VariableType, input.CustomLength);
        }

        private static void IncrementIndexForType(ref int index, VariableType variableType, int? customLength)
        {
            switch (variableType)
            {
                case VariableType.Bit:
                    index++;
                    return;
                case VariableType.Byte:
                    index += 8;
                    return;
                case VariableType.UInt16:
                    index += 16;
                    return;
                case VariableType.CustomLengthBytes:
                    if (!customLength.HasValue) throw new NullReferenceException();
                    index += (customLength!.Value * 8);
                    return;
                case VariableType.CustomLengthBits:
                    if (!customLength.HasValue) throw new NullReferenceException();
                    index += customLength!.Value;
                    return;
                default:
                    throw new NotImplementedException();
            }
        }

        private static string GetTypeName(VariableType variableType)
        {
            switch (variableType)
            {
                case VariableType.Bit:
                    return "bool";
                case VariableType.Byte:
                    return "uint8_t";
                case VariableType.UInt16:
                    return "uint16_t";
                case VariableType.CustomLengthBits:
                    return "bool";
                case VariableType.CustomLengthBytes:
                    return "uint8_t";
                default:
                    throw new NotImplementedException();
            }
        }

        private static string GetGetMethodName(VariableType variableType)
        {
            switch (variableType)
            {
                case VariableType.Bit:
                    return "getBit";
                case VariableType.Byte:
                    return "getByte";
                case VariableType.UInt16:
                    return "getUInt16";
                case VariableType.CustomLengthBits:
                    return "getBoolArray";
                case VariableType.CustomLengthBytes:
                    return "getByteArray";
                default:
                    throw new NotImplementedException();
            }
        }

        private static string GetLockedGetMethodName(VariableType variableType)
        {
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
                case VariableType.CustomLengthBits:
                    return "setBoolArray";
                case VariableType.CustomLengthBytes:
                    return "setByteArray";
                default:
                    throw new NotImplementedException();
            }
        }
    }
}
