#ifndef __MYCONF_H__
#define __MYCONF_H__
#define MYHASHMAP __gnu_cxx
#include <ext/hash_map>
#include <set>
#include <utility>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
class MyHashFn
{
	public:
		std::size_t operator()(const std::string& str) const
		{
			return MYHASHMAP::__stl_hash_string(str.c_str()) ;
		}
};
class MyConf
{
	public:
		MyConf(const std::string &conf_name)
		{
			do_some(conf_name);
		}
		void show()
		{
			std::cout <<"mystoplist: " << m_conf["mystoplist"] << std::endl ;
			std::cout <<"myictclas: " << m_conf["myictclas"] << std::endl ;
			std::cout <<"myoffset: " << m_conf["myoffset"] << std::endl ;
		}
		MYHASHMAP::hash_map<std::string, std::string, MyHashFn> m_conf ;
		MYHASHMAP::hash_map<int, std::pair<int, int> > m_offset ;
		MYHASHMAP::hash_map<std::string, std::string, MyHashFn> m_stop ;
		MYHASHMAP::hash_map<std::string, std::set<std::pair<int, double> >, MyHashFn> m_invert ; 
	private:
		void do_some(const std::string & conf_name) ;
};
#endif
