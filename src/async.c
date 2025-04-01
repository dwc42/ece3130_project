#include "date.h"
#include "float.h"
#include "stdlib.h"
#include "events.h"
#include "async.h"

struct AsyncTask nullptrTask = {NULL, 0, -1, 0};
int lengthTasks(struct AsyncTask *array)
{
	int i = 0;
	while ((array[i]).id > -1)
	{
		i++;
	}
	return i;
}
int indexOfTask(struct AsyncTask *array, int id)
{
	for (int i = 0; array[i].id > -1; i++)
	{
		if (array[i].id == id)
			return i;
	}
	return -1;
}
int removeTask(struct AsyncTask **array, int id)
{

	int foundIndex = indexOfTask(*array, id);
	if (foundIndex == -1)
		return -1;
	struct AsyncTask **newArray = (struct AsyncTask **)malloc(sizeof(struct AsyncTask *) * (lengthTasks(*array) - 1));
	int j = 0;
	for (int i = 0; (*array)[i].id > -1; i++)
	{
		if (i == foundIndex)
			continue;
		newArray[j++] = &(*array)[i];
	}
	newArray[j] = &nullptrTask;
	return foundIndex;
}
int clearRun(int id)
{
	return removeTask(&Async.runAfterTasks, id);
}

struct AsyncType Async;
int runAfter(void (*callback)(), double time)
{
	if (time < 0)
		return -1;
	struct AsyncTask *task = (struct AsyncTask *)malloc(sizeof(struct AsyncTask));
	task->callback = callback;
	task->time = time;
	task->id = Async.currentId++;
	task->runDate = date() + time;
	Async.runAfterTasks = (struct AsyncTask *)realloc(Async.runAfterTasks, sizeof(struct AsyncTask) * (Async.currentId + 1));
	int len = lengthTasks(Async.runAfterTasks);
	Async.runAfterTasks[len] = *task;
	return task->id;
}
int runInterval(void (*callback)(), double interval)
{
	if (interval < 0)
		return -1;
	struct AsyncTask *task = (struct AsyncTask *)malloc(sizeof(struct AsyncTask));
	task->callback = callback;
	task->time = interval;
	task->id = Async.currentId++;
	task->runDate = date() + interval;
	int len = lengthTasks(Async.runIntervalTasks);
	Async.runIntervalTasks = (struct AsyncTask *)realloc(Async.runIntervalTasks, sizeof(struct AsyncTask) * (len + 1));
	Async.runIntervalTasks[len] = *task;
	Async.runIntervalTasks[len + 1] = nullptrTask;
	return task->id;
}
void checkAsync()
{
	double currentDate = date();
	for (int i = 0; Async.runIntervalTasks[i].id > -1; i++)
	{
		if (currentDate > Async.runIntervalTasks[i].runDate)
		{
			Async.runIntervalTasks[i].callback();
			Async.runIntervalTasks[i].runDate = currentDate + Async.runIntervalTasks[i].time;
		}
	}
	for (int i = 0; Async.runAfterTasks[i].id > -1; i++)
	{
		if (currentDate > Async.runAfterTasks[i].runDate)
		{
			Async.runAfterTasks[i].callback();
		}
	}
}
void InitAsync()
{
	Async.currentId = 0;
	Async.runAfterTasks = (struct AsyncTask *)malloc(sizeof(struct AsyncTask) * 2);
	Async.runIntervalTasks = (struct AsyncTask *)malloc(sizeof(struct AsyncTask) * 2);
	Async.runAfterTasks[0].id = -1;
	Async.runIntervalTasks[0].id = -1;
	Async.check = checkAsync;
	Async.runAfter = runAfter;
	Async.runInterval = runInterval;
	Async.clearRun = clearRun;
}