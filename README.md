# An Upgrade to the exisiting [XV6](https://github.com/mit-pdos/xv6-public)
Original Readme can be found [here](https://github.com/vjspranav/xv6-public/blob/v1/README_original)

## Achieved through the upgrade
* Addition of Schedulers
    * FCFS Scheduler
    * PBS Scheduler (Priority Based)
    * MLFQ Scheduler (**M**ulti **L**evel **F**eedback **Q**ueue)
* Extending the process structure with
    * wait time
    * run time
    * creation time
    * end time
* Custom waitx system call mimicking wait but assigning values of runtime and wait time to argument passed.
* ps, a user function which displays details of all processes in proc table.

Let's look at all changes in detail

### waitx system call
It is exactly similar to the wait system call already used, but in addition it takes in two integer arguments ```wtime``` and ```rtime```  
to which the values of the total wait time and total run time of process are assigned respectively.  
To do this proc structure has been extended with:  
    ```wtime, ctime, rtime, twtime, iotime```  
In every call of trap.c at the moment tick is incremented updateRuntime Function is invoked
```
void updateRuntime(void){
  struct proc *p;
  acquire(&ptable.lock);
  for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
    if(p->state==RUNNING){
      p->rtime++;
      #ifdef MLFQ
      p->mlfq_time--;
      p->queue[p->cur_q]++;
      #endif
    }
    if(p->state==RUNNABLE){
      p->wtime++;
      p->twtime++;
    }
    if(p->state==SLEEPING)
      p->iotime++;
    
  }
  release(&ptable.lock);
}
```
If process is 
* sleeping iotime is incremented
* running runtime is incremented
* runnable waittime and totatl waitime is incremented
* If MLFQ the the time stayed in mlfq queue is decremented and num ticks in that  queue for process is incremented.
> We assume only a process in runnable state to be waiting, a sleeping process does not count for wait as it is a voluntary exit.
> wtime is reset to 0 everytime the scheduler picks it up or every time it changes queue