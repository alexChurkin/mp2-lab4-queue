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
	int ccTotal;	   //����� ����� ������
	int procCount;	   //����� �����������

	int ccCompleted;   //����� ��������� ������
};

struct Stat {

};