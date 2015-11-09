#ifndef _DB_AGENT_H_
#define _DB_AGENT_H_

#include <mysql/mysql.h>
#include <stdlib.h>
#include <string>
#include <assert.h>
#include <string.h>

struct DBConfig
{
    std::string host;
    std::string user;
    std::string pwd;
    std::string db;
    std::string sock;

    unsigned int port;
};

class QueryResult
{
public:
    QueryResult()
        : m_Result(NULL)
        , m_Row(NULL)
        , m_ColNum(0)
        , m_ColIdx(0)
    {}

    ~QueryResult()
    {
        if (m_Result != NULL)
        {
            mysql_free_result(m_Result);
        }
    }

    bool StoreResult(MYSQL* handle)
    {
        FreeResult();

        if (!(m_Result = mysql_store_result(handle)))
            return false;

        m_ColNum = mysql_num_fields(m_Result);
        
        return true;
    }

    void FreeResult()
    {
        if (m_Result != NULL)
            mysql_free_result(m_Result);
        
        m_Result = NULL;
        m_Row = NULL;
        m_ColNum = 0;
        m_ColIdx = 0;
    }

    bool FetchRow()
    {
         m_Row = mysql_fetch_row(m_Result);
         m_ColIdx = 0;

         return m_Row != NULL;
    }

    template<class T>
    void FetchCol(T& v)
    {
    }

    void FetchCol(int& v)
    {
        assert(m_ColIdx < m_ColNum);

        v = atoi((char*)m_Row[m_ColIdx++]);
    }
    
    void FetchCol(unsigned int v)
    {
        assert(m_ColIdx < m_ColNum);

        v = atoi((char*)m_Row[m_ColIdx++]);
    }

    void FetchCol(char* &v)
    {
        assert(m_ColIdx < m_ColNum);

        v = (char*)m_Row[m_ColIdx++];
    }

    void FetchCol(std::string& v)
    {
        assert(m_ColIdx < m_ColNum);

        v = (char*)m_Row[m_ColIdx++];
    }

private:
    MYSQL_RES *m_Result;
    MYSQL_ROW m_Row;
    unsigned int m_ColNum;
    unsigned int m_ColIdx;
};

class DBAgent
{
public:
    DBAgent()
    {
        m_Handle = mysql_init(NULL);
        if (m_Handle == NULL)
            ShowError();
    }

    ~DBAgent()
    {
        if (m_Handle != NULL)
            mysql_close(m_Handle);
    }

    bool ConnectMySQL(DBConfig& config)
    {
        if (mysql_real_connect(m_Handle, config.host.c_str(), config.user.c_str(), config.pwd.c_str(), config.db.c_str(), config.port, config.sock.c_str(), 0) == NULL)
        {
            ShowError();
            return false;
        }

        return true;
    }

    bool ExecuteSQL(const char* sql)
    {
        return ExecuteSQL(sql, strlen(sql));
    }

    bool ExecuteSQL(const char* sql, QueryResult& result)
    {
        return ExecuteSQL(sql, strlen(sql), result);
    }

    bool ExectueSQL(std::string& sql)
    {
        return ExecuteSQL(sql.c_str(), sql.length());
    }

    bool ExecuteSQL(std::string& sql, QueryResult& result)
    {
        return ExecuteSQL(sql.c_str(), sql.length(), result);
    }

    bool ExecuteSQL(const char* sql, unsigned long length)
    {
        if (mysql_real_query(m_Handle, sql, length))
        {
            ShowError();
            return false;
        }

        return true;;
    }

    bool ExecuteSQL(const char* sql, unsigned  long length, QueryResult& result)
    {
        if (!ExecuteSQL(sql, length))
        {
            return false;
        }

       if (!result.StoreResult(m_Handle))
       {
           ShowError();

           return false;
       }

       return true;
    }

    void CloseMySQL()
    {
        if (m_Handle)
            mysql_close(m_Handle);

        m_Handle = NULL;
    }

private:
    void ShowError()
    {
        printf("MySQL Error %u, %s\n", mysql_errno(m_Handle), mysql_error(m_Handle));
    }

private:
    MYSQL* m_Handle;
};

#endif
