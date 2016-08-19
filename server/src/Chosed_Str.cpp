#include "Chosed_Str.h"

Chosed_Str::Chosed_Str(std::string str,int pre,int dis)
{
	My_str = str ;
	My_pre = pre ;
	My_dis = dis ;
}


std::string& Chosed_Str::get_str() 
{
	return My_str ;
}


int& Chosed_Str::get_pre() 
{
	return My_pre ;
}


int& Chosed_Str::get_dis() 
{
	return My_dis ;
}