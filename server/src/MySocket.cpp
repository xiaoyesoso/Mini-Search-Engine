#include "MySocket.h"

void MySocket::setnonblocking(int sock)
{
	int opts; 
	opts = fcntl(sock, F_GETFL); 
	if (opts < 0) 
	{ 
		perror("fcntl(sock,GETFL)"); 
		exit(1); 
	} 
	opts = opts | O_NONBLOCK; 
	if (fcntl(sock, F_SETFL, opts) < 0) 
	{ 
		perror("fcntl(sock,SETFL,opts)"); 
		exit(1); 
	} 
}


MySocket::MySocket()
{
}


void MySocket::init()
{

	//生成用于处理accept的epoll专用的文件描述符 
	PthPool -> epfd = epoll_create(256); 
	listenfd = socket(AF_INET, SOCK_STREAM, 0); 
	//把 socket设置为非阻塞方式 
	setnonblocking(listenfd); 
	//设置与要处理的事件相关的文件描述符 
	PthPool ->ev.data.fd = listenfd; 
	//设置要处理的事件类型 
	PthPool ->ev.events = EPOLLIN | EPOLLET; 
	//注册 epoll事件 
	epoll_ctl(PthPool ->epfd, EPOLL_CTL_ADD, listenfd, &(PthPool ->ev)); //要加（）

	//TCP
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET; 
	char *local_addr = MyIP; 
	inet_aton(local_addr, &(serveraddr.sin_addr));//htons(SERV_PORT); 
	serveraddr.sin_port = htons(MyPort); 
	bind(listenfd, (sockaddr *) &serveraddr, sizeof(serveraddr));
	
	std::cout << "bind" << std::endl;
	
	listen(listenfd, MyListenQ); 
}



void MySocket::RunSocket()
{
	int i ;
	for (;;) 
	{   
		//等待 epoll事件的发生 
		nfds = epoll_wait( PthPool ->epfd,  PthPool ->events, PthPool->EpollQ, 500); 
		//处理所发生的所有事件 
		//std::cout << "nfds = " << nfds << std::endl;
		for (i = 0; i < nfds; ++i) 
		{ 
			
			if ( PthPool ->events[i].data.fd == listenfd) 
			{ 
				connfd = accept(listenfd, (sockaddr *) &clientaddr, &clilen); 
				if (connfd < 0) 
				{ 
					perror("connfd<0"); 
					exit(1); 
				} 
				else std::cout<<"listened accepted"<<std::endl;
				setnonblocking(connfd); 
				char *str = inet_ntoa(clientaddr.sin_addr); 
				std::cout << "connec_ from >>" << str << std::endl; 
				//设置用于读操作的文件描述符 
				PthPool ->ev.data.fd = connfd; 
				//设置用于注测的读操作事件 
				PthPool ->ev.events = EPOLLIN | EPOLLET; 
				//注册ev 
				epoll_ctl( PthPool ->epfd, EPOLL_CTL_ADD, connfd, & PthPool ->ev); 
			} 
		
			else if ( PthPool ->events[i].events & EPOLLIN) 
			{ 
				printf("reading!\n"); 
				if ((sockfd =  PthPool ->events[i].data.fd) < 0) continue; 
				Task* new_task = (Task*)malloc(sizeof(Task)); 
				new_task->fd = sockfd; 
				new_task->next = NULL; 

				//添加新的读任务 

				
				

				if ( PthPool ->readhead == NULL) 
				{ 
					PthPool ->readhead = new_task; 
					PthPool ->readtail = new_task;
					//唤醒所有等待cond1 条件的线程 
					PthPool ->my_Con.Broadscast(); 
				} 
				else 
				{ 
					std::cout << "Mutex Lock" <<std::endl;
					PthPool ->my_Lock.Lock(); 
					PthPool ->readtail->next = new_task; 
					PthPool ->readtail = new_task;
					//唤醒所有等待cond1 条件的线程 
					PthPool ->my_Con.Broadscast(); 
					PthPool ->my_Lock.Unlock();
					std::cout << "Mutex UnLock" <<std::endl;
				} 
				
				
				
			} 

			else if ( PthPool ->events[i].events & EPOLLOUT) 
			{ 
				rdata = (User_Data *) PthPool ->events[i].data.ptr; 
				sockfd = rdata->get_fd(); 
				std::string line =  rdata->get_result();
				write(sockfd, line.c_str() , line.length() + 1 ); 

				std::cout << "sockfd " << sockfd << "data.fd" 
					<< PthPool ->events[i].data.fd << std::endl;

				delete rdata; 
				//设置用于读操作的文件描述符 
				PthPool ->ev.data.fd = sockfd; 
				//设置用于注测的读操作事件 
				PthPool ->ev.events = EPOLLIN | EPOLLET; 
				//修改sockfd上要处理的事件为 EPOLIN 
				epoll_ctl( PthPool ->epfd, EPOLL_CTL_MOD, sockfd, & PthPool ->ev);
				std::cout <<" Over" <<std::endl;
			} 

		} 
	} 
}


