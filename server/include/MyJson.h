#ifndef __MYJSON_H__
#define __MYJSON_H__
#include <string>
#include <iostream>
#include <vector>
#include <utility>
#include <stdio.h>
#include <ext/hash_map>
#include <string.h>
#include "json/json.h"

#include "MyPage.h"


class MyJson
{
	public:
		std::string operator()(std::vector<std::pair<int,std::vector<double> > > &res_vec, MYHASHMAP::hash_map<int, MyPage> & page_map)
		{
			std::string res ;
			res = func(res_vec, page_map) ;
			return res ;
		}
		std::string func(std::vector<std::pair<int, std::vector<double> > > &res_vec, MYHASHMAP::hash_map<int, MyPage> & page_map)
		{
			Json::Value root ;
			Json::Value arr ;
			int index_res_vec ;
			for(index_res_vec = 0 ; index_res_vec != res_vec.size(); index_res_vec ++)
			{
				Json::Value elem ;
				elem["title"] = page_map[ res_vec[index_res_vec].first ].m_doctitle ;
				elem["summary"] = page_map[ res_vec[index_res_vec].first ].m_doccontent.size() < 50 ?  page_map[ res_vec[index_res_vec].first ].m_doccontent : page_map[ res_vec[index_res_vec].first ].m_doccontent.substr(0,50) + "..." ;
				arr.append(elem);
			}
			root["files"] = arr ;
			Json::StyledWriter stlwriter ;
			return stlwriter.write(root);
		}
};

#endif
