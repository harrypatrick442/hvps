using System.Text;

namespace FPGAInterfaceGenerator
{
    public class FPGAInterfaceGenerator
    {
        public static void Generate(FPGAInterfaceSetup setup,
            string cppDirectory, string verilogDirectory) {
            if (setup.Name.ToLower() == "fpgainterface") {
                throw new ArgumentException($"Cannot use {nameof(setup.Name)} \"{setup.Name}\"");
            }
            FPGACPlusPlusInterfaceGenerator.GenerateCPlusPlusInterface(
                setup, cppDirectory);
            FPGAVerilogInterfaceGenerator.GenerateVerilogInterface(
                setup, verilogDirectory);
        }
    }
}
