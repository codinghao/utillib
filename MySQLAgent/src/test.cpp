#include <stdio.h>
#include "DBPool.h"
#include <pthread.h>

void test_dbagent()
{
    DBAgent db;
    DBConfig config;
    config.host = "localhost";
    config.user = "admin";
    config.pwd = "wzh";
    config.port = 3306;
    config.db = "tinydb";
    config.sock = "/var/lib/mysql/mysql.sock";
    db.ConnectMySQL(config);
#if 0   
    db.ExecuteSQL("insert into foo values(1, 'zhangsan', 'zhangsan@foo.com')");
    db.ExecuteSQL("insert into foo values(2, 'luxi', 'luxi@foo.com')");
    db.ExecuteSQL("insert into foo values(3, 'wangwu', 'wangwu@foo.com')");
    db.ExecuteSQL("insert into foo values(4, 'lilei', 'lilei@foo.com')");
    db.ExecuteSQL("insert into foo values(5, 'lili', 'lili@foo.com')");
    db.ExecuteSQL("insert into foo values(6, 'hanmeimei', 'hanmeimei@foo.com')");
    db.ExecuteSQL("insert into foo values(7, 'john', 'john@foo.com')");
#endif
    int id = 0;
    char *name = NULL;
    char *email = NULL;

    QueryResult result;
    db.ExecuteSQL("select id, name, email from foo", result);
    while(result.FetchRow())
    {
        result.FetchCol(id);
        result.FetchCol(name);
        result.FetchCol(email);

        printf("id : %d, name : %s, email : %s\n", id, name, email);
    }

    result.FreeResult();
    db.CloseMySQL();
}

void* thread_work(void* arg)
{
    DBPool* pool = (DBPool*)arg;
    for (int i = 0; i < 10000; ++ i)
    {
        DBAgent* agent = pool->Pop();
        agent->ExecuteSQL("insert into foo(name, email) values('test', 'test')");
        pool->Push(agent);
    }
    
    printf("thread %u exit!\n", (unsigned int)pthread_self());

    return NULL;
}

void test_pool()
{
    DBPool pool;
    DBConfig config;
    config.host = "localhost";
    config.user = "admin";
    config.pwd = "wzh";
    config.port = 3306;
    config.db = "tinydb";
    config.sock = "/var/lib/mysql/mysql.sock";
    pool.Init(config, 2);
    pthread_t thread[8] = {0};

    for (int i = 0; i < 8; ++ i)
    {
        pthread_create(thread + i, NULL, &thread_work, (void*)&pool);
        printf("thread %u start...\n", (unsigned int)thread[i]);
    }

    for (int i = 0; i < 8; ++ i)
        pthread_join(thread[i], NULL);
}

int main(int argc, char* argv[])
{
#if 0
    test_agent();
#endif

    test_pool();

    return 0;
}
