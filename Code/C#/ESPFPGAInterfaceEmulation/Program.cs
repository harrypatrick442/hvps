using ESPFPGAInterface.Mock;
using EspInterface = ESPFPGAInterface.Mock.ESPFPGAInterface;
public class Program { 
    public static void Main(string[] args) {
        ushort inputsLength = 10;
        ushort outputsLength = 5;
        Bus bus = new Bus();
        EspInterface esp = EspInterface.Initialize(inputsLength, outputsLength, bus);
        FPGAEmulator fpgaEmulator = new FPGAEmulator(bus, inputsLength, outputsLength);
        esp.SetByte(2, 7);
        Thread.Sleep(1000);
        if (esp.GetBit(1))
        {
            throw new Exception();
        }
        if (esp.GetByte(2) != 7) {
            throw new Exception();
        }
        esp.SetByte(2, 100);
        esp.SetBit(1, true);
        Thread.Sleep(1000);
        byte b = esp.GetByte(2);
        if (b!= 100)
        {
            throw new Exception();
        }
        if (!esp.GetBit(1))
        {
            throw new Exception();
        }

    }
}