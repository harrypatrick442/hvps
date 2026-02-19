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
                buffer._Values[i + toStartIndex] = _Values[i];
            }
        }
        public bool ShiftLeft(bool newValue)
        {
            bool ret = _Values[_Values.Length - 1];
            int index = _Values.Length - 2;
            while (index >=0)
            {
                bool value =  _Values[index];
                _Values[index+1] = value;
                index--;
            }
            _Values[0] = newValue;
            return ret;
        }
        public bool ShiftRight(bool newValue)
        {
            int index = 1;
            bool ret = _Values[0];
            while (index <_Values.Length)
            {
                bool value = _Values[index];
                _Values[index-1] = value;
                index++;
            }
            _Values[_Values.Length - 1] = newValue;
            return ret;
        }
    }
}
