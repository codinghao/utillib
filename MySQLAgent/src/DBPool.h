#ifndef _DB_POOL_H_
#define _DB_POOL_H_

#include "DBAgent.h"
#include <pthread.h>
#include <deque>
#include <algorithm>
#include <functional>

class DBPool
{
public:
    typedef std::deque<DBAgent*> DBQueue;

    DBPool()
    {
    }
    
    ~DBPool()
    {
        std::for_each(m_DBQueue.begin(), m_DBQueue.end(), std::bind1st(std::mem_fun(&DBPool::DeallocAgent), this));
        m_DBQueue.clear();
    }

    void Init(DBConfig& config, unsigned int max)
    {
        m_MaxNum = max;
        m_DBConfig = config;

        for (unsigned int i = 0; i < max; ++ i)
        {
            m_DBQueue.push_back(AllocAgent());
        }
    }

    DBAgent* Pop()
    {
        pthread_mutex_lock(&m_Mutex);

        if (!m_DBQueue.empty())
        {
           DBAgent* agent = m_DBQueue.front();
           m_DBQueue.pop_front();
           
           pthread_mutex_unlock(&m_Mutex);

           return agent;
        }

        pthread_mutex_unlock(&m_Mutex);

        return AllocAgent();
    }

    void Push(DBAgent* agent)
    {
        pthread_mutex_lock(&m_Mutex);

        if (m_DBQueue.size() < m_MaxNum)
        {
            m_DBQueue.push_back(agent);
            pthread_mutex_unlock(&m_Mutex);
            return ;
        }

        pthread_mutex_unlock(&m_Mutex);

        DeallocAgent(agent);
    }

private:

    DBAgent* AllocAgent()
    {
        printf("new\n");

        DBAgent* agent = new DBAgent();

        assert(agent != NULL);

        agent->ConnectMySQL(m_DBConfig);

        return agent;
    }

    void DeallocAgent(DBAgent* agent)
    {
        printf("delete\n");

        agent->CloseMySQL();

        delete agent;
    }

private:
    unsigned int m_MaxNum;
    DBQueue m_DBQueue;
    DBConfig m_DBConfig;
    pthread_mutex_t m_Mutex;
};

#endif
