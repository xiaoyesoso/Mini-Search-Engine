#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <fstream>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdexcept>
#include <unistd.h>
#include <string.h>
#include <map>
#include <utility>
#include <set>
#include <functional>
#include <algorithm>
#include <ext/hash_map>
#define MYHASHMAP __gnu_cxx

class DirScan
{
	public:
		DirScan(std::vector<std::string>& vec):m_vec(vec)
		{
		}
		void operator()(const std::string &dir_name)
		{
			traverse(dir_name);
		}
		void traverse(const std::string& dir_name)
		{
			DIR* pdir = opendir(dir_name.c_str());
			if(pdir == NULL)
			{
				std::cout << "dir open" << std::endl ;
				exit(-1);
			}
			chdir(dir_name.c_str());
			struct dirent* mydirent ;
			struct stat mystat ;
			while((mydirent =readdir(pdir) ) !=NULL)
			{
				stat(mydirent->d_name, &mystat);
				if(S_ISDIR(mystat.st_mode))
				{
					if(strncmp(mydirent->d_name, ".", 1)== 0 || strncmp(mydirent->d_name,"..", 2) == 0)
					{
						continue ;
					}else 
					{
						traverse(mydirent->d_name);
					}
				}else 
				{
					std::string file_name="";
					file_name = file_name + getcwd(NULL,0)+"/"+ mydirent->d_name ;
					m_vec.push_back(file_name);
				}
			}
			chdir("..");
			closedir(pdir);
		}
	private:
		std::vector<std::string>& m_vec ; // 文件路径
};
class FileProcess
{
	public:
		FileProcess(std::vector<std::string>& vec, std::string& str):m_vec(vec)
		{
			m_title = str ;
		}
		void operator()(const std::string &file_name, const std::string & offset_file)
		{
			
			do_some(file_name, offset_file) ;
		}
		void do_some(const std::string& file_name, const std::string& offset_file)
		{
			//打开文件
			FILE* fp = fopen(file_name.c_str(),"w");
			FILE* fp_offset = fopen(offset_file.c_str(), "w");
			if(fp == NULL || fp_offset == NULL)
			{
				std::cout << "file open" << std::endl ;
				exit(0); 
			}
			int index ;
			char* mytxt = new char[1024*1024]() ; //格式化网页
			int mydocid ;  //网页号
			char myurl[256] = "" ; //网页URL
			char* mycontent = new char[1024 * 1024]() ; //网页内容
			char* mytitle = new char[1024]() ; //网页标题
			for(index = 0 ; index != m_vec.size(); index ++)
			{
				memset(mytxt, 0, 1024*1024);
				memset(myurl, 0, 256);
				memset(mycontent, 0, 1024* 1024);
				memset(mytitle, 0, 1024);
				FILE * fp_file = fopen(m_vec[index].c_str(), "r");
				//读取网页内容 和 标题
				read_file(fp_file, mycontent, mytitle);
				fclose(fp_file);
				mydocid = index + 1 ;
				strncpy(myurl, m_vec[index].c_str(), m_vec[index].size());
				//格式化网页格式：<doc><docid>网页号</docid><docurl>网页URL</docurl><doctitle>网页标题</doctitle><doccontent>网页内容</doccontent></doc>\n
				sprintf(mytxt, "<doc><docid>%d</docid><docurl>%s</docurl><doctitle>%s</doctitle><doccontent>%s</doccontent></doc>\n", mydocid, myurl, mytitle, mycontent);
				int myoffset = ftell(fp);//格式化网页偏移
				int mysize = strlen(mytxt);//格式化网页大小
				char offset_buf[128]="";
			    //写文件
				fprintf(fp_offset,"%d\t%d\t%d\n",mydocid, myoffset, mysize);
				write_to_file(fp, mytxt); 
			}
			fclose(fp);
		}
		
		//读取网页内容 和 标题
		void read_file(FILE* fp ,  char* mycontent, char* mytitle )
		{
			int iret ;
			const int size = 1024 * 1024 ;
			char* line = new char[1024]() ;
			int pos = 0 ;
			
			//读取fp 全篇文件
			while(1)
			{
				int iret = fread( mycontent + pos, 1, size - pos, fp);
				if(iret == 0)
				{
					break ;
				}else 
				{
					pos += iret ;
				}
			}
			rewind(fp) ;
			int count = 0, flag = 0 ; ;
			//前十行 且 未读完文件
			//如果能找到 “标  题” ，这一行为 mytitle
			while(count <=10 && fgets(line, 512, fp) != NULL)
			{
				std::string str_line(line);
				if( str_line.find(m_title.c_str(), 0) != std::string::npos)
				{
					strncpy(mytitle, str_line.c_str(), str_line.size());
					flag = 1 ;
					break ;
				} 
				count ++ ;
					
			}
			//如果不能找到,回到开头，前1024字节为标题
			if( flag == 0)
			{
				rewind(fp);
				fgets(mytitle,1024, fp );
			}	
		}
		
		
		
		void write_to_file(FILE* fp, char* mytxt)
		{
			int iret , pos = 0 ;
			int len = strlen(mytxt);
			while(1)
			{
				iret = fwrite(mytxt + pos, 1, len - pos, fp);
				len = len - iret ;
				if(len == 0)
				{
					break ;
				}  
			}
		}
	private:
		std::vector<std::string>& m_vec ; // 文件路径
		std::string m_title ; //"标  题"
		MYHASHMAP::hash_map<int, std::pair<int, int> > m_offset ;//网页偏移
};
void show(std::vector<std::string>::value_type& val)
{
	std::cout << val << std::endl ;
}
int main(int argc, char* argv[])//exe  src_txt_dir  ripepage_filename  offset_file_name
{
	std::vector<std::string> str_vec ;
	DirScan mydirscan(str_vec);
	// 遍历 src_txt_dir 下 文件 并把文件路径保存在str_vec中
	mydirscan(argv[1]);
	std::cout << str_vec.size() << std::endl ;
	std::string title ;
	std::getline(std::cin,title) ;//"标  题"
	std::cout << title << std::endl ;
	FileProcess myfileprocess(str_vec, title);
	myfileprocess(argv[2], argv[3]);
	std::cout << "Over" << std::endl ;
	return 0 ;
}


