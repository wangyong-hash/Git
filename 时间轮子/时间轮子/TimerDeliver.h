#pragma once
#include <list>
#include <vector>

using namespace std;

// 超时任务抽象类
class TimerOutProc {
public:
	TimerOutProc(){ }
	virtual ~TimerOutProc(){}
	virtual void Proc() = 0;					// 超时任务时区调用的接口
	virtual int GetTimerSec() = 0;		// 获取当前任务的超时间隔， 一秒触发一次就返回1；
};
// 节点类型
struct WheelNode {
	int LastCount = -1;							// 当前任务的 圈数
	TimerOutProc * pProc = NULL;			// 超时任务的指针

};


// 继承来自处理者， 作为一个超时任务调度器
class TimerDeliver : public AzinHander
{
public:
	TimerDeliver();
	~TimerDeliver();

	// 单例函数
	static TimerDeliver& GetInstance() {
		return m_single;
	}

	// 注册一个Timer 处理对象
	bool RegisterProcObject(TimerOutProc &_proc);
	// 注销
	void UnRegisterProcObject(TimerOutProc& _proc);

	// 通过 Handler 继承的虚函数
	virtual IZinxMsg * InternalHandle(IZinxMsg * _msg) override;
	virtual AZinxHandler * GetNextHandler(IZinxMSg& _oNextHandler) override;

private:
	static TimerDeliver m_single;

	// 当前轮子的刻度
	int m_cur_index = 0;

	// 时间轮向量， 每个格子中放一个list, list 元素是圈数和定时器节点
	vector<list<WheelNode>> m_TimerWheel;
};

