#include "User_Data.h"

int User_Data::get_fd()
{
	return My_fd ;
}

char* User_Data::get_line()
{
	return My_line ;
}
	
std::string  User_Data::get_result()
{
	return My_result;
}

void User_Data::set_fd(int fd)
{
	My_fd = fd;
}

void User_Data::set_result( std::string result)
{
	My_result = result ;
}


void User_Data::Line_Copy(char * str)
{
	strcpy(My_line , str );
}
