using Core.Strings;

namespace FPGAConstantsGenerator
{
    public static class ConstantsFactory
    {
        public static Constant[] FromEnum<TEnum>(int? nBits = null) where TEnum : struct, Enum
        {
            string enumName = StringHelper.CamelCaseToSnakeCase(typeof(TEnum).Name).ToUpper();
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
            var constants = new List<Constant> {
                new Constant($"{enumName}_LAST_INDEX", bitsRequired-1, Format.RawInteger)
            };
            foreach(var value in values){
                string name = StringHelper.CamelCaseToSnakeCase(value.ToString()).ToUpper();
                string localParamName = $"{enumName}_{name}";
                int intValue = Convert.ToInt32(value);
                constants.Add(new Constant(localParamName, intValue, Format.Binary, bitsRequired));
            }
            return constants.ToArray();
        }
    }
}
