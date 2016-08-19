#define MYHASHMAP __gnu_cxx
#include<stdio.h>
#include<iostream>
#include<fstream>
#include<string>
#include<ctype.h>
#include<sstream>
#include<map>
#include<set>
#include <vector>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<unistd.h>
#include<dirent.h>
#include<sys/stat.h>
#include <ext/hash_map>

class MyHashFn
{
	public:
		std::size_t operator()(const std::string& str) const
		{
			return MYHASHMAP::__stl_hash_string(str.c_str()) ;
		}
};



MYHASHMAP::hash_map<std::string, int, MyHashFn> word_fre;   //词频

MYHASHMAP::hash_map<std::string, std::set<std::string>, MyHashFn> Index; //单词索引


//进行英文语料分词
void Eng_Lib_Read(std::string name)
{
	//读取打开文本
	std::ifstream fin(name.c_str());
	if(! fin.is_open())
	{
		perror(name.c_str());
	}
	else
	{
		std::cout <<name<<" OPENED" << std::endl;
	}
	std::string line;
	
	while(std::getline(fin,line))
	{
		for(int i = 0 ;i < line.length() ; i++)
		{
			
			//如果不是字母 数组 ，则变空格
			if(!isalnum(line[i]))
			{
				line[i] = ' ';
			}
			//大写变小写
			else if(line[i] >= 'A' && line[i] <= 'Z')
				line[i] = line[i] -  'A' + 'a';
		}						

		std::stringstream ss;
		ss<<line;
		std::string word;
		while(ss >> word)
		{
			int w_len = word.length();
			//建立单词索引
			for(int  i = 0 ;i < w_len ; ++ i)
			{
				std::string key = "";
				key =  key + word[i];
				//std::cout << "key = " << key << std::endl; 
				Index[key].insert(word);
			}
			//建立词频
			++word_fre[word]; 
		}
	}
}

//进行中文语料分词 
void Cha_Lib_Read(std::string name)
{
	std::ifstream fin(name.c_str());
	if(! fin.is_open())
	{
		perror(name.c_str());
	}
	else
	{
		std::cout <<name<<" OPENED" << std::endl;
	}
	std::string line;
	while(std::getline(fin,line))
	{
		int line_len = line.length() ;
		for(int i = 0 ;i < line_len ; i++)
		{
			//遇见‘/’后 ， 在遇见‘ ’前的字符 都是变成‘ ’
			//例：“杭州市/ns 长春/nz 药店/n 。”

			if(line[i] == '/')
 			{
			    while(i < line_len && line[i] != ' ')
					line[i++] = ' ';			
			}
		}						

		std::stringstream ss;
		ss<<line;
		std::string word;
		while(ss >> word)
		{
			int w_len = word.length();
			printf("%s\n",word.c_str());
			std::string key ;
			int fir = 1;
			/*
			扣词语中的汉字
			
			utf-8可以根据字的第一个字节移位推出长度的

            0xxxxxxx

            110xxxxx 10xxxxxx

            1110xxxx 10xxxxxx 10xxxxxx

            11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
			*/
			for(int  i = 0 ;i < w_len ; ++ i)
			{
				if(word[i] & (1 << 4))
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
				else key = word.substr(i, 1);
				
				//如果是第一个字 而且 i 移动到 词语最后一个字节
				if(fir && i == w_len -1 )  continue;  //去除单个字
				else fir = 0 ;
				printf("%s\n",key.c_str());
				Index[key].insert(word);
			}
			++word_fre[word]; 
		}
	}
}


//遍历语料文件  scan_dir(char* dir,int val)
//                        语料地址  语料类型
void scan_dir(char* dir,int val)
{
	//打开目录
	DIR* pdir = opendir(dir);
	char name[512] ="" ;//用来保存文本文件路径
	if(pdir == NULL)
	{
		perror("opendir");
		exit(1);
	}
	struct dirent* pent ;//目录文件信息
	struct stat mystat ;//文件状态信息
	
	//读目录文件信息
	while( (pent = readdir(pdir)) != NULL )
	{
		//排除 该层 和 上一层
		if (strcmp(".", pent ->d_name) == 0 || \
			strcmp("..", pent ->d_name) == 0 \
			)
		{
			continue ;
		}
		
		
		memset(name, 0, 512);
		//拼成路径
		sprintf(name, "%s/%s", dir, pent ->d_name);
		
		//读取文本的格式 （“txt”）
		char type[5] ;
		int len = strlen(name);
		int loc = 0;
		for(int i = len-3 ; i <= len ; i++)
			type[loc++] = name[i];
		
		
		memset(&mystat, 0, sizeof(mystat));
		//获取文件的状态信息
		if(lstat(name, &mystat ) == -1)
		{
			perror("lstat");
			exit(1) ;
		}
		
		//如果是 目录文件 ，那么进行 DFS
		if(S_ISDIR(mystat.st_mode))
		{
			scan_dir(name,val);
		}
		//否则 ， 如果是 txt 类型的文件
		else if( strcmp(type,"txt")== 0)
		{
			if(val == 1) //进行英文语料分词
				Eng_Lib_Read(name);
			else    //进行中文语料分词 
				Cha_Lib_Read(name);

			printf("%s\n",name);
		}

	}
	//关闭pdir
	closedir(pdir);
}


int main()
{
	//遍历英文语料文件
	scan_dir("./data/Eng",1);
	//遍历中文语料文件
	scan_dir("./data/Chi/ReSource",2);

	//打开 写 文件流
	std::ofstream Word_Fre_out("../../lib/Word_Fre.lib");
	std::ofstream Index_out("../../lib/index.lib");
	
	//写 词频库
	MYHASHMAP::hash_map<std::string, int, MyHashFn>::iterator wit;
	for( wit = word_fre.begin() ; wit != word_fre.end() ; ++ wit)
	{
	   Word_Fre_out << wit->first <<  "	" << wit->second <<std::endl;
	}

	//写 词语索引库
	MYHASHMAP::hash_map<std::string, std::set<std::string> ,MyHashFn>::iterator init;
	for(init = Index.begin() ; init != Index.end() ; ++init)
	{
		Index_out << init->first <<"	"<< (init->second).size(); 
		std::set<std::string>::iterator setit;
		for(setit = (init->second).begin() ; setit != (init->second).end() ; ++ setit)
			Index_out << "	" << *setit;
		Index_out << std::endl;
	}

	return 0 ;
}






