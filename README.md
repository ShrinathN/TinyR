# TinyR
<h1>Description</h1>
A very minimal task based watchdog implemented for AVR uCs.<br>
Make sure your tasks take less than the overflow time of the TIMER0, with the appropriate prescaler.<br>
Timer0 overflow time is the maximum time a task can take, if it itends to exceeds that, the next task will be given chance for execution instead.<br>
Does not continue unfinished tasks, restarts them instead.<br>
<h2>How to use?</h2>
This is pretty easy to use, first disable interrupts using `cli()`<br>
Then initialize the scheduler using `TinyR_SimpleInit({PRESCALER})`. You can use one of these as
a prescaler
<ul>
<li>SCHEDULER_OFF</li>
<li>PRESCALER_0</li>
<li>PRESCALER_8</li>
<li>PRESCALER_64</li>
<li>PRESCALER_256</li>
<li>PRESCALER_1024</li>
</ul><br>
Then add a task using `TinyR_AddTask({task})` where task should be the address to your function. Your function should include `TinyR_TaskComplete()` at the end of its processing.<br>
Finally you can use `sei()` to start interrupts and finally start the scheduler<br>
You can use `TinyR_RemoveTask({ID})` where ID is the ID of the task<br>
