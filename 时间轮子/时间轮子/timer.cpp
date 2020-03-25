#include "Timer.h"
#include "TimerDeliver.h"
#include <stdint.h>
#include <sys/timerfd.h>
#include <iostream>

using namespace std;

Timer::Timer()
{
}

Timer::~Timer()
{
}

// 
bool Timer::Init()
{
	int ret = -1;
	struct itimerspec tmo = { {1, 0}, {1, 0} };

	// 创建定时器
	mFd = timerfd_create(CLOCK_MONOTONIC, 0);
	if (-1 == mFd) {
		perror("timerFd_create");
		return false;
	}

	// 设置定时器超时时间
	ret = timerfe_settime(mFd, 0, &tmo, NULL);
	if (-1 == ret) {
		perror("timerfd_settime");
		return false;
	}

	return true;
}

// 超时次数
bool Timer::ReadFd(string & _inPut)
{
	uint64_t val;
	int ret = -1;

	// 读数据(根据超时时间可以读出次数)
	ret = read(mFd, (char*)&val, sizeof(val));
	if (ret != sizeof(val)) {
		perror("read");
		return false;
	}

	_inPut.append((char*)(&val), sizeof(val));
	
	return false;
}

bool Timer::WriteFd(string & _outPut)
{
	return false;
}

// 释放资源
void Timer::Fini()
{
	if (mFd > 0) {
		close(mFd);
	}
}

int Timer::GetFd()
{
	return mFd;
}

string Timer::GetChannelInfo()
{
	return string("timer");
}

AZinxHandler * Timer::GetPutNextStage(BytesMsg & _oinput)
{
	return &TimerDeliver::GetInstance();
}
