//===========CONFIG===========
#define MAX_TASKS 10
//============================

//macros to drive the timer
#define SCHEDULER_OFF 0
#define PRESCALER_0 1
#define PRESCALER_8 2
#define PRESCALER_64 3
#define PRESCALER_256 4
#define PRESCALER_1024 5

typedef unsigned char BYTE;
typedef void (*TinyTask)(void); //typedef function

// this struct will store all the info about the tasks
struct _TaskList_
{
    TinyTask * list[MAX_TASKS]; //list of function pointers to the tasks
    BYTE totalTasks; //total number of tasks
    BYTE currentTask; //current task being executed
}TaskList; //struct

/* This function will execute the next task in case the current one
completes before expected time*/
void TinyR_TaskComplete()
{
    TCNT0 = 0xff; //sets the TCNT0 counter right on the edge
}

/* This function initializes the RoundRobin scheduler*/
void TinyR_SimpleInit(BYTE timerPrescaler)
{
    TaskList.currentTask = 0; //setting current tasks as 0
    TaskList.totalTasks = 0; //setting total tasks as 0
    TIMSK |= (1 << TOIE0); //enabling overflow vector
    TCCR0 |= (timerPrescaler & 0b111); //setting the prescaler
}

/* This function adds a task to the list*/
void TinyR_AddTask(TinyTask task)
{
    if(TaskList.totalTasks < MAX_TASKS) //if the total tasks < MAX_TASKS, ie the task list is not full
    {
        TaskList.list[TaskList.totalTasks] = (TinyTask*)task; //add a new task
        TaskList.totalTasks++; //increment totalTasks counter
    }
}

void TinyR_RemoveTask(BYTE taskID)
{
    if((TaskList.totalTasks != 0) && (taskID <= TaskList.totalTasks)) //if the totalTasks != 0, ie the list is not empty AND taskID < total tasks ie, its a valid ID
    {
        TaskList.list[taskID] = TaskList.list[TaskList.totalTasks-1]; //set the task with taskID as the last task
        TaskList.totalTasks--; //decrement totalTasks counter
    }
}


void (*functionToRun)(); //this is the function pointer used to execute the next task

/* This interrupt routine will be executed on the overflow of TIMER0
This will start executing the next task*/
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
        functionToRun(); //running task
    }
}
