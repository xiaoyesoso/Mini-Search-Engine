
#include "MyPage.h"
#include "MySplit.h"
#include <map>
#include <queue>
#include <algorithm>
bool operator ==(const MyPage& left, const MyPage& right) 
{
	std::vector<std::string >::const_iterator iter_left = left.m_top.begin() ;
	int num_common  = 0 ;
	for(; iter_left != left.m_top.end() ; iter_left ++)
	{
		num_common += count(right.m_top.begin(), right.m_top.end(), *iter_left) ;
	}
	int min_size = left.m_top.size() < right.m_top.size() ? left.m_top.size() : right.m_top.size() ;
	if( (double)num_common / min_size > 0.6 )
	{
		return true ;
	}else 
	{
		return false ;
	}
}
void MyPage::do_some(const std::string& doc,MyConf& conf, MySplit& asplit)
{
	std::string head_docid = "<docid>" ;
	std::string tail_docid= "</docid>" ;
	std::string head_docurl = "<docurl>" ;
	std::string tail_docurl = "</docurl>" ;
	std::string head_doctitle = "<doctitle>" ;
	std::string tail_doctitle = "</doctitle>";
	std::string head_doccontent = "<doccontent>" ;
	std::string tail_doccontent = "</doccontent>" ;
	
	
//	截取ID
	int bpos = doc.find(head_docid );
	int epos = doc.find(tail_docid, bpos ) ;
	m_docid = doc.substr(bpos + head_docid.size() , epos - bpos - head_docid.size() );

//	截取URL
    bpos = doc.find(head_docurl );
    epos = doc.find(tail_docurl, bpos ) ;
	m_docurl = doc.substr(bpos + head_docurl.size() , epos - bpos - head_docurl.size() );

//	截取title
	bpos = doc.find(head_doctitle );
	epos = doc.find(tail_doctitle, bpos ) ;
	m_doctitle = doc.substr(bpos + head_doctitle.size() , epos - bpos - head_doctitle.size() );

//	截取content
	bpos = doc.find(head_doccontent );
	epos = doc.find(tail_doccontent, bpos ) ;
	m_doccontent = doc.substr(bpos + head_doccontent.size() , epos - bpos - head_doccontent.size() );

	std::vector<std::string> word_vec ;//保存网页分词结果
	word_vec = asplit(m_doccontent.c_str()) ;
	topK(word_vec);
}


void MyPage::topK(std::vector<std::string>& word_vec)
{
	std::vector<std::string>::iterator iter  ;
	//建立  wordmap
	for(iter = word_vec.begin(); iter != word_vec.end(); iter ++)
	{
		m_wordmap[*iter] ++ ;
	}
	////建立 topK
	std::priority_queue<std::pair< std::string, int>, std::vector<std::pair< std::string, int> >, MyCompare  > word_queue(m_wordmap.begin(), m_wordmap.end());
	int num = 0 ;
	while(!word_queue.empty())
	{
		m_top.push_back(word_queue.top().first) ;
		word_queue.pop();
		if(m_top.size() == 10)
		{
			break ;
		}
	}

}

