using Core.Strings;

namespace FPGAConstantsGenerator
{
    public static class ConstantsFactory
    {
        public static Constant[] FromEnum<TEnum>(int? nBits = null) where TEnum : struct, Enum
        {
            string enumName = StringHelper.CamelCaseToSnakeCase(typeof(TEnum).Name).ToUpper();
            List<Constant> constants= new List<Constant>();
            var values = Enum.GetValues<TEnum>();
            int maxValue = values.Select(v=>Convert.ToInt32(v)).Max();
            int bitsRequired = Math.Max(1, (int)Math.Ceiling(Math.Log2(maxValue + 1)));
            if (nBits.HasValue)
            {
                if (nBits < bitsRequired)
                {
                    throw new ArgumentException($"Was unable to use {nBits}. Required a minimum of {bitsRequired} bits as had maximum value of {maxValue}.");
                }
                bitsRequired = nBits.Value;
            }
            return values.Select(value => {
                string name = StringHelper.CamelCaseToSnakeCase(value.ToString()).ToUpper();
                string localParamName = $"{enumName}_{name}";
                int intValue = Convert.ToInt32(value);
                return new Constant(localParamName, intValue, bitsRequired, Format.Binary);
            }).ToArray();
        }
    }
}
