#include "Thread.h"
#include "ThreadPool.h"

void MyThread::setPThPool(ThreadPool * p)
{
	std::cout << "set 1" << std::endl;
	pThPool = p ;  
	std::cout << "set 2" << std::endl;
	th_cache = p ->cache ;
	std::cout << "set down" << std::endl;
}


void MyThread::start()
{
	//生成线程 调用 回调函数
	std::cout << "pthread_create(&m_tid, NULL, call_back,this);" << std::endl;
	pthread_create(&m_tid, NULL, call_back,this);
}

//回调函数
void* MyThread::call_back(void *args)
{
	std::cout << "call_back" << std::endl;
	MyThread* pThis = (MyThread*)args;
	//回调类内的 readtask 函数
	pThis ->readtask();
}


bool MyThread::find_txt(std::vector<std::string>& qurey_vec, std::vector<std::pair< int, std::vector<double> > >& res_vec)
{
	int index_qurey_vec  ;
	for(index_qurey_vec = 0 ; index_qurey_vec != qurey_vec.size(); index_qurey_vec ++)
	{
		//有词语不存在就退出循环
		if(pThPool->invert.count(qurey_vec[index_qurey_vec]) == 0 )
		{
			break ;
		}
	}
	std::cout << "如果有词语不存在 则 返回 false" << std::endl;
	if(index_qurey_vec != qurey_vec.size())
	{
		std::cout << "return false" << std::endl;
		return false ;
	}
	
	else 
	{
		//                      docid  , 权值
		typedef std::set<std::pair<int, double> >::iterator  set_iterator_first ;
		//                                    pair编号                      
		typedef std::pair< set_iterator_first, int >  set_iterator ; 
		
		//num_set_iterator  为 qurey_vec 的大小
		int num_set_iterator = qurey_vec.size() ;
		set_iterator* iter_arr = new set_iterator[ num_set_iterator ] ;
		int min_size = 0x7fffffff ;
		for(index_qurey_vec = 0 ; index_qurey_vec != num_set_iterator; index_qurey_vec ++)
		{
			//std::cout << " W1 " << std::endl;
			iter_arr[index_qurey_vec].first = pThPool->invert[qurey_vec[index_qurey_vec]].begin();
			iter_arr[index_qurey_vec].second = 0 ;
			int cur_size = pThPool->invert[qurey_vec[index_qurey_vec]].size();
			//选择 最小的集合
			if(cur_size < min_size)
			{
				min_size = cur_size ;
			}
		}
		int break_flag = 0 ;
		int index_set_iterator ;
		while(!break_flag)
		{
			//判断各个 集合 迭代器所所指向的文章是否相同
			for(index_set_iterator = 0 ; index_set_iterator < num_set_iterator - 1; index_set_iterator ++)
			{
				//std::cout << " W2 " << std::endl;
				if( (iter_arr[index_set_iterator].first)->first != (iter_arr[index_set_iterator + 1].first ) -> first )
				{
					break ;
				}
			}


			if(index_set_iterator == num_set_iterator - 1)
			{
				//相同，则   所有迭代器往后移动
				std::vector<double> weight_vec ;
				int docid = (iter_arr[0].first)->first ;
				for(index_set_iterator = 0 ; index_set_iterator != num_set_iterator ; index_set_iterator ++)
				{
					//权值
					weight_vec.push_back( (iter_arr[index_set_iterator].first)->second   );
					//迭代器往后移动
					//std::cout << " P1 " << std::endl;
					if(iter_arr[index_set_iterator].second  >= min_size )
					{
						break_flag = 1 ;
						continue;
					}
					else
					{
						(iter_arr[index_set_iterator].first ) ++  ;
						(iter_arr[index_set_iterator]. second) ++  ;
					}

					//移动到最后一个pair 结束
					if(iter_arr[index_set_iterator].second  >= min_size )
					{
						break_flag = 1 ;
					}
				}
				//std::cout << "将结果加入 res_ves" << std::endl;
				res_vec.push_back(make_pair(docid, weight_vec));

			}else 
			{
				//找到最小的docid，该docid位置的游标往后移动
				int min_docid = 0x7fffffff ;
				int max_docid = -1 ;
				int min_index ,max_index;
				for(index_set_iterator = 0 ; index_set_iterator != num_set_iterator; index_set_iterator ++)
				{
					if( (iter_arr[index_set_iterator].first )->first < min_docid )
					{
						min_docid = (iter_arr[index_set_iterator].first) -> first ;
						min_index = index_set_iterator ;
						
					}
					
					//if((iter_arr[index_set_iterator].first )->first > max_docid)
					//{
					//	max_docid = (iter_arr[index_set_iterator].first) -> first ;
					//	max_index = index_set_iterator ;	
					//}
				}
				//std::cout << " P2 " << std::endl;
				if(iter_arr[min_index].second >=  (pThPool->invert[qurey_vec[min_index]]).size()  )
				{
					break_flag = 1 ;
					continue;
				}
				else
				{
				  iter_arr[min_index].first ++ ;
				  iter_arr[min_index].second ++ ;
				}
				//std::cout << " second : " << iter_arr[min_index].second << " size :"  <<  (pThPool->invert[qurey_vec[min_index]]).size()  << std::endl;
				//最小的docid 移动到最后一个pair 结束
				if(iter_arr[min_index].second >=  (pThPool->invert[qurey_vec[min_index]]).size()  )
				{
					break_flag = 1 ;
				}
			}
		}

			if(num_set_iterator != 1)
			{

		        delete [] iter_arr ;
				std::cout << "delete [] iter_arr ;" << std::endl;
			}else 
			{
				delete iter_arr ;
				std::cout << "delete iter_arr ;" << std::endl;
			}
		if(res_vec.size() == 0 )
			return false;
		else return true ;
	}

}

