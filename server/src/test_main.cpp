#include "MyConf.h"
#include "MyLock.h"
#include "MyCondition.h"
#include "User_Data.h"
#include "Chosed_Str.h"
#include "ThreadPool.h"
#include "MySocket.h"
#include "test.h"

int main(int argc, char* argv[])
{


	MyConf myconf("./conf/my.conf");
	myconf.Index_name = "../lib/index.lib";
	myconf.word_fre_name = "../lib/Word_Fre.lib";
	myconf.cache_name = "../lib/cache.lib";
	myconf.result_name = "../lib/result.lib";
	strcpy( myconf.ConIP ,"127.0.0.1");
	myconf.ConPort = 5080 ;
	myconf.ListenQ = 20 ;
	myconf.EpollQ = 20 ;
	myconf.init();


	MySplit txtsplit(myconf);
	txtsplit.set_type(0);
	MySplit querysplit(myconf);
	querysplit.set_type(1);
	MySplit* psplit = & txtsplit ;
	MYHASHMAP::hash_map<int, MyPage>  page_map ;
	load_page_map(myconf,txtsplit ,page_map);
	std::cout << "load over " << std::endl;

	
	
	psplit = &querysplit;
	ThreadPool th_Pool(myconf , 5 , psplit ,page_map);
	std::cout << "ThreadPool over " << std::endl;

	//初始化、启动Socket
	th_Pool.mysocket->init();
	std::cout << "mysocket->init over " << std::endl;
	th_Pool.mysocket->RunSocket();
	std::cout << "mysocket->RunSocket over " << std::endl;
	
	return 0 ;
}



