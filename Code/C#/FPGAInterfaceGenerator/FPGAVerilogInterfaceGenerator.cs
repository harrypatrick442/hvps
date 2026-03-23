using Core.Strings;
using System.Linq;
using System.Text;

namespace FPGAInterfaceGenerator
{
    public class FPGAVerilogInterfaceGenerator
    {
        public static void GenerateVerilogInterface(
            FPGAInterfaceSetup setup,
            string outputDirectory)
        {
            string moduleName = setup.Name;
            string vFilePath = Path.Combine(outputDirectory, $"{moduleName}.v");
            StringBuilder sb = new StringBuilder();

            // Calculate total bit lengths
            int inputsLength = 0;
            foreach (var input in setup.Inputs)
                IncrementIndexForType(ref inputsLength, input.VariableType);

            int outputsLength = 0;
            foreach (var output in setup.Outputs)
                IncrementIndexForType(ref outputsLength, output.VariableType);

            int totalLength = inputsLength + outputsLength;

            // Module declaration
            sb.AppendLine($"module {moduleName} (");
            sb.AppendLine("    input wire clk,");
            sb.AppendLine("    // Shift register bus pins");
            sb.AppendLine("    input wire in_shift,");
            sb.AppendLine("    input wire in_value,");
            sb.AppendLine("    output reg out_value,");
            sb.AppendLine("    input wire out_shift,");
            sb.AppendLine("    input wire to_output,");
            sb.AppendLine("    input wire go_live,");
            sb.AppendLine("    // Named outputs to core logic");

            // Named output ports (these go TO core logic)
            int bitIndex = 0;
            foreach (var input in setup.Inputs)
            {
                string portName = StringHelper.CamelCaseToSnakeCase(input.Name);
                string typeName = GetVerilogType(input.VariableType);
                sb.AppendLine($"    output wire {typeName} {portName},");
                IncrementIndexForType(ref bitIndex, input.VariableType);
            }

            // Named input ports (these come FROM core logic)
            bitIndex = 0;
            int outputPortCount = setup.Outputs.Count();
            int currentOutput = 0;
            foreach (var output in setup.Outputs)
            {
                currentOutput++;
                string portName = StringHelper.CamelCaseToSnakeCase(output.Name);
                string typeName = GetVerilogType(output.VariableType);
                string comma = currentOutput < outputPortCount ? "," : "";
                sb.AppendLine($"    input wire {typeName} {portName}{comma}");
                IncrementIndexForType(ref bitIndex, output.VariableType);
            }

            sb.AppendLine(");");
            sb.AppendLine();

            // Internal registers
            sb.AppendLine($"    // Input buffer (staged, not yet live)");
            sb.AppendLine($"    reg [{inputsLength - 1}:0] input_staged;");
            sb.AppendLine($"    // Live input buffer");
            sb.AppendLine($"    reg [{inputsLength - 1}:0] input_live;");
            sb.AppendLine($"    // Full output shift buffer (inputs + outputs)");
            sb.AppendLine($"    reg [{totalLength - 1}:0] output_buffer;");
            sb.AppendLine($"    // Shift counter");
            sb.AppendLine($"    integer shift_count;");
            sb.AppendLine();

            // Assign named output wires from live buffer
            sb.AppendLine("    // Named signal assignments from live input buffer");
            bitIndex = 0;
            foreach (var input in setup.Inputs)
            {
                string portName = StringHelper.CamelCaseToSnakeCase(input.Name);
                int width = GetBitWidth(input.VariableType);
                if (width == 1)
                {
                    sb.AppendLine($"    assign {portName} = input_live[{bitIndex}];");
                }
                else
                {
                    sb.AppendLine($"    assign {portName} = input_live[{bitIndex + width - 1}:{bitIndex}];");
                }
                IncrementIndexForType(ref bitIndex, input.VariableType);
            }
            sb.AppendLine();

            // Shift in logic (MSB first)
            sb.AppendLine("    // Shift in - MSB first");
            sb.AppendLine("    always @(posedge in_shift) begin");
            sb.AppendLine($"        input_staged <= {{input_staged[{inputsLength - 2}:0], in_value}};");
            sb.AppendLine("    end");
            sb.AppendLine();

            // Go live logic
            sb.AppendLine("    // Go live - commit staged to live");
            sb.AppendLine("    always @(posedge go_live) begin");
            sb.AppendLine("        input_live <= input_staged;");
            sb.AppendLine("    end");
            sb.AppendLine();

            // Load output buffer on to_output
            sb.AppendLine("    // Load output buffer when to_output pulses");
            sb.AppendLine("    always @(posedge to_output) begin");
            sb.Append($"        output_buffer <= {{");

            // Pack outputs MSB first: staged inputs first, then core logic outputs
           
            foreach (var output in setup.Outputs)
            {
                string portName = StringHelper.CamelCaseToSnakeCase(output.Name);
                sb.Append($"{portName}, ");
            }
            sb.AppendLine("input_staged};");
            sb.AppendLine("    end");
            sb.AppendLine();

            // Shift out logic
            sb.AppendLine("    // Shift out - MSB first");
            sb.AppendLine("    always @(posedge out_shift) begin");
            sb.AppendLine($"        out_value <= output_buffer[{totalLength - 1}];");
            sb.AppendLine($"        output_buffer <= {{output_buffer[{totalLength - 2}:0], 1'b0}};");
            sb.AppendLine("    end");
            sb.AppendLine();

            sb.AppendLine("endmodule");

            File.Delete(vFilePath);
            File.WriteAllText(vFilePath, sb.ToString());
        }

        private static void IncrementIndexForType(ref int index, VariableType variableType)
        {
            switch (variableType)
            {
                case VariableType.Bit: index++; return;
                case VariableType.Byte: index += 8; return;
                case VariableType.UInt16: index += 16; return;
                default: throw new NotImplementedException();
            }
        }

        private static int GetBitWidth(VariableType variableType)
        {
            switch (variableType)
            {
                case VariableType.Bit: return 1;
                case VariableType.Byte: return 8;
                case VariableType.UInt16: return 16;
                default: throw new NotImplementedException();
            }
        }

        private static string GetVerilogType(VariableType variableType)
        {
            switch (variableType)
            {
                case VariableType.Bit: return "";
                case VariableType.Byte: return "[7:0]";
                case VariableType.UInt16: return "[15:0]";
                default: throw new NotImplementedException();
            }
        }
    }
}