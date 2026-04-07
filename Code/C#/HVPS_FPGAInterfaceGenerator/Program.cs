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
            Output actualFirstStageVoltage2 = new Output("ActualFirstStageVoltage2", VariableType.Byte);
            Output actualOutputVoltage2 = new Output("ActualOutputVoltage2", VariableType.Byte);
            Output actualPeakPrimaryCurrent2 = new Output("ActualPeakPrimaryCurrent2", VariableType.Byte);
            FPGAInterfaceSetup fpgaInterfaceSetup = new FPGAInterfaceSetup(
                "HVPS_FPGAInterface",
                new IOVariable[] {
                    new Input("Drive", VariableType.Bit),
                    new Input("Drive2", VariableType.Bit),
                    new Input("DesiredMaxFirstStageVoltage", VariableType.Byte),
                    new Input("DesiredOutputVoltage", VariableType.Byte),
                    new Input("DesiredMaxPeakPrimaryCurrent", VariableType.Byte),
                    new Input("Command", VariableType.CustomLengthBits, 8),
                    actualFirstStageVoltage,
                    actualOutputVoltage,
                    actualPeakPrimaryCurrent,
                    actualFirstStageVoltage2,
                    actualOutputVoltage2,
                    actualPeakPrimaryCurrent2,
                    new Output("Error", VariableType.Bit),
                    new Output("EchoDesiredMaxFirstStageVoltage", VariableType.Byte),
                    new Output("EchoDesiredDesiredOutputVoltage", VariableType.Byte),
                    new Output("EchoDesiredMaxPeakPrimaryCurrent", VariableType.Byte),
                    new Output("BufferedData", VariableType.CustomLengthBytes, 120),
                },
                new GetMultipleVariableCPlusPlusMethod[] {
                    new GetMultipleVariableCPlusPlusMethod(
                        "getAllFeedbacks",
                        actualFirstStageVoltage,
                        actualOutputVoltage,
                        actualPeakPrimaryCurrent,
                        actualFirstStageVoltage2,
                        actualOutputVoltage2,
                        actualPeakPrimaryCurrent2
                    )
                }
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
