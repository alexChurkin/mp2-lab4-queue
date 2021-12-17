#pragma once
#include "Randomex.h"
#include "DataStructs.h"
#include "..\mp2-lab4-queue\TQueue.h"
#include "DataGridViewExtensions.h"
#include <vector>
#include <algorithm>

ref class ProcFarm
{
private:
	Randomex^ randomex;

	int n;
	float newTaskP = 0.15;
	int minProcForTask = 1, maxProcForTask = 16;
	int minCC = 1, maxCC = 4;

	//���������� � �� ����������
	Processor* processors;
	Stat* stat;

	//��������� �������� id
	int lastTaskId = 0;

	//������� ����� (TODO �������� �� ������� ��� �������)
	TQueue<Task>* qTasks = new TQueue<Task>(100000);
	std::vector<int>* activeTasksIds = new std::vector<int>(n);

public:
	ProcFarm(int _n)
	{
		randomex = gcnew Randomex();
		n = _n;
		processors = new Processor[n];
		stat = new Stat();
	}

	~ProcFarm()
	{
		delete[] processors;
		delete stat;

		delete qTasks;
		delete activeTasksIds;
	}

	int GetCurrentLoad()
	{
		return stat->currentLoad;
	}

	int GetActiveTasksCount()
	{
		return stat->tasksActive;
	}

	int GetQueueTasksCount()
	{
		return qTasks->GetCount();
	}

	int GetTotalCCs()
	{
		return stat->totalCCs;
	}

	int GetTotalWaitingCCs()
	{
		return stat->totalWaitingCCs;
	}

	int GetReceivedTasksCount()
	{
		return stat->tasksReceived;
	}

	int GetCompletedTasksCount()
	{
		return stat->tasksCompleted;
	}

	int GetAverageLoad()
	{
		return stat->averageLoad;
	}

	void SetNewTaskP(float _newTaskP)
	{
		newTaskP = _newTaskP;
	}

	void SetProcForTaskRange(int _minProcForTask, int _maxProcForTask)
	{
		minProcForTask = _minProcForTask;
		maxProcForTask = _maxProcForTask;
	}

	void SetCCRange(int _minCC, int _maxCC)
	{
		minCC = _minCC, maxCC = _maxCC;
	}


	void Tick(cli::array<System::Windows::Forms::Button^>^ procViews,
		System::Windows::Forms::DataGridView^ dgLogs,
		System::Windows::Forms::DataGridView^ dgActive,
		System::Windows::Forms::DataGridView^ dgQueue)
	{
		//���������� ��������� ������� ������� �� �����������, �������� �����������
		stat->currentLoad = 0;



		for (int i = 0; i < n; i++)
		{
			//��� ������ �����������
			if (processors[i].isWaiting)
			{
				processors[i].ccWaiting++;
				stat->totalWaitingCCs++;
			}
			//��� ����������

			
			else
			{
				stat->currentLoad++;
				stat->totalCCs++;

				processors[i].ccCount++;
				processors[i].ccOfTaskCount--;
				/*
				auto it = std::find(
					activeTasksIds->begin(),
					activeTasksIds->end(),
					processors[i].taskId);

				//�� ��� ��������� ������
				if (processors[i].ccOfTaskCount > 0)
				{
					//TODO Update remained ccs (UI)
					int index = std::distance(activeTasksIds->begin(), it);
					dgActive->Rows[i]->Cells[0]->Value =
						processors[i].taskId + " : " + processors[i].ccOfTaskCount;
				}
				//�������� ������
				else
				{
					stat->tasksCompleted++;

					if (it != activeTasksIds->end())
					{
						activeTasksIds->erase(it);

						int index = std::distance(activeTasksIds->begin(), it);
						dgActive->Rows->RemoveAt(index);

						AddRowWithScroll(dgLogs, System::Drawing::Color::Aquamarine,
							"������ " + std::to_string(processors[i].taskId) + " ������� ���������.");
					}

					processors[i].taskId = -1;
					processors[i].isWaiting = true;
					procViews[i]->BackColor = System::Drawing::Color::Teal;
					dgLogs->Rows[i]->DefaultCellStyle->ForeColor = Randomex::RandColor();
				}*/
			}
			
		}



		//��������� ������ �� ������� � � ���������
		if (qTasks->IsNotEmpty())
		{
			Task t = qTasks->Pop();

			auto tColor = dgQueue->Rows[0]->DefaultCellStyle->ForeColor;
			dgQueue->Rows->RemoveAt(0);

			//����� ��������� ����������
			if ((n - stat->currentLoad) >= t.procCount)
			{
				AddRowWithScroll(dgActive, tColor, t.id + " : " + t.procCount);

				AddRowWithScroll(dgLogs, System::Drawing::Color::DeepSkyBlue,
					"������ " + t.id + ", ��������� " + t.procCount
						+ " �����������, ���������� �� ����������.");

				for (int i = 0, selCount = 0; selCount < t.procCount; i++)
				{
					if (processors[i].isWaiting)
					{
						selCount++;

						processors[i].isWaiting = false;
						processors[i].taskId = t.id;
						processors[i].ccOfTaskCount = t.ccTotal;

						procViews[i]->BackColor = tColor;
					}
				}
			}
			//������ ��������� ����������
			else
			{
				qTasks->Push(t);

				AddRowWithScroll(dgQueue, tColor, t.id + " : " + t.procCount);
			}
		}

		
		//����������� ������ � ������������ P
		if (randomex->RandBool(newTaskP))
		{
			Task newTask{};
			newTask.id = ++lastTaskId;
			newTask.procCount = randomex->RandInt(minProcForTask, maxProcForTask);
			newTask.ccTotal = randomex->RandInt(minCC, maxCC);
			//newTask.ccCompleted = 0;
			qTasks->Push(newTask);
			

			AddRowWithScroll(dgQueue, Randomex::RandColor(),
				newTask.id + " : " + newTask.procCount);
			//TODO Insert to queue and to ui
		}
	}
};