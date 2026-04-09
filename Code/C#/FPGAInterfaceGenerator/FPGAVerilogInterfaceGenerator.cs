using Core.Strings;
using System;
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
            int debounceLimit = (50 * setup.SleepPeriodUs)/2;
            string moduleName = setup.Name;
            string vFilePath = Path.Combine(outputDirectory, $"{moduleName}.v");
            StringBuilder sb = new StringBuilder();

            // Calculate total bit lengths
            int inputsLength = 0;
            foreach (var input in setup.Inputs)
                IncrementIndexForType(ref inputsLength, input.VariableType, input.CustomLength);

            int outputsLength = 0;
            foreach (var output in setup.Outputs)
                IncrementIndexForType(ref outputsLength, output.VariableType, output.CustomLength);

            int totalLength = inputsLength + outputsLength;

            // Module declaration
            sb.AppendLine($"module {moduleName} (");
            sb.AppendLine("    // Shift register bus pins");
            sb.AppendLine("    input wire clk,");
            sb.AppendLine("    input wire in_shift,");
            sb.AppendLine("    input wire in_value,");
            sb.AppendLine("    output wire out_value,");  // wire not reg - driven by assign
            sb.AppendLine("    input wire out_shift,");
            sb.AppendLine("    input wire to_output,");
            sb.AppendLine("    input wire go_live,");
            sb.AppendLine("    // Named outputs to core logic");

            // Named output ports (these go TO core logic)
            int bitIndex = 0;
            foreach (var input in setup.Inputs)
            {
                string portName = StringHelper.CamelCaseToSnakeCase(input.Name);
                string typeName = GetVerilogType(input.VariableType, input.CustomLength);
                sb.AppendLine($"    output wire {typeName} {portName},");
                IncrementIndexForType(ref bitIndex, input.VariableType, input.CustomLength);
            }

            // Named input ports (these come FROM core logic)
            bitIndex = 0;
            int outputPortCount = setup.Outputs.Count();
            int currentOutput = 0;
            foreach (var output in setup.Outputs)
            {
                currentOutput++;
                string portName = StringHelper.CamelCaseToSnakeCase(output.Name);
                string typeName = GetVerilogType(output.VariableType, output.CustomLength);
                string comma = currentOutput < outputPortCount ? "," : "";
                sb.AppendLine($"    input wire {typeName} {portName}{comma}");
                IncrementIndexForType(ref bitIndex, output.VariableType, output.CustomLength);
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
            sb.AppendLine();

            // Edge detection registers
            sb.AppendLine("    // Edge detection registers");
            sb.AppendLine("    reg in_shift_prev;");
            sb.AppendLine("    reg go_live_prev;");
            sb.AppendLine("    reg to_output_prev;");
            sb.AppendLine("    reg out_shift_prev;");
            sb.AppendLine();

            // Debounced signal wires
            sb.AppendLine("    // Debounced signal wires");
            sb.AppendLine("    wire in_shift_debounced;");
            sb.AppendLine("    wire go_live_debounced;");
            sb.AppendLine("    wire to_output_debounced;");
            sb.AppendLine("    wire out_shift_debounced;");
            sb.AppendLine();

            // Assign named output wires from live buffer
            sb.AppendLine("    // Named signal assignments from live input buffer");
            bitIndex = 0;
            foreach (var input in setup.Inputs)
            {
                string portName = StringHelper.CamelCaseToSnakeCase(input.Name);
                int width = GetBitWidth(input.VariableType, input.CustomLength);
                if (width == 1)
                {
                    sb.AppendLine($"    assign {portName} = input_live[{bitIndex}];");
                }
                else
                {
                    sb.AppendLine($"    assign {portName} = input_live[{bitIndex + width - 1}:{bitIndex}];");
                }
                IncrementIndexForType(ref bitIndex, input.VariableType, input.CustomLength);
            }

            // Combinatorial assignment for out_value - always reflects current top of buffer
            sb.AppendLine($"    assign out_value = output_buffer[{totalLength - 1}];");
            sb.AppendLine();

            // Debouncer instantiations
            sb.AppendLine("    // Debouncer instantiations");
            AppendDebouncer(sb, "in_shift", "in_shift_debounced", debounceLimit);
            AppendDebouncer(sb, "go_live", "go_live_debounced", debounceLimit);
            AppendDebouncer(sb, "to_output", "to_output_debounced", debounceLimit);
            AppendDebouncer(sb, "out_shift", "out_shift_debounced", debounceLimit);
            sb.AppendLine();

            // Single synchronous always block
            sb.AppendLine("    // All logic synchronous to system clock");
            sb.AppendLine("    always @(posedge clk) begin");
            sb.AppendLine();
            sb.AppendLine("        // Update edge detection registers");
            sb.AppendLine("        in_shift_prev  <= in_shift_debounced;");
            sb.AppendLine("        go_live_prev   <= go_live_debounced;");
            sb.AppendLine("        to_output_prev <= to_output_debounced;");
            sb.AppendLine("        out_shift_prev <= out_shift_debounced;");
            sb.AppendLine();
            sb.AppendLine("        // Shift in - rising edge of in_shift");
            sb.AppendLine("        if (in_shift_debounced && !in_shift_prev) begin");
            sb.AppendLine($"            input_staged <= {{input_staged[{inputsLength - 2}:0], in_value}};");
            sb.AppendLine("        end");
            sb.AppendLine();
            sb.AppendLine("        // Go live - rising edge of go_live");
            sb.AppendLine("        if (go_live_debounced && !go_live_prev) begin");
            sb.AppendLine("            input_live <= input_staged;");
            sb.AppendLine("        end");
            sb.AppendLine();
            sb.AppendLine("        // Output buffer - rising edge of to_output or out_shift");
            sb.AppendLine("        if (to_output_debounced && !to_output_prev) begin");
            sb.Append($"            output_buffer <= {{");
            foreach (var output in setup.Outputs.Reverse())
            {
                string portName = StringHelper.CamelCaseToSnakeCase(output.Name);
                sb.Append($"{portName}, ");
            }
            sb.AppendLine("input_staged};");
            sb.AppendLine("        end else if (out_shift_debounced && !out_shift_prev) begin");
            sb.AppendLine($"            output_buffer <= {{output_buffer[{totalLength - 2}:0], 1'b0}};");
            sb.AppendLine("        end");
            sb.AppendLine();
            sb.AppendLine("    end");
            sb.AppendLine();
            sb.AppendLine("endmodule");

            File.Delete(vFilePath);
            File.WriteAllText(vFilePath, sb.ToString());
        }

        private static void AppendDebouncer(
            StringBuilder sb, string signalName, string debouncedName,
            int debounceLimit)
        {
            sb.AppendLine($"    debouncer #(");
            sb.AppendLine($"        .DEBOUNCE_LIMIT({debounceLimit})");
            sb.AppendLine($"    ) debouncer_{signalName} (");
            sb.AppendLine($"        .clk(clk),");
            sb.AppendLine($"        .bouncy_in({signalName}),");
            sb.AppendLine($"        .debounced_out({debouncedName})");
            sb.AppendLine($"    );");
        }

        private static void IncrementIndexForType(ref int index, VariableType variableType, int? customLength)
        {
            switch (variableType)
            {
                case VariableType.Bit: index++; return;
                case VariableType.Byte: index += 8; return;
                case VariableType.UInt16: index += 16; return;
                case VariableType.CustomLengthBits:
                    if (!customLength.HasValue) throw new NullReferenceException();
                    index += customLength!.Value; 
                    return;
                case VariableType.CustomLengthBytes:
                    if (!customLength.HasValue) throw new NullReferenceException();
                    index += (customLength!.Value * 8); 
                    return;
                default: throw new NotImplementedException();
            }
        }

        private static int GetBitWidth(VariableType variableType, int? customLength)
        {
            switch (variableType)
            {
                case VariableType.Bit: return 1;
                case VariableType.Byte: return 8;
                case VariableType.UInt16: return 16;
                case VariableType.CustomLengthBits:
                    if (!customLength.HasValue) throw new NullReferenceException(); 
                    return customLength!.Value;
                case VariableType.CustomLengthBytes:
                    if (!customLength.HasValue) throw new NullReferenceException(); 
                    return customLength!.Value * 8;
                default: throw new NotImplementedException();
            }
        }

        private static string GetVerilogType(VariableType variableType, int? customLength)
        {
            switch (variableType)
            {
                case VariableType.Bit: return "";
                case VariableType.Byte: return "[7:0]";
                case VariableType.UInt16: return "[15:0]";
                case VariableType.CustomLengthBits:
                    if (!customLength.HasValue) throw new NullReferenceException(); 
                    return $"[{customLength!.Value - 1}:0]";
                case VariableType.CustomLengthBytes:
                    if (!customLength.HasValue) throw new NullReferenceException(); 
                    return $"[{(customLength!.Value * 8)- 1}:0]";
                default: throw new NotImplementedException();
            }
        }
    }
}
