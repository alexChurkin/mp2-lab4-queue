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
	float newTaskP = 0.15f;
	int minProcForTask = 1, maxProcForTask = 16;
	int minCC = 1, maxCC = 4;

	//���������� � �� ����������
	Processor* processors;
	Stat* stat;
	int ticksCount = 0;

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
		return (int) stat->averageLoad;
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
		ticksCount++;
		//1. ���������� ��������� ������� ����� �� �����������, �������� �����������

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
				stat->totalCCs++;

				processors[i].ccCount++;
				processors[i].ccOfTaskCount--;

				auto it = std::find(
					activeTasksIds->begin(),
					activeTasksIds->end(),
					processors[i].taskId);

				//�� ��� ��������� ������
				if (processors[i].ccOfTaskCount > 0)
				{
					_int64 index = std::distance(activeTasksIds->begin(), it);

					dgActive->Rows[index]->Cells[0]->Value =
						processors[i].taskId + " : " + processors[i].ccOfTaskCount;
				}
				//�������� ������
				else
				{
					if (it != activeTasksIds->end())
					{
						stat->tasksCompleted++;

						_int64 index = std::distance(activeTasksIds->begin(), it);
						activeTasksIds->erase(it);
						stat->tasksActive--;

						dgActive->Rows->RemoveAt((int) index);

						AddRowWithScroll(dgLogs, System::Drawing::Color::Aquamarine,
							"������ " + std::to_string(processors[i].taskId) + " ������� ���������.");
					}


					stat->currentLoad--;
					processors[i].taskId = -1;
					processors[i].isWaiting = true;
					procViews[i]->BackColor = System::Drawing::Color::FromArgb(51, 51, 51);
				}
			}

		}

		//2. ����������� ����� ������ � ������������ P
		if (randomex->RandBool(newTaskP))
		{
			stat->tasksReceived++;

			Task newTask{};
			newTask.id = ++lastTaskId;
			newTask.procCount = randomex->RandInt(minProcForTask, maxProcForTask);
			newTask.ccTotal = randomex->RandInt(minCC, maxCC);
			qTasks->Push(newTask);

			AddRow(dgQueue, Randomex::RandColor(),
				newTask.id + " : " + newTask.procCount + " : " + newTask.ccTotal);
		}

		//3. ��������� ������ �� ������� � � ���������
		if (qTasks->IsNotEmpty())
		{
			Task t = qTasks->Pop();

			auto tColor = dgQueue->Rows[0]->DefaultCellStyle->ForeColor;
			dgQueue->Rows->RemoveAt(0);

			//����� ��������� ����������
			if ((n - stat->currentLoad) >= t.procCount)
			{
				stat->currentLoad += t.procCount;
				stat->tasksActive++;

				activeTasksIds->push_back(t.id);

				AddRowWithScroll(dgActive, tColor, t.id + " : " + t.ccTotal);

				AddRowWithScroll(dgLogs, System::Drawing::Color::DeepSkyBlue,
					"������ " + t.id + " ������� ���������� �� ����������.");

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

				AddRowWithScroll(dgLogs, System::Drawing::Color::Orange,
					"������ " + t.id + " �������� (������������ ��������� �����������).");
				AddRow(dgQueue, tColor, t.id + " : " + t.procCount + " : " + t.ccTotal);
			}
		}

		stat->averageLoad =
			stat->averageLoad * (((double)ticksCount - 1) / ticksCount) + ((double) stat->currentLoad / ticksCount);
	}
};