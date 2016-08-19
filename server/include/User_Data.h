
#if !defined(_USER_DATA_H)
#define _USER_DATA_H

#include "MyConf.h"

class User_Data
{

public:
	int get_fd();
	char* get_line();
	std::string  get_result();
	void set_fd(int fd);
	void set_result(std::string  result);
	void Line_Copy(char * );
private:
	int My_fd; 
	char My_line[MAXLINE]; 
	std::string  My_result ; 
};


#endif  //_MYLOCK_H

