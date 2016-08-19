#include "MyConf.h"
#include <stdlib.h>
#include <stdio.h>
void MyConf::do_some(const std::string& conf_name)
{
	//读取配置文件
	std::ifstream fin(conf_name.c_str()) ;
	if(!fin)
	{
		std::cout << "open conf file faile !" << std::endl ;
		exit(0);
	}
	std::string line , key, value;
	while( getline(fin, line) )
	{
		std::cout << line << std::endl ;
		std::istringstream sin(line);
		sin >> key >> value ;
		m_conf[key] = value ;
	}
	fin.close();
	fin.clear();
	
	
	//打开偏移文件
	fin.open(m_conf["myoffset"].c_str());
	if(!fin)
	{
		std::cout << "open offset fail " << std::endl ;
		exit(0);
	}
	int idocid, ioffset ,isize ;
	line.clear();
	while(getline(fin, line) )
	{
		std::istringstream sin(line);
		sin >> idocid >> ioffset >> isize ;
		m_offset[idocid].first = ioffset ;
		m_offset[idocid].second = isize ;
		std::cout << idocid << "  " << ioffset << "   " << isize<< "   "<< std::endl;
	}
	fin.close();
	fin.clear();
	
	//打开停用词库
	fin.open(m_conf["mystoplist"].c_str());
	if(!fin)
	{
		std::cout << "open mystoplist fail " << std::endl ;
		exit(0);
	}
	line.clear();
	while(fin >> line)
	{
		m_stop[line] = line ;
	}
	fin.close();
	
	
}

