#ifndef ASYNC_H
#define ASYNC_H

typedef struct AsyncTask
{
	void (*callback)();
	double time;
	int id;
	double runDate;
};

struct AsyncType
{
	int currentId;
	struct AsyncTask *runAfterTasks;
	struct AsyncTask *runIntervalTasks;
	void (*check)();
	int (*runAfter)(void (*callback)(), double time);
	int (*runInterval)(void (*callback)(), double interval);
	int (*clearRun)(int id);
};

extern struct AsyncType Async;

void InitAsync();
#endif /* ASYNC_H */