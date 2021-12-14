#pragma once
struct ProcInfo
{
	int taskId;     //id ������
	int stepCount;  //����� ������ - �����
	int taskCount;  //����� ������ - ������ ������
	int waitingTime;//����� ������ �������

	bool waiting;   //���� - ����� ��� ��������
};

struct TaskInfo
{
	int id;			//id
	int stepCount;	//����� ��������� ������
	int allSteps;	//����� ����� ������
	int procCount;	//����� �����������
};