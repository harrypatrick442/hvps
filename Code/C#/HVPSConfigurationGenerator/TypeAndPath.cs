using System.Runtime.InteropServices;
namespace HVPSConfigurationGenerator
{
    public class TypeAndPath
    {
        public Type StructType { get; }
        public string Path { get; }
        public TypeAndPath(Type structType, string path)
        {
            StructType = structType;
            Path = path;
        }
    }
}