//执行查询逻辑：
//该函数需要四个参数，第一个为一个配置对象的引用，第二个为分词对象的引用，第三个为保存着网页库文档的hash_map，第四个为要查询的语句
//首先将查询语句进行分词去停用词，获得一个词组，计算该词组的每个词的权重（通过 TF*IDF），然后根据
//网页库的倒排索引（已经提前加载到内存）,找出包含查询词组的各个文档，然后通过计算找到的每个文档与查询语句（将查询语句当成一篇文档）的余弦相似度
//根据这个余弦相似度给找到的文档集合按照从大到小排个序（余弦值越大，相似性越高），最后将结果封装成json格式的数据返回 。
//用于保存查询结果的集合，该vector保存着包含查询词组的文档向量
//	std::vector<std::pair<int, std::vector<double> > > res_vec ;
	//用查询词组的权重初始化一个文档相似度比较对象，用于后期排序处理
std::string MyThread::excute_qurey(std::vector<std::string> qurey_vec ,std::vector<std::pair<int, std::vector<double> > > & res_vec )
{
	std::cout << "begin excute" << std::endl;
	//查询词组的权重  
	std::vector<double> weight_vec ;
	//查询词组的词和词频 ，用于后期计算权重
	MYHASHMAP::hash_map<std::string, int ,MyHashFn> freq_map ;

	int index_qurey_vec ;
	//统计词和词频
	for(index_qurey_vec = 0 ; index_qurey_vec != qurey_vec.size(); index_qurey_vec ++ )
	{
		freq_map[ qurey_vec[index_qurey_vec]] ++ ;
	}
	//cout qurey word weight
	//计算查询词组权重
	double sum_weight  = 0 ;
	int sum_page = pThPool->offset.size() ;
	for(index_qurey_vec = 0 ; index_qurey_vec != qurey_vec.size(); index_qurey_vec ++ )
	{
		int df = pThPool->invert[ qurey_vec[index_qurey_vec] ].size() ;
		double idf = log( (double)(sum_page) / df  + 0.05  ) / log(2) ;
		int tf = freq_map[qurey_vec[index_qurey_vec]];
		sum_weight += pow( tf * idf , 2) ;
		weight_vec.push_back(tf * idf);
	}
	int index_weight_vec ;
	for(index_weight_vec = 0 ; index_weight_vec != weight_vec.size(); index_weight_vec ++ )
	{
		weight_vec[index_weight_vec] /= sqrt( sum_weight ) ;
	}
	MyCompareDoc acomparedoc(weight_vec);
	std::cout << "MyCompareDoc built" << std::endl;
	//找出包含查询词组的所有文档，并返回这些文档的向量，文档的向量表示为：
	//docid : weight1, weight2 ...
	//其中docid为文档的id，weighti为查询词组的第i个查询词在文档id为docid的权重	
	if( find_txt(qurey_vec, res_vec ) )
	{
		std::cout << "find_txt" << std::endl;
		//对查找出的结果根据文档相似度比较函数对结果排个序
		stable_sort(res_vec.begin(), res_vec.end(), acomparedoc);
		//将结果封装成json格式的数据并返回
		MyJson ajson ;
		return ajson(res_vec,pThPool->my_page_map);
	}else//no result 
	{
		std::cout << "no result " << std::endl ;
		std::string tem = "Not Found";
		return tem ;
	}

}



