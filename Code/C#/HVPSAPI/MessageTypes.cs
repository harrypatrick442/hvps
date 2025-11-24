namespace HVPSAPI
{
    public static class MessageTypes
    {
        public const string Start = "start";
        public const string Stop = "stop";
        public const string ShutDown = "sd";
        public const string RunSystemChecksOnly = "rsco";
        public const string StateChanged = "sc";
        public const string LiveData = "ld";
        public const string Message = "m";
        public const string Error = "e";
        public const string RanSystemChecks = "cm";
        public const string CoreDumpSummary = "cd";
        public const string LastAbort = "la";
        public const string Test = "test";
        public const string ClearLoggedErrors = "cle";
        public const string GreetingRequest = "gr";
        public const string Greeting = "g";
        public const string IndicateState = "is";
        public const string IndicateStateRequest = "isr";
        public const string SendStateToIndicate = "ssti";
    }
}
