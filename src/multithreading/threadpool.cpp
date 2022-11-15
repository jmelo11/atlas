/*
 * Created on Mon Oct 31 2022
 *
 * Author: Jose Melo
 */

#include <atlas/multithreading/threadpool.hpp>

//  Statics
namespace Atlas {
    ThreadPool ThreadPool::myInstance;
    thread_local size_t ThreadPool::myTLSNum = 0;
}  // namespace atlas