void  MyThread::readtask()
{
	int fd = -1; 
	unsigned int n; 
	//用于把读出来的数据传递出去 
	User_Data *data = NULL; 
	MYHASHMAP::hash_map<std::string, std::string, MyHashFn>::iterator it ;
	while (1) 
	{ 
		pThPool->my_Lock.Lock();
		//等待到任务队列不为空 
		while (pThPool->readhead == NULL) 
			pThPool->my_Con.Wait();

		fd = pThPool->readhead->fd; 
		//从任务队列取出一个读任务 
		Task *tmp = pThPool->readhead; 
		pThPool->readhead = pThPool->readhead->next; 
		delete tmp; 
		pThPool->my_Lock.Unlock();

		data = new User_Data(); 
		data->set_fd(fd); 
		char Data_Line[MAXLINE] ;
		//1、读取错误
		if ((n = read(fd, Data_Line, MAXLINE)) < 0) 
		{ 
			std::cout << "readline error" << std::endl; 
			std::string reslut =  "readline error";	
			std::cout << reslut <<std::endl;
			data->set_result(reslut);
			//设置需要传递出去的数据 
			pThPool->ev.data.ptr = data; 
			//设置用于注测的写操作事件 
			pThPool->ev.events = EPOLLOUT | EPOLLET; 
			//修改sockfd上要处理的事件为 EPOLLOUT 
			epoll_ctl(pThPool->epfd, EPOLL_CTL_MOD, fd, &pThPool->ev); 
			continue;
		}
		//2、无客户端
		else if (n == 0)                        
		{ 
			std::cout << "Client close connect!" << std::endl; 
			std::string reslut =  "Client close connect!";	
			std::cout << reslut <<std::endl;
			data->set_result(reslut);
			//设置需要传递出去的数据 
			pThPool->ev.data.ptr = data; 
			//设置用于注测的写操作事件 
			pThPool->ev.events = EPOLLOUT | EPOLLET; 
			//修改sockfd上要处理的事件为 EPOLLOUT 
			epoll_ctl(pThPool->epfd, EPOLL_CTL_MOD, fd, &pThPool->ev); 
			continue;
		}
		//3、正常情况
		else 
		{ 
			printf("Data_Line : %s\n",Data_Line);

			data ->Line_Copy(Data_Line);
			std::vector<std::string> correct_reslut; 

			//分词	
			correct_reslut = (*pThPool->mysplit)(Data_Line) ;
			int len_reslut = correct_reslut.size() ;

			//先查询result_cache 
			std::set<std::string > bf_reslut_set ;
			for(int k = 0 ; k < len_reslut ; ++ k )
			{
				bf_reslut_set.insert(correct_reslut[k]);
			}

			//如果result_cache 中存在 , 则直接接触这次循环
			if(th_result_cache.count(bf_reslut_set)!= 0)
			{
				std::cout << "Hit th_result_cache" << std::endl; 	
				MyJson ajson ; 
				std::string result = ajson(th_result_cache[bf_reslut_set],pThPool->my_page_map);  
				std::cout << result <<std::endl;
				data->set_result(result);
				//设置需要传递出去的数据 
				pThPool->ev.data.ptr = data; 
				//设置用于注测的写操作事件 
				pThPool->ev.events = EPOLLOUT | EPOLLET; 
				//修改sockfd上要处理的事件为 EPOLLOUT 
				epoll_ctl(pThPool->epfd, EPOLL_CTL_MOD, fd, &pThPool->ev); 
				continue ;
			}
			


			for(int i = 0 ; i < len_reslut ; ++ i)
			{
				std::cout << "分词 " << i+1 << ":" << correct_reslut[i] << std::endl;
			}


			//1 对每个词语进行纠错
			for(int i = 0 ; i < len_reslut ; ++ i)
			{

				if(th_cache.count(correct_reslut[i])!= 0)
				{
					std::cout << "Hit Cache" << std::endl; 					
					std::cout << "before correct : " << correct_reslut[i] <<". after correct : " << th_cache[ correct_reslut[i] ] << std::endl;					correct_reslut[i] = th_cache[ correct_reslut[i] ];
				}
				else
				{
					std::string str_tem =  RunTask(correct_reslut[i]);
					std::cout << "Read Memory" << std::endl; 
					std::cout << "before correct : " << correct_reslut[i] <<". after correct : " << str_tem << std::endl;

					//更新 th_cache
					pThPool->Cache_Lock.Lock();
					th_cache[correct_reslut[i]] = str_tem ;					
					pThPool->Cache_Lock.Unlock();
					correct_reslut[i] = str_tem;

				}
			}




			//纠错后再次 查询result_cache 
			std::set<std::string > at_reslut_set ;
			at_reslut_set.clear() ;
			for(int k = 0 ; k < len_reslut ; ++ k )
			{
				at_reslut_set.insert(correct_reslut[k]);
			}
			//如果result_cache 中存在 , 则直接接触这次循环
			if(th_result_cache.count(at_reslut_set)!= 0)
			{
				std::cout << "Hit th_result_cache" << std::endl; 	
				MyJson ajson ; 
				std::string result = ajson(th_result_cache[at_reslut_set],pThPool->my_page_map);  
				std::cout << result <<std::endl;
				data->set_result(result);
				//设置需要传递出去的数据 
				pThPool->ev.data.ptr = data; 
				//设置用于注测的写操作事件 
				pThPool->ev.events = EPOLLOUT | EPOLLET; 
				//修改sockfd上要处理的事件为 EPOLLOUT 
				epoll_ctl(pThPool->epfd, EPOLL_CTL_MOD, fd, &pThPool->ev); 
				continue ;
			}


			//查询结果集合
			std::vector<std::pair<int, std::vector<double> > >  res_vec ;
			//2 纠错后 开始查询
			std::string reslut =  excute_qurey(correct_reslut ,res_vec);	
			std::cout << reslut <<std::endl;
			if(res_vec.size() != 0)
			{
			//更新th_result_cache
			pThPool->Result_Lock.Lock();
			th_result_cache[bf_reslut_set] = res_vec ;
			th_result_cache[at_reslut_set] = res_vec ;
			pThPool->Result_Lock.Unlock();
			}

			data->set_result(reslut);
			//设置需要传递出去的数据 
			pThPool->ev.data.ptr = data; 
			//设置用于注测的写操作事件 
			pThPool->ev.events = EPOLLOUT | EPOLLET; 
			//修改sockfd上要处理的事件为 EPOLLOUT 
			epoll_ctl(pThPool->epfd, EPOLL_CTL_MOD, fd, &pThPool->ev); 
		} 
	} 
}


