#pragma once
struct Processor
{
	int taskId;			  //id ����������� ������
	int ccCount = 0;	  //����� ������ ������
	int ccWaiting = 0;	  //����� ������ �������
	int ccOfTaskCount = 0;//������ �������� �� ������ ������
	bool isWaiting;		  //���� - ����� ��� ��������
};

struct Task
{
	int id;			   //id
	int procCount;	   //����� �����������
	int ccTotal;	   //���������� ������ ��� ����������
};

struct Stat {
	//������� ��������
	int currentLoad = 0;
	//����������� �����
	int tasksActive = 0;
	//����� � �������
	int tasksQueue = 0;

	//��������� ����� ������ ������ (���� �����������)
	int totalCCs = 0;
	//��������� ����� ������ ������� (���� �����������)
	int totalWaitingCCs = 0;
	//�������� ����� ��������
	int tasksReceived = 0;
	//�������� ����� ���������
	int tasksCompleted = 0;
	//� ������� ��������� �����������
	double averageLoad = 0;
};