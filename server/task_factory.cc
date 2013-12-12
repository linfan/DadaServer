#include "task_factory.h"
#include "task_tcpreadmsg.h"
#include "task_tcpwritemsg.h"
#include "utility.h"

TaskFactory* TaskFactory::m_ins = NULL;
auto_ptr<TaskFactory> TaskFactory::m_auto_ptr;
boost::mutex TaskFactory::inst_mutex;

TaskFactory::TaskFactory()
{
    TRACE_FUNC_BEGIN
    m_auto_ptr = auto_ptr<TaskFactory>(this);
    TRACE_FUNC_LEAVE
}

TaskFactory::~TaskFactory()
{
    TRACE_FUNC_BEGIN
    TRACE_FUNC_LEAVE
}

TaskFactory* TaskFactory::Ins()
{
    boost::mutex::scoped_lock lock(inst_mutex);
    if ( m_ins == NULL)
        m_ins = new TaskFactory();
    return m_ins;
}

Task* TaskFactory::CreateTask(int type, void *arg)
{
    TRACE_FUNC_BEGIN
    switch (type)
    {
        case TASK_TYPE_TCP_READ_MSG:
            return new TaskTcpReadMsg(arg);
        case TASK_TYPE_TCP_WRITE_MSG:
            return new TaskTcpWriteMsg(arg);
        default:
            return NULL;
    }
    TRACE_FUNC_LEAVE
}

