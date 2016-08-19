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
class MySplit
{
	public:
		MySplit( MyConf& conf):m_conf(conf)
		{
			//初始化分词系统
			if( !ICTCLAS_Init(m_conf.m_conf["myictclas"].c_str() ) )
			{
				std::cout << "ICTCLAS Init faile!" << std::endl ;
				exit(0);
			}
		}
		std::vector<std::string>  operator()(const char*  src)
		{
			return do_some(src) ;
		}			
		~MySplit()
		{
			ICTCLAS_Exit();
		}

	private:
		MyConf& m_conf ;
		std::vector<std::string>  do_some(const char*  src)
		{
			int src_len = strlen(src);
			char* dest = new char[6 * src_len]() ;
			ICTCLAS_ParagraphProcess(src, src_len, dest, CODE_TYPE_UTF8, 0);
			std::istringstream sin(dest);
			std::string word ;
			std::vector<std::string> word_vec ;
			while(sin >> word)
			{
				     //去停用词
				if(!m_conf.m_stop.count(word) && word[0] != '\r')
				{
					word_vec.push_back(word);
				}
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
