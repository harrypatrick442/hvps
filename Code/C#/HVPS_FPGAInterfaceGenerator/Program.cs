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
            Output actualFirstStageVoltage = new Output("ActualFirstStageVoltage", VariableType.Byte);
            Output actualOutputVoltage = new Output("ActualOutputVoltage", VariableType.Byte);
            Output actualPeakPrimaryCurrent = new Output("ActualPeakPrimaryCurrent", VariableType.Byte);

            FPGAInterfaceSetup fpgaInterfaceSetup = new FPGAInterfaceSetup(
                "HVPS_FPGAInterface",
                new IOVariable[] {
                    //PUT MOST IMPORTANT FIRST
                    new Input("Drive", VariableType.Bit),
                    new Input("Drive2", VariableType.Bit),
                    new Input("DesiredOutputVoltage", VariableType.Byte),
                    new Input("Command", VariableType.CustomLengthBits, 8),
                    actualFirstStageVoltage,
                    actualOutputVoltage,
                    actualPeakPrimaryCurrent,
                    new Output("Error", VariableType.Bit),
                    new Output("MaxFirstStageVoltage", VariableType.Byte),
                    new Output("MaxOutputVoltage", VariableType.Byte),
                    new Output("MaxPrimaryCurrent", VariableType.Byte),
                    new Output("EchoDesiredOutputVoltage", VariableType.Byte),
                    new Output("BufferedData", VariableType.CustomLengthBytes, 128),
                },
                new GetMultipleVariableCPlusPlusMethod[] {
                    new GetMultipleVariableCPlusPlusMethod(
                        "getAllFeedbacks",
                        actualFirstStageVoltage,
                        actualOutputVoltage,
                        actualPeakPrimaryCurrent
                    )
                },
                singleton:true
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
