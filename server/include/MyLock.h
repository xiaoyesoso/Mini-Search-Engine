
#if !defined(_MYLOCK_H)
#define _MYLOCK_H

#include <pthread.h>

class MyLock {
public:
	MyLock(pthread_mutex_t& );
	void Lock();
	void Unlock();
private:
	pthread_mutex_t m_mutex;
};

#endif  //_MYLOCK_H
