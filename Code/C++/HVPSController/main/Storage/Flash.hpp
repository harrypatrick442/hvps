
class Flash{
    public:
        static void initialize();
        static bool getIsInitialized();
    private:
        static bool _isInitialized;
};