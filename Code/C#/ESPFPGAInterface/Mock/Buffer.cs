using Core;
using Core.Timing;
using Initialization.Exceptions;
using Timer = System.Timers.Timer;

namespace ESPFPGAInterface.Mock
{
    public class Buffer
    {
        public int Length { get; }
        private bool[] _Values;
        public Buffer(int length) {
            Length = length;
            _Values = new bool[length];
        }
        public void CopyTo(Buffer buffer) {
            CopyTo(buffer, 0);
        }
        public void CopyTo(Buffer buffer, int toStartIndex) {
            for (int i = 0; i < Length; i++)
            {
                buffer._Values[i] = _Values[i+ toStartIndex];
            }
        }
        public void ShiftLeft(bool newValue)
        {
            int index = 0;
            while (index < _Values.Length-1)
            {
                bool value =  _Values[index++];
                _Values[index] = value;
            }
            _Values[0] = newValue;
        }
        public void ShiftRight(bool newValue)
        {
            int index = _Values.Length - 1;
            while (index > 0)
            {
                bool value = _Values[index--];
                _Values[index] = value;
            }
            _Values[_Values.Length - 1] = newValue;
        }
    }
}
