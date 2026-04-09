using System.Text;
namespace FPGAConstantsGenerator
{
    public class ConstantsGenerator
    {
        public static void Generate(string filePath, params Constant[] constants)
        {
            if (!filePath.EndsWith(".sv")) {
                throw new ArgumentException("Not a SystemVerilog .sv file path");
            }
            StringBuilder sb = new StringBuilder();
            sb.AppendLine("// AUTO GENERATED - DO NOT EDIT");
            sb.AppendLine();
            foreach (var constant in constants)
            {
                ValidateConstant(constant);
                sb.Append("localparam ");
                sb.Append(constant.Name);
                sb.Append(" = ");
                sb.Append(constant.NBits);
                sb.Append("'");
                sb.Append(FormatValue(constant));
                sb.AppendLine(";");
            }
            sb.AppendLine();
            File.Delete(filePath);
            File.WriteAllText(filePath, sb.ToString());
        }

        private static void ValidateConstant(Constant constant)
        {
            int maxValue = (1 << constant.NBits) - 1;
            if (constant.Value < 0 || constant.Value > maxValue)
                throw new ArgumentException(
                    $"Constant '{constant.Name}' value {constant.Value} " +
                    $"does not fit in {constant.NBits} bits (max {maxValue})");
        }

        private static string FormatValue(Constant constant)
        {
            return constant.WriteFormat switch
            {
                Format.Decimal => $"d{constant.Value}",
                Format.Hex => $"h{constant.Value:X}",
                Format.Binary => $"b{Convert.ToString(constant.Value, 2).PadLeft(constant.NBits, '0')}",
                _ => throw new NotImplementedException()
            };
        }
    }
}