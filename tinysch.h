//===========CONFIG===========
#define MAX_TASKS 10
//============================
#define SCHEDULER_OFF 0
#define PRESCALER_0 1
#define PRESCALER_8 2
#define PRESCALER_64 3
#define PRESCALER_256 4
#define PRESCALER_1024 5

typedef unsigned char BYTE;
typedef void (*TinyTask)(void);

struct _TaskList_
{
    TinyTask * list[MAX_TASKS];
    BYTE totalTasks;
    BYTE currentTask;
}TaskList;


/* This function initializes the RoundRobin scheduler*/
void TinyR_SimpleInit(BYTE timerPrescaler)
{
    TaskList.currentTask = 0;
    TaskList.totalTasks = 0;
    TIMSK |= (1 << TOIE0);
    TCCR0 |= (timerPrescaler & 0b111);
}

void TinyR_AddTask(TinyTask task)
{
    if(TaskList.totalTasks < MAX_TASKS)
    {
        TaskList.list[TaskList.totalTasks] = (TinyTask*)task;
        TaskList.totalTasks++;
    }
}

void TinyR_RemoveTask(BYTE taskID)
{
    if((TaskList.totalTasks != 0) && (taskID <= TaskList.totalTasks))
    {
        TaskList.list[taskID] = TaskList.list[TaskList.totalTasks-1];
        TaskList.totalTasks--;
    }
}


void (*functionToRun)();
ISR(TIMER0_OVF_vect)
{
    if(TaskList.totalTasks == 0) //meaning if there are no tasks, stop scheduler
        TinyR_SimpleInit(SCHEDULER_OFF); //stop scheduler
    else //else if there are non 0 tasks
    {
        TaskList.currentTask++; //increment current task
        if(TaskList.currentTask >= TaskList.totalTasks)
            TaskList.currentTask = 0;
        functionToRun = (void *)TaskList.list[TaskList.currentTask];
        functionToRun();
    }
}
