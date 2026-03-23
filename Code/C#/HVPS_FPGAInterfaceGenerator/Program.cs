using System;
using System.IO;
using System.Linq;
using System.Reflection;
using FPGAInterfaceGenerator;
namespace HVPS_FPGAInterfaceGenerator
{
    class Program
    {
        static void Main(string[] args)
        {
            string reposDirectory = Assembly.GetEntryAssembly()!.Location;
            while (Path.GetFileName(reposDirectory).ToLower() != "repos")
            {
                reposDirectory = Directory.GetParent(reposDirectory)!.FullName;
            }
            string hvpsDirectory = Path.Combine(reposDirectory, "hvps");
            FPGAInterfaceSetup fpgaInterfaceSetup = new FPGAInterfaceSetup(
                "HVPS_FPGAInterface",
                new Input("Drive", VariableType.Bit),
                new Input("Drive2", VariableType.Bit),
                new Input("MaxFirstStageVoltageAllowed", VariableType.Byte),
                new Input("DesiredOutputVoltage", VariableType.Byte),
                new Output("ActualFirstStageVoltage", VariableType.Byte),
                new Output("ActualOutputVoltage", VariableType.Byte),
                new Output("ActualPrimaryCurrent", VariableType.Byte)
            );
        }
    }
}
