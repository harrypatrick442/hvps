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
            string cppGeneratedDirecotry = Path.Combine(
                hvpsDirectory, "Code", "C++", "HVPSController2",
                "main", "Generated");
            string verilogDirectory =  Path.Combine(
                hvpsDirectory, "Code", "Verilog", "HVPSController2");
            FPGAInterfaceSetup fpgaInterfaceSetup = new FPGAInterfaceSetup(
                "HVPS_FPGAInterface",
                new Input("Drive", VariableType.Bit),
                new Input("Drive2", VariableType.Bit),
                new Input("DesiredMaxFirstStageVoltage", VariableType.Byte),
                new Input("DesiredOutputVoltage", VariableType.Byte),
                new Input("DesiredMaxPeakPrimaryCurrent", VariableType.Byte),
                new Output("ActualFirstStageVoltage", VariableType.Byte),
                new Output("ActualOutputVoltage", VariableType.Byte),
                new Output("ActualPeakPrimaryCurrent", VariableType.Byte),
                new Output("Error", VariableType.Bit)
            );
            FPGAInterfaceGenerator.FPGAInterfaceGenerator
                .Generate(
                    fpgaInterfaceSetup,
                    cppGeneratedDirecotry,
                    verilogDirectory
                    );
        }
    }
}
