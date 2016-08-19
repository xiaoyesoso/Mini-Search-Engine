#include "Thread.h"
#include "ThreadPool.h"

void Cache_Thread::setPThPool(ThreadPool * p)

{

	pThPool = p ;

}



void Cache_Thread::start() 

{
	pthread_create(&m_tid, NULL, call_back,this);

}



void* Cache_Thread::call_back(void *args)

{

	std::cout << "call_back" << std::endl;

	Cache_Thread * pThis = ( Cache_Thread *)args;
	pThis ->readtask();

}



void Cache_Thread::readtask()

{
	MYHASHMAP::hash_map<std::string, std::string, MyHashFn>::iterator it ;
	double th_end,th_start;
	th_start = clock();
	while(1)
	{
		th_end = (double)(clock() - th_start)/CLOCKS_PER_SEC;
		if(th_end * 1000 >= 50)   
		{
			int Qsize = pThPool->Th_Que.size();
			MYHASHMAP::hash_map<std::string, std::string, MyHashFn>::iterator it ;
			std::map< std::set<std::string> ,std::vector<std::pair<int, std::vector<double> > >  >::iterator re_it;

			for(int k = 0 ; k < Qsize ; ++ k)
			{


				//同步 Thread 和 ThreadPool 之间的 纠错 Cache
				pThPool->Cache_Lock.Lock();

				for(it = pThPool->Th_Que[k]->th_cache.begin() ; 
					it != pThPool->Th_Que[k]->th_cache.end() ; ++ it)
				{
					pThPool->cache[it->first] = it ->second; 
				}
				//std::cout<<"cache renew"<<std::endl;
				for(it = pThPool->cache.begin() ; it != pThPool->cache.end() ; ++ it)
				{
					pThPool->Th_Que[k]->th_cache[it->first] = it ->second; 
				}
				//std::cout<<k<<"   Thread  cache renew  "<<std::endl;
				pThPool->Cache_Lock.Unlock();


				//同步 Thread 与 ThreadPool 之间的 result Cache
				pThPool->Result_Lock.Lock();
				//std::cout << " Thread to ThreadPool " << std::endl;
				for(re_it = pThPool->Th_Que[k]->th_result_cache.begin() ; 
					re_it != pThPool->Th_Que[k]->th_result_cache.end() ; ++ re_it)
				{
					pThPool->result_cache[re_it->first] = re_it->second; 
				}
				//std::cout<<"result cache renew"<<std::endl;
				//std::cout << " ThreadPool to  Thread" << std::endl;
				for(re_it = pThPool->result_cache.begin() ; 
					re_it != pThPool->result_cache.end() ; ++ re_it)
				{
					pThPool->Th_Que[k]->th_result_cache[re_it->first] = re_it ->second;
				}
				//std::cout<<k<<"   Thread  result cache renew "<<std::endl;
				pThPool->Result_Lock.Unlock();	
			}


			//同步 ThreadPool 和 disk 之间的 纠错 Cache
			pThPool ->Cache_Lock.Lock(); 
			std::ofstream write_cache( pThPool ->cache_name.c_str() );
			if(!write_cache.is_open())
				perror("write_cache");
			for(it = pThPool ->cache.begin() ; it !=pThPool ->cache.end() ; ++ it)
			{
				write_cache << it ->first << "	" << it->second <<std::endl;
			}
			write_cache.close();
			//std::cout<<"disk_cache renew"<<std::endl;
			pThPool ->Cache_Lock.Unlock();	


			//同步 ThreadPool 和 disk 之间的 result Cache
			//格式 : SetSize	str1	str2	……	vector<pair>.size	vector<double>.size	int	double	double	…	int	double double	…………
			pThPool ->Result_Lock.Lock(); 
			std::ofstream write_result( pThPool->result_name.c_str() );
			if(!write_result.is_open())
				perror("write_result");
			for(re_it = pThPool ->result_cache.begin() ; re_it !=pThPool ->result_cache.end() ; ++ re_it)
			{
				int SetSize = re_it->first.size();
				write_result << SetSize;
				std::set<std::string>::iterator set_it;
				for(set_it = re_it->first.begin() ; set_it != re_it->first.end() ; 
					++ set_it)
					write_result << "	" << *set_it;
				int n = re_it->second.size();
				int m = re_it->second[0].second.size();
				write_result << "	" << n<<"	"<<m;
				for(int i = 0 ; i < n ; ++ i)
				{
					write_result << "	" << re_it->second[i].first;
					for(int j = 0 ; j < m ; ++j)
					{
						write_result << "	" << re_it->second[i].second[j];
					}
				}
				write_result << "	";
			}
			write_result.close();
			//std::cout<<"disk_cache renew"<<std::endl;
			pThPool ->Result_Lock.Unlock();	

			th_start = clock();
		}

	}



}




