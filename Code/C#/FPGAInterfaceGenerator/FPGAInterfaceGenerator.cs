using System.Text;

namespace FPGAInterfaceGenerator
{
    public class FPGAInterfaceGenerator
    {
        public static void Generate(FPGAInterfaceSetup setup,
            string cPlusPlusFilePath, string verilogFilePath) {
            if (setup.Name.ToLower() == "fpgainterface") {
                throw new ArgumentException($"Cannot use {nameof(setup.Name)} \"{setup.Name}\"");
            }
            GenerateCPlusPlusInterface(setup, cPlusPlusFilePath);
            GenerateFPGAInterface(setup, verilogFilePath);
        }
        private static void GenerateCPlusPlusInterface(
            FPGAInterfaceSetup setup, 
            string cPlusPlusFilePath) {
            StringBuilder sb = new StringBuilder();
            sb.AppendLine("#include \"FPGA/FPGAInterface.hpp\"");
            sb.Append("class ");
            sb.Append(setup.Name);
            sb.AppendLine(" {");
            appendOutput = Create_AppendOutput;
            foreach (var output in setup.Outputs) {
                sb.Append("public ");
                string variableTypeName= GetTypeName(output.VariableType);
                sb.Append(variableTypeName);
                sb.Append(" ");
                sb.Append(ToUpperCamelCase(output.Name));
                sb.AppendLine("(){");
                switch (output.VariableType) {
                    case VariableType.Bit:
                        sb.AppendLine();
                        break;
                    case VariableType.Byte:
                        break;
                    case VariableType.UInt16:
                        break;
                    default:
                        throw new NotImplementedException();
                }

            sb.AppendLine("}");
        }
        private static void GenerateFPGAInterface(
            FPGAInterfaceSetup setup, 
            string verilogFilePath) { 
            
        }
    }
}
