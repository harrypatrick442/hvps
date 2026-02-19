using Core.Timing;
using Initialization.Exceptions;
using Timer = System.Timers.Timer;

namespace ESPFPGAInterface.Mock
{
    public class ESPFPGAInterface
    {
        private const int INTERVAL_MILLISECONDS = 100;
        private const int MINIMUM_UPDATE_PERIOD = 50;
        private static ESPFPGAInterface? _Instance;
        public static ESPFPGAInterface Initialize(
            UInt16 inputsLength, UInt16 outputsLength, 
            IESPSideOfBus fpgaBus) {
            if (_Instance != null) {
                throw new AlreadyInitializedException(nameof(ESPFPGAInterface));
            }
            _Instance = new ESPFPGAInterface(inputsLength, outputsLength, fpgaBus);
            return _Instance;
        }
        public static ESPFPGAInterface GetInstance()
        {
            if (_Instance == null) {
                throw new NotInitializedException(nameof(ESPFPGAInterface));
            }
            return _Instance;
        }
        /// <summary>
        /// LSB first
        /// </summary>
        private int _InputsLength;
        private int _OutputsLength;
        private bool[] _InputBuffer;
        private bool[] _FullOutputBuffer;
        private bool _Disposed = false;
        private bool _InputsChanged = true;
        private readonly object _LockObject = new object();
        private IESPSideOfBus _FPGABus;
        protected ESPFPGAInterface(UInt16 inputsLength, UInt16 outputsLength, IESPSideOfBus fpgaBus) {
            _InputsLength = inputsLength;
            _OutputsLength = outputsLength;
            _FPGABus = fpgaBus;
            _InputBuffer = new bool[inputsLength];
            _FullOutputBuffer = new bool[inputsLength+outputsLength];
            _FPGABus.SetGoLive(false);
            _FPGABus.SetInShift(false);
            _FPGABus.SetOutShift(false);
            _FPGABus.SetToOutput(false);
            StartLooping();
        }
        public void SetBit(UInt16 index, bool value) {
            lock (_LockObject)
            {
                _InputBuffer[index] = value;
                _InputsChanged = true;
            }
        }
        public void SetByte(UInt16 indexFrom, byte value)
        {
            lock (_LockObject)
            {
                _InputBuffer[indexFrom] = (value & 0x01) != 0; // Bit 0 (LSB)
                _InputBuffer[indexFrom + 1] = (value & 0x02) != 0; // Bit 1
                _InputBuffer[indexFrom + 2] = (value & 0x04) != 0; // Bit 2
                _InputBuffer[indexFrom + 3] = (value & 0x08) != 0; // Bit 3
                _InputBuffer[indexFrom + 4] = (value & 0x10) != 0; // Bit 4
                _InputBuffer[indexFrom + 5] = (value & 0x20) != 0; // Bit 5
                _InputBuffer[indexFrom + 6] = (value & 0x40) != 0; // Bit 6
                _InputBuffer[indexFrom + 7] = (value & 0x80) != 0; // Bit 7 (MSB)
                _InputsChanged = true;
            }
        }
        public void SetUInt16(UInt16 indexFrom, UInt16 value)
        {
            lock (_LockObject)
            {
                var buffer = _InputBuffer;

                // --- Lower Byte (value & 0xFF) ---
                buffer[indexFrom] = (value & 0x0001) != 0; // Bit 0 (LSB)
                buffer[indexFrom + 1] = (value & 0x0002) != 0; // Bit 1
                buffer[indexFrom + 2] = (value & 0x0004) != 0; // Bit 2
                buffer[indexFrom + 3] = (value & 0x0008) != 0; // Bit 3
                buffer[indexFrom + 4] = (value & 0x0010) != 0; // Bit 4
                buffer[indexFrom + 5] = (value & 0x0020) != 0; // Bit 5
                buffer[indexFrom + 6] = (value & 0x0040) != 0; // Bit 6
                buffer[indexFrom + 7] = (value & 0x0080) != 0; // Bit 7

                // --- Upper Byte (value >> 8) ---
                buffer[indexFrom + 8] = (value & 0x0100) != 0; // Bit 8
                buffer[indexFrom + 9] = (value & 0x0200) != 0; // Bit 9
                buffer[indexFrom + 10] = (value & 0x0400) != 0; // Bit 10
                buffer[indexFrom + 11] = (value & 0x0800) != 0; // Bit 11
                buffer[indexFrom + 12] = (value & 0x1000) != 0; // Bit 12
                buffer[indexFrom + 13] = (value & 0x2000) != 0; // Bit 13
                buffer[indexFrom + 14] = (value & 0x4000) != 0; // Bit 14
                buffer[indexFrom + 15] = (value & 0x8000) != 0; // Bit 15 (MSB)

                _InputsChanged = true;
            }
        }
        public bool GetBit(UInt16 index)
        {
            lock (_LockObject)
            {
                return _FullOutputBuffer[index];
            }
        }

