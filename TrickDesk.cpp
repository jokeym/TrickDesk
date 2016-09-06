#include <windows.h>

HWND hwnd; //��������һ�����ǲ��ҵĴ��ھ��
HWND hwdesk; //������������Ĵ��ھ��

MSG uMsg;  // ������Ϣ
DWORD HotkeyID;
DWORD TimerID;

typedef struct _item{
	HWND hWnd;
	struct _item * next;
}hideHwnd;

hideHwnd * hideListHead; // ����ͷָ��
hideHwnd * hideListEnd; // �������ָ��

HANDLE processHeap; // ���������̵�Ĭ�϶�

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	hwdesk = FindWindowA("Progman", "Program Manager");//��ȡ����Ĵ��ھ��

	//��ȡһ��ϵͳȫ�ֵ�ΨһID
	HotkeyID = GlobalAddAtomA("Jokeym 1033800341");
	RegisterHotKey(0, HotkeyID, 0, VK_F8);
	//����һ����ʱ�����ó���ÿ��һ��ʱ�������һ�µ�ǰ�����ϵĴ���
	TimerID = GlobalAddAtomA("���� 1033800341");

	SetTimer(0, TimerID, 10, 0);// ÿ10ms����һ��

	processHeap = GetProcessHeap(); //��ȡ���̵�Ĭ�϶�
	// ��ʼ������ʵ�ֵ�����
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
			hwnd = GetForegroundWindow();// �Ѿ�����Ĵ��ڵľ��
			if (hwnd != hwdesk) // �Ƚ��Ƿ������棬��������棬����
			{
				if (IsWindowVisible(hwnd)) //��鴰���Ƿ�ɼ�
				{
					hideHwnd * addr = (hideHwnd *)HeapAlloc(processHeap, HEAP_ZERO_MEMORY, sizeof(hideHwnd));
					if (addr) // ����Ƿ�ɹ������뵽���ڴ�
					{
						if (hideListEnd == 0) // ��������Ƿ�Ϊ��
						{
							hideListHead = addr; //����ͷָ��������ڴ�
							hideListEnd = addr; //�������ָ��ָ��������ڴ�
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
						ShowWindow(hwnd, SW_HIDE);//��������
					}
				}
			}
		}
	}
	// �����صĶ�����ԭ����
	while (hideListHead)
	{
		//���ڻ�ԭ
		ShowWindow(hideListHead->hWnd, SW_SHOW);
		// �ڴ��ͷ�
		LPVOID addrTmp = (LPVOID)hideListHead;
		hideListHead = hideListHead->next;
		HeapFree(processHeap, HEAP_NO_SERIALIZE, addrTmp);
	}
	// ȡ����ʱ��ע��
	KillTimer(0, TimerID);
	ExitProcess(0);
}