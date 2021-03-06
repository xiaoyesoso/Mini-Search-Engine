#ifndef __MYSPLIT_H__
#define __MYSPLIT_H__
#include <string>
#include "ICTCLAS50.h"
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <sstream>
#include "MyConf.h"
//分词类，把一篇文档分词去除停用词
class MySplit
{
public:
	//带参数的构造函数，定义一个该类的对象时需传入一个配置对象来初始化该类的一个引用成员
	MySplit( MyConf& conf):m_conf(conf)
	{
		//初始化分词程序，该分词库是使用的中科院的分词库，初始化时需传入分词库的配置文件的路径
		if( !ICTCLAS_Init(m_conf.m_conf["myictclas"].c_str() ) )
		{
			std::cout << "ICTCLAS Init faile!" << std::endl ;
			exit(0);
		}
	}
	//重载函数调用操作符，出入一个字符串类型的数据来分词
	std::vector<std::string>  operator()(const char*  src)
	{
		return do_some(src) ;
	}			
	//在析构函数中释放分词库所占用的资源
	~MySplit()
	{
		ICTCLAS_Exit();
	}

	void set_type(int type)
	{
		my_type = type;
	}

private:
	//一个配置对象的引用
	MyConf& m_conf ;
	int my_type;
	//判断一个词是不是停用词
	bool is_stopword( std::string& word )
	{
		int flag = 0 ;
		MYHASHMAP::hash_map<std::string, std::string, MyHashFn>::iterator iter ;
		for(iter = m_conf.m_stop.begin(); iter != m_conf.m_stop.end(); iter ++)
		{
			if(word.find(iter -> first) != std::string::npos)
			{
				flag = 1 ;
				break ;
			}
		}
		if(flag == 0 && word[0] != '\r' )
		{
			return false ;
		}else 
		{
			return true ;
		}
	} 

	//判断这个分词是否单个字
	bool isAlone(std::string word)
	{
		//如果是英文 ， 则返回 flase ；
		if(word[0] >= 0 && word[0] <= 128 ) return false ;

		int word_len = word.length() ;
		int len ;
		if(word[0] & (1 << 4))
			len = 4;
		else if(word[0] & (1 << 5))
			len = 3;
		else if(word[0] & (1 << 6))
			len = 2;
		else len = 1;
		//std::cout << "len:" << len<<"     word_len:" <<word_len <<std::endl;
		if(len == word_len) return true;
		else  return false;
	}

	//实际处理分词的逻辑，首先使用分词程序对传入数据进行分词，然后对分词后的数据去除停用词最后将去除停用词后的结果
	//放入一个vector中去返回
	std::vector<std::string>  do_some(const char*  src)
	{
		int src_len = strlen(src);
		char* dest = new char[6 * src_len]() ;
		ICTCLAS_ParagraphProcess(src, src_len, dest, CODE_TYPE_UTF8, 0);
		//std::string str_dest(dest);

		//printf("dest :%s\n",dest);
		std::istringstream sin(dest);
		std::string word ;
		std::vector<std::string> word_vec ;
		bool B_merge = false;
		std::string Str_merge = "";
		int Num_merge = 0; //合并的字的个数 
		if(my_type == 0)
		{
			//std::cout << "my_type:   " << my_type << std::endl; 
			while(sin >> word)
			{
				//std::cout << "word:   " <<  word << std::endl; 

				if(!m_conf.m_stop.count(word) && word[0] != '\r')
				{
					word_vec.push_back(word);
				}
			}		
		}
		else
		{
			//std::cout << "my_type:   " << my_type << std::endl; 

			while(sin >> word)
			{
				std::cout << "word:   " <<  word << std::endl; 

				if(!m_conf.m_stop.count(word) && word[0] != '\r')
				{
					//把连续的单个字 加起来
					if(isAlone(word))
					{
						B_merge = true ;
						Str_merge += word;
						++Num_merge ;
						std::cout << "Str_merge:   " <<  Str_merge << std::endl; 
					}
					else
					{
						//如果之前出现单个字
						if(B_merge)
						{
							//如果只有一个字，将它并入左边的词语中
							if(Num_merge == 1 )
							{
								word_vec[word_vec.size() -1 ] += Str_merge; 
								std::cout << word_vec[word_vec.size() -1 ] << std::endl;
							}
							else 
							{
								word_vec.push_back(Str_merge); 
								std::cout << Str_merge << std::endl; 
							}
							//还原
							B_merge = false;
							Str_merge = "";
							Num_merge = 0 ;
						}
						
						word_vec.push_back(word);
						
					}

				}
			}
			//如果循环结束后 Str_merge != "" ,说明 最后几个是单个字，组合后还未加入数组
			if(Str_merge != "")
				word_vec.push_back(Str_merge);
		}

		if(src_len == 1)
		{
			delete dest ;
		}else {
			delete [] dest ;
		}
		return word_vec ;
	}

};
#endif

