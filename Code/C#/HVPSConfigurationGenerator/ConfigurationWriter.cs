using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using Checksums;
using Core.Atomics;
namespace HVPSConfigurationGenerator
{
    internal class ConfigurationWriter
    {
        public static void WriteConfigurationStructFile<TConfigurationStruct>(
            string structDefinitionFilePath,
            AlreadyWroteWatcher alreadyWroteWatcher
        ) where TConfigurationStruct : unmanaged
        {
            StringBuilder sbTop = new StringBuilder();
            sbTop.AppendLine("#pragma once");
            StringBuilder sbStruct = new StringBuilder();
            sbStruct.AppendLine("struct Configuration {");
            bool requiresCstdint = false;
            foreach (var field in typeof(TConfigurationStruct).GetFields(BindingFlags.Public | BindingFlags.Instance))
            {
                requiresCstdint = requiresCstdint||RequiresCstdint(field.FieldType);
                string cPlusPlusTypeName = GetCPlusPlusTypeName(field.FieldType);
                string fieldName = field.Name;
                sbStruct.AppendLine($"    const {cPlusPlusTypeName} {fieldName};");
            }
            sbStruct.AppendLine("};");
            if (requiresCstdint) {
                sbTop.AppendLine("#include <cstdint>");
            }
            sbTop.Append(sbStruct);
            alreadyWroteWatcher.ImGoingToWrite(structDefinitionFilePath);
            Directory.CreateDirectory(Path.GetDirectoryName(structDefinitionFilePath)!);
            File.WriteAllText(structDefinitionFilePath, sbTop.ToString());
        }
        public static void WriteProjectSpecificConfiguration<TConfigurationStruct>(
            string projectSpecificConfigurationFilePath,
            TConfigurationStruct configurationStruct,
            string structHppFileRelativePath,
            string dependenciesIncludePathPrefix,
            AlreadyWroteWatcher alreadyWroteWatcher,
            string instancePrefix = "Config"
        ) where TConfigurationStruct: unmanaged
        {


            uint crc = Crc32.Compute(in configurationStruct);
            StringBuilder sb = new StringBuilder();
            Atomic<bool> isFirst = new Atomic<bool>(true);
            sb.AppendLine("#pragma once");
            if (string.IsNullOrEmpty(structHppFileRelativePath)) {
                throw new ArgumentException(nameof(structHppFileRelativePath));
            }
            sb.AppendLine($"#include \"{structHppFileRelativePath}\"");
            sb.AppendLine($"#include \"{dependenciesIncludePathPrefix}System/Aborter.hpp\"");
            sb.AppendLine($"#include \"{dependenciesIncludePathPrefix}Core/Checksums/Crc32.hpp\"");
            string instance1Name = $"{instancePrefix}1";
            string instance2Name = $"{instancePrefix}2";
            sb.AppendLine($"inline constexpr Configuration {instance1Name}{{");
            foreach (var field in typeof(TConfigurationStruct).GetFields(BindingFlags.Public | BindingFlags.Instance))
            {
                string cPlusPlusTypeName = GetCPlusPlusTypeName(field.FieldType);
                string fieldName = field.Name;
                object? value = field.GetValue(configurationStruct);
                if (isFirst.Value)
                {
                    isFirst.Value = false;
                }
                else
                {
                    sb.AppendLine(",");
                }
                sb.Append($"    .{fieldName} = {value}");
            }
            sb.AppendLine();
            sb.AppendLine("};");
            sb.AppendLine($"inline const uint32_t CONFIG_CRC32_EXPECTED = {crc};");
            //sbAll.AppendLine("static_assert(podConfig1 == CONFIG_CRC32_EXPECTED, \"pod did not match expected crc\"");
            sb.AppendLine($"inline Configuration {instance2Name} = {instance1Name};//This one is in RAM. {instance1Name} is in ROM.");
            sb.AppendLine("inline bool validateConfiguration(){");
            sb.AppendLine($"    uint32_t podConfig1 = Crc32::computePod({instance1Name});");
            sb.AppendLine($"    uint32_t podConfig2 = Crc32::computePod({instance2Name});");

            sb.AppendLine($"    if (podConfig1 != podConfig2){{");

            sb.AppendLine($"        Aborter::safeAbort(\"Configuration\", \"The CRC32 computed for {instance2Name} did not match the CRC32 for {instance1Name}\");");
            sb.AppendLine($"        return false;");

            sb.AppendLine("    }");
            sb.AppendLine($"    if (podConfig1 != CONFIG_CRC32_EXPECTED){{");

            sb.AppendLine($"        Aborter::safeAbort(\"Configuration\", \"The CRC32 computed for {instance1Name} did not match the expected value\");");
            sb.AppendLine("        return false;");

            sb.AppendLine("    }");
            sb.AppendLine("    return true;");
            sb.AppendLine("}");
            alreadyWroteWatcher.ImGoingToWrite(projectSpecificConfigurationFilePath);
            Directory.CreateDirectory(Path.GetDirectoryName(projectSpecificConfigurationFilePath)!);
            File.WriteAllText(projectSpecificConfigurationFilePath, sb.ToString());
        }
        private static string GetCPlusPlusTypeName(Type fieldType)
        {
            if (fieldType == typeof(byte)) return "uint8_t";
            if (fieldType == typeof(sbyte)) return "int8_t";
            if (fieldType == typeof(short)) return "int16_t";
            if (fieldType == typeof(ushort)) return "uint16_t";
            if (fieldType == typeof(int)) return "int32_t";
            if (fieldType == typeof(uint)) return "uint32_t";
            if (fieldType == typeof(long)) return "int64_t";
            if (fieldType == typeof(ulong)) return "uint64_t";

            if (fieldType == typeof(float)) return "float";
            if (fieldType == typeof(double)) return "double";
            if (fieldType == typeof(decimal)) return "double"; // no native decimal in C++, map to double

            throw new NotSupportedException($"Unsupported type: {fieldType.FullName}");
        }
        private static bool RequiresCstdint(Type type)
        {
            return type == typeof(byte)
                || type == typeof(sbyte)
                || type == typeof(short)
                || type == typeof(ushort)
                || type == typeof(int)
                || type == typeof(uint)
                || type == typeof(long)
                || type == typeof(ulong);
        }
    }
}
