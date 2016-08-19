#ifndef __TEST_PAGE_H__
#define __TEST_PAGE_H__
#include "MyPage.h"
#include "MyConf.h"
#include "MySplit.h"
#include "MyCompareDoc.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <math.h>
#include <utility>
#include <string.h>
#include <algorithm>
#include <functional>
#include <map>
void testMyPage() ;
void load_page_map(MyConf& conf, MySplit &txtsplit ,MYHASHMAP::hash_map<int, MyPage> & page_map) ;
std::string excute_qurey(MyConf& conf, MySplit& txtsplit,MYHASHMAP::hash_map<int, MyPage>& page_map ,const std::string& str) ;
#endif