// 编辑距离算法
int MyThread::calcDidstanc(std::vector<std::string> pstr1, int len1, std::vector<std::string> pstr2, int len2)
{
	//初始化
	for(int i = 0; i <= len1; ++i)
	{
		dp[i][0] = i;
	}

	for(int j = 0; j <= len2; ++j)
	{
		dp[0][j] = j;
	}

	for(int i = 1; i <= len1; ++i)
	{
		for(int j = 1; j <= len2; ++j)
		{
			if(pstr1[i - 1] == pstr2[j - 1])//最后字符相同
			{
				dp[i][j] = dp[i - 1][j - 1];
			}
			else
			{
				int t1 = dp[i - 1][j];        // 删除X第i个字符
				t1 = t1 < dp[i][j - 1] ? t1 : dp[i][j - 1];//和 删除Y第j个字符，取小
				t1 = t1 < dp[i - 1][j - 1] ? t1 : dp[i - 1][j - 1];// 和 最后字符改相同，取小
				dp[i][j] = t1 + 1;
			}
		}
	}

	return dp[len1][len2];
}


//将汉语 词语 分成  字
std::vector<std::string> MyThread::StrToVec(std::string word)
{
	int w_len = word.length();
	std::vector<std::string> Str_Vec;
	std::string key ;
	for(int  i = 0 ;i < w_len ; ++ i)
	{
		if(word[i] >= 0 && word[i] <= 128) //英文字母、数字
			key = word.substr(i, 1);

		//  汉字
		else if(word[i] & (1 << 4))		
		{
			key = word.substr(i, 4);
			i = i + 3;
		}
		else if(word[i] & (1 << 5))
		{
			key = word.substr(i, 3);
			i = i + 2;
		}
		else if(word[i] & (1 << 6))
		{
			key = word.substr(i, 2);
			i = i + 1;
		}
		else
		{
			key = word.substr(i, 1);
		}

		Str_Vec.push_back(key);
	}

	return Str_Vec;

}


//纠错模块
std::string MyThread::RunTask(std::string  Data_Str )
{
	char Data_line[MAXLINE];
	strcpy( Data_line,Data_Str.c_str() );

	std::vector<std::string> name = StrToVec(Data_line);
	int len = name.size();
	std::set<std::string>::iterator it ;
	std::priority_queue<Chosed_Str>  chosed_queue;  
	std::set<std::string>  chosed_set; //不重复的保存词语中的字

	for(int i = 0 ; i < len ; ++ i)
	{
		std::set<std::string> Compare_Set = pThPool->Index[name[i]];
		std::cout << "name[i] :" << name[i] << "  Compare_Set.size() :" << Compare_Set.size() << std::endl;
		for(it = Compare_Set.begin() ; it != Compare_Set.end() ; ++ it)
		{
			if(chosed_set.count(*it) )
				continue;
			chosed_set.insert(*it);

			std::vector<std::string> str_vec_tem = StrToVec(*it);
			int tem_len = str_vec_tem.size();

			int dis = calcDidstanc(name,len,str_vec_tem,tem_len) ;
			if( dis < 4 )
			{
				Chosed_Str tem(*it,pThPool->word_fre[*it],dis) ;
				chosed_queue.push(tem);
			}
		}
	}	
	Chosed_Str tem =  chosed_queue.top();
	return tem.get_str() ;
}


