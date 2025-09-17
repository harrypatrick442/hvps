#ifndef PriorityLooper_hpp
#define PriorityLooper_hpp

class PriorityLooper{
	public:
		static const char* TAG;
		static void start();
	private:
		static void run(void*);
};
#endif