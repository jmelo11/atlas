#include <multithreading/threadpool.h>

//  Statics
namespace atlas {
	ThreadPool ThreadPool::myInstance;
	thread_local size_t ThreadPool::myTLSNum = 0;
}
