#include "tcp_net_socket.h"
int tcp_connect(const char* ip, int port)   //用于客户端的连接
{
    int sfd = socket(AF_INET, SOCK_STREAM, 0);//向系统注册申请新的socket
    if(sfd == -1)
    {
        perror("socket");
        exit(-1);
    }
    struct sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(struct sockaddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(port);
    serveraddr.sin_addr.s_addr = inet_addr(ip);
if(connect(sfd, (struct sockaddr*)&serveraddr, sizeof(struct sockaddr)) == -1)
//将sfd连接至制定的服务器网络地址serveraddr
    {
                perror("connect");
                close(sfd);
                exit(-1);
    }
else printf("connect susscess\n");
    return sfd;
}

int main(int argc, char* argv[])
{
   
    int sfd = tcp_connect("127.0.0.1",5080);
    while(1)
   {
    char buf[20000] = {0};
    char str[512] = {0};
    printf("please write in:\n");
    scanf("%s",str);
    int str_len = strlen(str);
    for(int i = 0; i < str_len ; ++ i)
	if(str[i] >= 'A' && str[i] <= 'Z')
		str[i] = str[i] + 'a' - 'A' ;	
    send(sfd, str, sizeof(str), 0);     //向sfd服务端发送数据
    int rec_len =  recv(sfd, buf, sizeof(buf), 0); //从sfd服务端接收数据
	
	printf("rec_len : %d \n%s\n",rec_len , buf);
   }
    close(sfd);
    return 0;
}

