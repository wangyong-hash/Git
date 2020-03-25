
#pragma once 
#include <string>

using namespace std;

// 定时器通道类。
class Timer : public Ichannel {
public : 
	Timer();
	virtual ~Timer();

	// 通过Ichannel 继承
	virtual bool Init() override;
	virtual bool ReadFd(string & _inPut) override;
	virtual bool WriteFd(string &_outPut) override;
	virtual void Fini() override;
	virtual int GetFd() override;
	virtual string GetChannelInfo() override;
	virtual  AZinxHandler * GetPutNextStage(BytesMsg &_oinput) override;

private: 
	int mFd = -1;
};