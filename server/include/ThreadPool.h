#if !defined(_THREADPOOL_H)
#define _THREADPOOL_H

#include "MyConf.h"
#include "MyLock.h"
#include "MyCondition.h"
#include "Task.h"
#include "User_Data.h"
#include "MyLock.h"
#include "Task.h"
#include "MyCondition.h"
//#include "MyThread.h"
#include "MySocket.h"
#include "MySplit.h"
#include "MyPage.h"


class MyThread;
class MySocket;

class ThreadPool {

public:

	ThreadPool( MyConf & myconf ,const int Thread_Num,MySplit* txtsplit , MYHASHMAP::hash_map<int, MyPage>&  page_map);
	MYHASHMAP::hash_map<std::string, std::set<std::string>, MyHashFn>   Index;
	MYHASHMAP::hash_map<std::string , int ,MyHashFn> word_fre ;
	MYHASHMAP::hash_map<std::string, std::string, MyHashFn> cache; //纠错cache
	std::map< std::set<std::string> ,std::vector<std::pair<int, std::vector<double> > >  > result_cache; //查询结果cache
	pthread_mutex_t mutex ,cache_mutex,Result_mutex; //互斥信号量
	pthread_cond_t cond1; 
	MyLock my_Lock , Cache_Lock,Result_Lock; //互斥锁
	MyCondition my_Con;
	Task *readhead , *readtail , *writehead ;
	struct epoll_event ev, events[20]; 
	int epfd; 
	
 	//class MyThread;
	std::vector< MyThread *> Th_Que ;
	

	std::string  Index_name ;
	std::string  word_fre_name ;
	std::string  cache_name ;
	std::string  result_name ;

	std::string ConIP;
	int ConPort,ListenQ,EpollQ;

	MYHASHMAP::hash_map<std::string, std::set<std::pair<int, double> >, MyHashFn> invert ;
	MYHASHMAP::hash_map<int, MyPage>  my_page_map;
	MYHASHMAP::hash_map<int, std::pair<int, int> > offset ;
	MySplit * mysplit;
	MySocket * mysocket;
}; 

#endif 



