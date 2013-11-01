#ifndef _THREAD_WORKER_FACTORY_H_
#define _THREAD_WORKER_FACTORY_H_

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <boost/thread/mutex.hpp>
#include "task.h"
using namespace std;

class TaskFactory
{
    private:
        static auto_ptr<TaskFactory> m_auto_ptr;
        static TaskFactory *m_ins;
        static boost::mutex inst_mutex;
    protected:
        TaskFactory();
        TaskFactory(const TaskFactory&);
        virtual ~TaskFactory();
        friend class auto_ptr<TaskFactory>; 
    public:
        static TaskFactory* Ins();
        Task* CreateTask(int type, void *arg);
};

#endif

