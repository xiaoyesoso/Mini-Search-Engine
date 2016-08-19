#include <stdio.h>
#include <string>
#include <vector>
#include <sstream>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<unistd.h>
#include<dirent.h>
#include<sys/stat.h>
#include<fcntl.h>

#include "NLPIR.h"

int cnt = 0;//文本编号

void Res(std::string dir)
{
	//生成路径
	std::stringstream ss ;
	std::string str;
	ss <<  "../../data/Chi/ReSource/"<< cnt << ".txt";
	ss >> str;
	//                分词前路径 ，分词后路径
	NLPIR_FileProcess(dir.c_str(),str.c_str(), 1);
	//增加编号
	++cnt;
}


void scan_dir(char* dir)
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
			scan_dir(name);
		}
		//否则 ， 如果是 txt 类型的文件
		else if( strcmp(type,"txt")== 0)
		{
			//分词
			Res(name);
            printf("%s\n",name);
		}

	}
	closedir(pdir);
}



int main(void)
{
	//初始化分词系统
	if(!NLPIR_Init("../", UTF8_CODE))
	{
		printf("init fails\n");
		return -1;
	}
	//DFS网页库 进行分词
	scan_dir("../../data/Chi/source");
	NLPIR_Exit();
	return 0;
}


