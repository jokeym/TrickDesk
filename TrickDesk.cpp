#include <windows.h>

HWND hwnd; //用来保存一个我们查找的窗口句柄
HWND hwdesk; //用来保存桌面的窗口句柄

MSG uMsg;  // 窗口消息
DWORD HotkeyID;
DWORD TimerID;

typedef struct _item{
	HWND hWnd;
	struct _item * next;
}hideHwnd;

hideHwnd * hideListHead; // 链表头指针
hideHwnd * hideListEnd; // 链表结束指针

HANDLE processHeap; // 保存程序进程的默认堆

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	hwdesk = FindWindowA("Progman", "Program Manager");//获取桌面的窗口句柄

	//获取一个系统全局的唯一ID
	HotkeyID = GlobalAddAtomA("Jokeym 1033800341");
	RegisterHotKey(0, HotkeyID, 0, VK_F8);
	//设置一个定时器，让程序每隔一段时间来检测一下当前桌面上的窗口
	TimerID = GlobalAddAtomA("无名 1033800341");

	SetTimer(0, TimerID, 10, 0);// 每10ms调用一次

	processHeap = GetProcessHeap(); //获取进程的默认堆
	// 初始化自已实现的链表
	hideListHead = 0;
	hideListEnd = 0;
	while (TRUE)
	{
		GetMessage(&uMsg, 0, 0, 0);
		if (uMsg.message == WM_HOTKEY)
		{
			MessageBoxA(0, "Jokeym ", "1033800341", MB_OK);
			break;
		}
		else if (uMsg.message == WM_TIMER)
		{
			hwnd = GetForegroundWindow();// 已经激活的窗口的句柄
			if (hwnd != hwdesk) // 比较是否是桌面，如果是桌面，跳过
			{
				if (IsWindowVisible(hwnd)) //检查窗口是否可见
				{
					hideHwnd * addr = (hideHwnd *)HeapAlloc(processHeap, HEAP_ZERO_MEMORY, sizeof(hideHwnd));
					if (addr) // 检查是否成功的申请到了内存
					{
						if (hideListEnd == 0) // 检查链表是否为空
						{
							hideListHead = addr; //链表头指向申请的内存
							hideListEnd = addr; //链表结束指针指向申请的内存
							addr->hWnd = hwnd;
							addr->next = 0;
						}
						else
						{
							hideListEnd->next = addr;
							hideListEnd = addr;
							hideListEnd->hWnd = hwnd;
							hideListEnd->next = 0;
						}
						ShowWindow(hwnd, SW_HIDE);//隐藏起来
					}
				}
			}
		}
	}
	// 把隐藏的东西还原回来
	while (hideListHead)
	{
		//窗口还原
		ShowWindow(hideListHead->hWnd, SW_SHOW);
		// 内存释放
		LPVOID addrTmp = (LPVOID)hideListHead;
		hideListHead = hideListHead->next;
		HeapFree(processHeap, HEAP_NO_SERIALIZE, addrTmp);
	}
	// 取消定时器注册
	KillTimer(0, TimerID);
	ExitProcess(0);
}