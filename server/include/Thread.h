#if !defined(_THREAD_H)
#define _THREAD_H

#include "MyConf.h"
#include "Chosed_Str.h"
#include "MyCompareDoc.h"
#include "MyJson.h"

class ThreadPool;

class Thread
{
public:
	virtual void start() = 0;
	virtual void setPThPool(ThreadPool * ) = 0;

protected:
	pthread_t  m_tid;
	ThreadPool * pThPool;
	pthread_mutex_t  cache_mutex;
	virtual void  readtask()  = 0;

};


class MyThread : public Thread
{
public:

	void setPThPool(ThreadPool * p);
	void start() ;
	MyThread(){};
	MYHASHMAP::hash_map<std::string, std::string, MyHashFn> th_cache;
	std::map< std::set<std::string> ,std::vector<std::pair<int, std::vector<double> > >  > th_result_cache; //查询结果cache
protected:
	struct epoll_event ev;
	int dp[MAXLINE][MAXLINE];
	int calcDidstanc(std::vector<std::string> pstr1, int len1, std::vector<std::string> pstr2, int len2);
	std::vector<std::string>  StrToVec(std::string word);
	void  readtask()  ;
	std::string RunTask(std::string);
	bool  find_txt(std::vector<std::string>& qurey_vec, std::vector<std::pair< int, std::vector<double> > >& res_vec);
	std::string excute_qurey(std::vector<std::string> qurey_vec ,std::vector<std::pair<int, std::vector<double> > > & res_vec );
	static void* call_back(void *args);  
};


class Cache_Thread :public Thread
{
public:
	void setPThPool(ThreadPool * p);
	void start() ;
	Cache_Thread(){};

private:
	static void* call_back(void *args); 
	void  readtask()  ;

};

#endif

