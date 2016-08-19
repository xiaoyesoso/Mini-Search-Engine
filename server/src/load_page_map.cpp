
#include "test.h"
void load_page_map(MyConf& conf, MySplit& txtsplit, MYHASHMAP::hash_map<int, MyPage>& page_map)
{	
	FILE* fp_ripepage = fopen(conf.m_conf["myripepage"].c_str(), "r");
	if(fp_ripepage == NULL)
	{
		std::cout << "open ripepage faile " << std::endl ;
		exit(0);
	}
	int page_num = 1 ;

	while(page_num <= 10475)
	{
		if(conf.m_offset.count(page_num) )
		{
			char* txt = new char[conf.m_offset[page_num ].second + 1]();
			fseek(fp_ripepage, conf.m_offset[page_num].first, SEEK_SET);
			fread(txt, 1, conf.m_offset[page_num].second, fp_ripepage);
			//std::cout << "MyPage" << page_num << std::endl;
			//printf("%s\n",txt);
			MyPage apage(txt, conf, txtsplit);
			page_map[ atoi( apage.m_docid.c_str() )] = apage ;
			delete [] txt ;
		}
		page_num ++ ;
	}
	fclose(fp_ripepage);
	
}
