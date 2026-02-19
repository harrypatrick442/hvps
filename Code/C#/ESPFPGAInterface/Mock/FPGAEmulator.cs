namespace ESPFPGAInterface.Mock
{
    public class FPGAEmulator
    {
        private IFPGASideOfBus _Bus;
        private Buffer _InputsBuffer;
        private Buffer _FullOutputsBuffer;
        private Buffer _LiveInputsBuffer;
        private Buffer _CoreLogicBuffer;
        public FPGAEmulator(IFPGASideOfBus bus, int inputsLength, int outputsLength) {
            _Bus = bus;
            _InputsBuffer = new Buffer(inputsLength);
            _LiveInputsBuffer = new Buffer(inputsLength);
            _FullOutputsBuffer = new Buffer(outputsLength+ inputsLength);
            _CoreLogicBuffer = new Buffer(outputsLength);
            _Bus.InShiftWire.PosEdge += HandlePosedgeInShiftWire;
            _Bus.GoLiveWire.PosEdge += HandlePosedgeGoLiveWire;
            _Bus.ToOutputWire.PosEdge += HandlePosedgeToOutputsWire;
            _Bus.OutShiftWire.PosEdge += HandlePosEdgeOutShiftWire;
        }
        private void HandlePosedgeInShiftWire(object o, EventArgs e)
        {
            //Shift left will be for the most significant bit first. Meaning the bit at the highest index first
            _InputsBuffer.ShiftLeft(_Bus.InValueWire.Get());
        }
        private void HandlePosedgeGoLiveWire(object o, EventArgs e)
        {
            _InputsBuffer.CopyTo(_LiveInputsBuffer);
        }
        private void HandlePosedgeToOutputsWire(object o, EventArgs e)
        {
            _InputsBuffer.CopyTo(_FullOutputsBuffer, 0);
            _CoreLogicBuffer.CopyTo(_FullOutputsBuffer, _InputsBuffer.Length);
        }
        private void HandlePosEdgeOutShiftWire(object o, EventArgs e)
        {
            bool value = _FullOutputsBuffer.ShiftLeft(false);
            _Bus.OutValueWire.Set(value);
        }
    }
}
