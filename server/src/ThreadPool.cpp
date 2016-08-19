
#include "ThreadPool.h"
#include "Thread.h"


ThreadPool::ThreadPool(MyConf &myconf ,const int Thread_Num,MySplit * txtsplit , MYHASHMAP::hash_map<int, MyPage>&  page_map)
	:my_Lock(mutex),Cache_Lock(cache_mutex),Result_Lock(Result_mutex),my_Con(cond1,mutex),
	Index(myconf.Index),word_fre(myconf.word_fre),cache(myconf.cache),result_cache(myconf.result_cache),
	Index_name(myconf.Index_name),word_fre_name(myconf.word_fre_name),cache_name(myconf.cache_name),result_name(myconf.result_name),
			ConIP(myconf.ConIP),ConPort(myconf.ConPort),ListenQ(myconf.ListenQ),EpollQ(myconf.EpollQ),
				invert(myconf.m_invert),offset(myconf.m_offset),my_page_map(page_map),
					mysplit(txtsplit)
				
{
	std::cout << "Index Size IN ThreadPool is " << Index.size() << std::endl;
	mysocket = (MySocket*)malloc(sizeof(MySocket));
	mysocket->PthPool = this ;
	strcpy( mysocket->MyIP , ConIP.c_str() );
	mysocket->MyPort = ConPort;
	mysocket->MyListenQ = ListenQ;
	mysocket->MyEpollQ = EpollQ;
	readhead = readtail = writehead = NULL;
	for(int i = 0 ;i < Thread_Num ; i++)
	{
		//MyThread* ptem = (MyThread*)malloc(sizeof(MyThread));
		MyThread* ptem = new MyThread();
		std::cout << "Thread Create" << std::endl;
		ptem ->setPThPool(this);
		//启动线程
		ptem ->start() ;
		std::cout << "Thread Start" << std::endl;

		Th_Que.push_back(ptem);
		std::cout << "Create " << i + 1 << "Thread" << std::endl;
	}
	Cache_Thread* pTP = new Cache_Thread();
	pTP ->setPThPool(this);
	pTP ->start() ;

}


