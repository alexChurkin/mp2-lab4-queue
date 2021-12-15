#pragma once
struct Processor
{
	int taskId;		   //id ����������� ������
	int ccCount;	   //����� ������ - �����
	int ccForTaskCount;//����� ������ - ������ ������
	int waitingTime;   //����� ������ �������

	bool isWaiting;	   //���� - ����� ��� ��������
};

struct Task
{
	int id;			//id
	int stepCount;	//����� ��������� ������
	int allSteps;	//����� ����� ������
	int procCount;	//����� �����������
};