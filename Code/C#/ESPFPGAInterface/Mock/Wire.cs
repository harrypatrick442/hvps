namespace ESPFPGAInterface.Mock
{
    public class Wire
    {
        private bool _Value;
        public event EventHandler PosEdge;
        public event EventHandler NegEdge;
        public Wire() {
            _Value = false;
        }
        public void Set(bool value) {
            _Value = value;
            if (value)
            {
                PosEdge?.Invoke(this, new EventArgs());
            }
            else 
            {
                NegEdge?.Invoke(this, new EventArgs());
            }
        }
        public bool Get() { 
            return _Value;
        }
    }
}
