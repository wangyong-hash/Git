#include "TimerDeliver.h"
#include <iostream>

using namespace std;

#define TIMER_WHEEL_SIZE 8

TimerDeliver TimerDeliver::m_single;

TimerDeliver::TimerDeliver()
{
	// 改变容器中可存储的元素个数
	m_TimerWheel.resize(TIMER_WHEEL_SIZE);
}


TimerDeliver::~TimerDeliver()
{
	
}

// 注册任务
bool TimerDeliver::RegisterProcObject(TimerOutProc & _proc)
{
	int tmo = 0, index = 0, cout = 0;
	WheelNode node;

	// 获取超时时间
	tmo = _proc.GetTimerSec();
	
	if (tmo <= 0) {
		return false;
	}

	// 计算时间轮子格子
	// (当前刻度 + 超时时间) % 时间轮子格子数
	index = (m_cur_index + tmo) % m_TimerWheel.size();

	// 计算圈数
	// 圈数 = 超时时间 / 时间路子格子数。
	cout = tmo / m_TimerWheel.size();
	// 圈数
	node.LastCount = cout;
	// 超时任务指针
	node.pProc = &_proc;

	
	// 添加到 vector 容器中
	m_TimerWheel[index].push_back(node);
	return false;
}

// 注销任务
void TimerDeliver::UnRegisterProcObject(TimerOutProc & _proc)
{
	// 遍历 vector
	for (auto& I : m_TimerWheel) {
		for (auto it = I.begin(); it != I.end(); ) {
			if (it->pProc == &_proc) {
				it = I.erase(it);
			}
			else {
				it++;
			}
		}
	}
}

// 调度实现
IZinxMsg * TimerDeliver::InternalHandle(IZinxMsg * _msg)
{
	uint64_t tmo = 0;
	list<WheelNode> registerList;

	// 完成超时间确定
	BytesMsg *pMsg = dynamic_cast<BytesMsg*>(&_msg);
	pMsg->szData.copy((char *)&tmo, sizeof(tmo));

	cout << "tmo: " << endl;
	for (uint64_t i = 0; i < tmo; i++) {
		// 当前刻度自增
		m_cur_index = (m_cur_index + 1) % m_TimerWheel.size();

		cout << "当前刻度： " << m_cur_index << endl;

		// 遍历当前格子中所有任务， 判断是否超时
		for (auto it = m_TimerWheel[m_cur_index].begin(); it != m_TimerWheel[m_cur_index].end(); ) {
			// 圈数减 1		(根据定时时间的长短，节点中记录的有，时间处于节点的圈数，
								// 每次调出不同刻度上的链表的不同事件的圈数进行减一，
							    // 在定时器圈数和轮盘刻度的计算公式，可以在定时满之后进行事件的调度。)
			it->LastCount--;
			if (it->LastCount < 0) {
				// 调用超时任务处理
				it->pProc->Proc();

				// 保存任务
				registerList.push_back(*it);
				it = m_TimerWheel[m_cur_index].erase(it);
			}
			else {
				it++;
			}
		}

		// 重新注册任务
		for (auto it : registerList) {
			RegisterProcObject(*(it.pProc));
		}
	}
	return nullptr;
}

AZinxHandler * TimerDeliver::GetNextHandler(IZinxMSg & _oNextHandler)
{
	return nullptr;
}
