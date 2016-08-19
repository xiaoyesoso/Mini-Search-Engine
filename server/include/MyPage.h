#ifndef __MYPAGE_H__
#define __MYPAGE_H__

#include "MyConf.h"
#include "MySplit.h"

class MyCompare
{
	public:
		bool operator() (const std::pair< std::string, int>& left, const std::pair< std::string, int>& right )
		{
			if(left.second < right.second )
			{
				return true ;
			}else if( left.second == right.second && left.first > right.first )
			{
				return true ;
			}else 
			{
				return false ;
			}
		} 
};
class MyPage
{
	public:
		friend bool operator == (const MyPage& left, const MyPage& right) ;
		MyPage()
		{
			
		}
		MyPage(const std::string& doc, MyConf& conf, MySplit& asplit)
		{
			do_some(doc , conf , asplit);	
		}
		void show() ;
		int size()
		{
			return m_docid.size() + m_top.size() ;
		}
	std::string m_docid ;
	std::vector<std::string > m_top ;
	MYHASHMAP::hash_map<std::string, int,MyHashFn> m_wordmap ;
	std::string m_doctitle ;
	std::string m_docurl ;
	std::string m_doccontent ;
	private:
	void do_some(const std::string& doc, MyConf& conf, MySplit& asplit) ;
	void topK(std::vector<std::string>& word_vec) ;
	//std::priority_queue<std::pair<std::string, int>, std::vector<std::pair<std::string, int> >,MyCompare > m_top ;
};
#endif

