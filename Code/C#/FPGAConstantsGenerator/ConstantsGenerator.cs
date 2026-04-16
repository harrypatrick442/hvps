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
                sb.Append(FormatValue(constant));
                sb.AppendLine(";");
            }
            sb.AppendLine();
            File.Delete(filePath);
            File.WriteAllText(filePath, sb.ToString());
        }

        private static void ValidateConstant(Constant constant)
        {
            if (!constant.WriteFormat.Equals(Format.RawInteger))
            {
                if (!constant.NBits.HasValue) 
                    throw new ArgumentNullException($"{nameof(constant)}.{nameof(constant.NBits)} was null for {nameof(Constant)} with {nameof(constant.Name)} \"{constant.Name}\"");
                int maxValue = (1 << constant.NBits.Value) - 1;
                if (constant.Value < 0 || constant.Value > maxValue)
                    throw new ArgumentException(
                        $"Constant '{constant.Name}' value {constant.Value} " +
                        $"does not fit in {constant.NBits} bits (max {maxValue})");
            }
        }

        private static string FormatValue(Constant constant)
        {
            return constant.WriteFormat switch
            {
                Format.Decimal => $"{constant.NBits}'d{constant.Value}",
                Format.Hex => $"{constant.NBits}'h{Convert.ToString(constant.Value, 16).PadLeft(constant.NBits!.Value/4, '0')}",
                Format.Binary => $"{constant.NBits}'b{Convert.ToString(constant.Value, 2).PadLeft(constant.NBits!.Value, '0')}",
                Format.RawInteger=> $"{constant.Value}",
                _ => throw new NotImplementedException()
            };
        }
    }
}