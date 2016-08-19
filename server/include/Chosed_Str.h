#if !defined(_CHOSED_STR_H)
#define _CHOSED_STR_H


#include <string>
class Chosed_Str
{
public:
	Chosed_Str();
	Chosed_Str(std::string ,int ,int );
	friend bool operator< (Chosed_Str n1, Chosed_Str n2)
	{
		if(n1.get_dis() != n2.get_dis())
			return n1.get_dis() > n2.get_dis();
		return n1.get_pre() < n2.get_pre();
	}

	std::string& get_str()  ;
	int& get_pre() ;
	int& get_dis()  ;
private:
	std::string My_str;
	int My_pre;
	int My_dis;

};



#endif