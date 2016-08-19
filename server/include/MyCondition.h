
#if !defined(_MYCONDITION_H)
#define _MYCONDITION_H

#include "MyLock.h"

class MyCondition {
public:
	MyCondition(pthread_cond_t cond, pthread_mutex_t& mutex);
	void Wait();
	void Broadscast();
private:
	pthread_mutex_t m_mutex;
	pthread_cond_t m_cond;
};

#endif  //_MYCONDITION_H
