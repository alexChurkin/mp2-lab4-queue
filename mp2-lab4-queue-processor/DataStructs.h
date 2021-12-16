#pragma once
struct Processor
{
	int taskId;		   //id ����������� ������
	int ccCount;	   //����� ������ ������
	int waitingTime;   //����� ������ �������

	int ccOfTaskCount; //����� ������ - ������ ������

	bool isWaiting;	   //���� - ����� ��� ��������
};

struct Task
{
	int id;			   //id
	int procCount;	   //����� �����������
	int ccTotal;	   //����� ����� ������
	int ccCompleted;   //����� ����������� ������
};

struct Stat {
	//������� ��������
	int currentLoad = 0;
	//����������� �����
	int tasksActive = 0;

	//��������� ����� ������ ������ (���� �����������)
	int totalCCs = 0;
	//��������� ����� ������ ������� (���� �����������)
	int totalWaitingCCs = 0;
	//�������� ����� ��������
	int tasksReceived = 0;
	//�������� ����� ���������
	int tasksCompleted = 0;
	//� ������� ��������� �����������
	int averageLoad = 0;

	void Clear()
	{
		totalCCs = totalWaitingCCs = 0;
		tasksReceived = tasksCompleted = 0;
		averageLoad = 0;
	}
};