        public byte GetByte(UInt16 indexFrom)
        {
            lock (_LockObject)
            {
                var buffer = _FullOutputBuffer;
                int result = 0;

                // Reconstruct byte from bool array (LSB at lowest index)
                if (buffer[indexFrom]) result |= 0x01;
                if (buffer[indexFrom + 1]) result |= 0x02;
                if (buffer[indexFrom + 2]) result |= 0x04;
                if (buffer[indexFrom + 3]) result |= 0x08;
                if (buffer[indexFrom + 4]) result |= 0x10;
                if (buffer[indexFrom + 5]) result |= 0x20;
                if (buffer[indexFrom + 6]) result |= 0x40;
                if (buffer[indexFrom + 7]) result |= 0x80;

                return (byte)result;
            }
        }

        public UInt16 GetUInt16(UInt16 indexFrom)
        {
            lock (_LockObject)
            {
                var buffer = _FullOutputBuffer;
                UInt16 result = 0;

                // --- Lower Byte ---
                if (buffer[indexFrom]) result |= 0x0001;
                if (buffer[indexFrom + 1]) result |= 0x0002;
                if (buffer[indexFrom + 2]) result |= 0x0004;
                if (buffer[indexFrom + 3]) result |= 0x0008;
                if (buffer[indexFrom + 4]) result |= 0x0010;
                if (buffer[indexFrom + 5]) result |= 0x0020;
                if (buffer[indexFrom + 6]) result |= 0x0040;
                if (buffer[indexFrom + 7]) result |= 0x0080;

                // --- Upper Byte ---
                if (buffer[indexFrom + 8]) result |= 0x0100;
                if (buffer[indexFrom + 9]) result |= 0x0200;
                if (buffer[indexFrom + 10]) result |= 0x0400;
                if (buffer[indexFrom + 11]) result |= 0x0800;
                if (buffer[indexFrom + 12]) result |= 0x1000;
                if (buffer[indexFrom + 13]) result |= 0x2000;
                if (buffer[indexFrom + 14]) result |= 0x4000;
                if (buffer[indexFrom + 15]) result |= 0x8000;

                return (UInt16)result;
            }
        }
        private void SetStagedInputsLive() {
            _FPGABus.SetGoLive(true);
            Sleep();
            _FPGABus.SetGoLive(false);
            Sleep();
        }
        private bool ValidateStagedInputs() {
            int inputsIndex = 0; 
            while (inputsIndex < _InputsLength)
            {
                if (_InputBuffer[inputsIndex] != _FullOutputBuffer[inputsIndex]) {
                    return false;
                }
                inputsIndex++;
            }
            return true;
        }
        /// <summary>
        /// shifts MSB into FPGA first
        /// </summary>
        private void ShiftValuesIn()
        {
            int i = _InputBuffer.Length - 1;
            while (i >=0)
            {
                _FPGABus.SetInShift(false);
                _FPGABus.SetInValue(_InputBuffer[i--]);
                Sleep();
                _FPGABus.SetInShift(true);
                Sleep();
            }
            _FPGABus.SetInShift(false);
        }
        private void ReadOutputs(bool includingStaging) {
            _FPGABus.SetToOutput(true);
            Sleep();
            _FPGABus.SetToOutput(false);
            Sleep();
            int length;
            if (includingStaging) {
                length = _FullOutputBuffer.Length;
            }
            else
            {
                length = _OutputsLength;
            }
            int i = 0;
            int intoIndex = _FullOutputBuffer.Length - 1;
            while (i <length)
            {
                _FPGABus.SetOutShift(true);
                Sleep();
                _FullOutputBuffer[intoIndex--] = _FPGABus.GetOutValue();
                _FPGABus.SetOutShift(false);
                Sleep();
                i++;
            }
        }
        private void Sleep() {
            Thread.Sleep(1);
        }
        public void Dispose()
        {
            lock (_LockObject) {
                _Disposed = true;
            }
        }
        private void StartLooping() {
            new Thread(Loop).Start();
        }
        private void Loop()
        {
            long startTime = TimeHelper.MillisecondsNow;
            while (true) {
                lock (_LockObject)
                {
                    if (_Disposed) return;
                    if (!_InputsChanged)
                    {
                        ReadOutputs(includingStaging: false);
                        DoLoopSleep(ref startTime);
                        continue;
                    }
                    ShiftValuesIn();
                    ReadOutputs(includingStaging: true);
                    if (ValidateStagedInputs())
                    {
                        SetStagedInputsLive();
                        _InputsChanged = false;
                    }
                }
                DoLoopSleep(ref startTime);
            }
        }
        private void DoLoopSleep(ref long startTime) {

            long now = TimeHelper.MillisecondsNow;
            long elapsed = now - startTime;
            startTime = now;

            if (elapsed > int.MaxValue)
            {
                return;
            }
            int toSleep = MINIMUM_UPDATE_PERIOD - (int)elapsed;
            if (toSleep <= 0)
            {
                return;
            }
            Thread.Sleep(toSleep);
        }
    }
}
