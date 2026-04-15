using System;
using System.IO;
using System.Linq;
using System.Reflection;
using BaseMessages.Constants;
using Core.FileSystem;
using DataMemberNamesClassBuilder;
using FPGAInterfaceGenerator;
using SharedMasterSlaveUARTMessages;
using SharedMasterSlaveUARTMessages.DataMemberNames.Messages;
namespace HVPSCodeGenerator
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
            GenerateMessageWrappers(hvpsDirectory);
            GenerateFPGAInterface(hvpsDirectory);
        }
        private static void GenerateMessageWrappers(string hvpsDirectory) {
            Type[] typeInEachNamespace = new Type[] {
                typeof(Configurations.Android),
                typeof(Ticketing),
                typeof(BaseMessages.Constants.Ticketing),
                typeof(Native.DataMemberNames.Messages.NativeCloseFileMessageDataMemberNames),
                typeof(CoreNativeAndBackend.DataMemberNames.Messages.FileInfoDataMemberNames),
                typeof(VirtualSockets.DataMemberNames.NewVirtualSocket2DataMemberNames),
                typeof(Bluetooth.DataMemberNames.Messages.BluetoothDeviceDataMemberNames),
                typeof(VoltageFeedbackMessages.DataMemberNames.Requests.SetVoltageThresholdRequestDataMemberNames),
                typeof(HVPSAPI.DataMemberNames.Responses.GreetingResponseDataMemberNames),
                typeof(HVPSUIMessages.DataMemberNames.Requests.ConnectToBluetoothDeviceRequestDataMemberNames),
                typeof(SharedMasterSlaveUARTMessages.DataMemberNames.Messages.HasBusMessageDataMemberNames)
            };
            JavaScriptConstantsBuilderHelper.Run(
                Path.Combine(hvpsDirectory, "Code", "JavaScript", "client", "src", "constants"), "Constants",
                typeInEachNamespace);
            JavascriptClassBuilderHelper.Run(
                    Path.Combine(hvpsDirectory, "Code", "JavaScript", "client", "src", "messages"),
                    "DataMemberNames",
                    typeInEachNamespace);
            MessageTypesBuilderHelper.GenerateMessageTypesJs(
                    Path.Combine(hvpsDirectory, "Code", "JavaScript", "client", "src", "messages"),
                    typeInEachNamespace);
            JavascriptClassBuilderHelper.Run(
                Path.Combine(hvpsDirectory, "Code", "JavaScript", "client", "src", "content_wrappers"),
                "ContentWrapperDataMemberNames",
                typeInEachNamespace);
            string generatedCppDirectory = Path.Combine(hvpsDirectory, "temp", "generated_cpp");
            DirectoryHelper.DeleteRecursively(generatedCppDirectory);
            Directory.CreateDirectory(generatedCppDirectory);
            CPlusPlusClassBuilderHelper.Run(
                generatedCppDirectory,
                "DataMemberNames",
                typeInEachNamespace,
                reservedKeys:new ReservedKey[] {
                    new ReservedKey(HasBusMessageDataMemberNames.Target, typeof(HasBusMessage))
                });
            string[] generatedDirectories = new string[]{
                Path.Combine(
                        hvpsDirectory, "Code", "C++", "Shared","Generated", "Messages")
            };
            foreach (string directory in generatedDirectories)
            {
                Directory.CreateDirectory(directory);
                DirectoryHelper.CopyRecurively(generatedCppDirectory, directory);
            }
            DirectoryHelper.DeleteRecursively(generatedCppDirectory);
        }
        private static void GenerateFPGAInterface(string hvpsDirectory) {
            string cppGeneratedDirecotry = Path.Combine(
                hvpsDirectory, "Code", "C++", "HVPSController2",
                "main", "Generated");
            string verilogDirectory = Path.Combine(
                hvpsDirectory, "Code", "Verilog", "HVPSController2");
            Output actualFirstStageVoltage = new Output("ActualFirstStageVoltage", VariableType.Byte);
            Output actualOutputVoltage = new Output("ActualOutputVoltage", VariableType.Byte);
            Output actualPeakPrimaryCurrent = new Output("ActualPeakPrimaryCurrent", VariableType.Byte);

            FPGAInterfaceSetup fpgaInterfaceSetup = new FPGAInterfaceSetup(
                "HVPS_FPGAInterface",
                new IOVariable[] {
                    //PUT MOST IMPORTANT FIRST
                    new Input("DesiredOutputVoltage", VariableType.Byte),
                    new Input("Command", VariableType.CustomLengthBits, 8),
                    new Output("DoneCommand", VariableType.Bit),
                    actualFirstStageVoltage,
                    actualOutputVoltage,
                    actualPeakPrimaryCurrent,
                    new Output("State", VariableType.CustomLengthBits, 8),
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
                singleton: true
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
