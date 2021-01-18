#include "lock.h"

int spin_lock_init(struct spin_lock_t * spin_lock)
{
    spin_lock->count = 0;
    return 0;
}
void spin_lock(struct spin_lock_t * spin_lock)
{
    if(spin_lock->count == 0)
        ++(spin_lock->count);
    else{
        pr_debug("please wait.....");
    }
}
void spin_unlock(struct spin_lock_t * spin_lock)
{
    if(spin_lock->count > 0)
        --(spin_lock->count);
    else{
        pr_debug("spin-lock is unlocked");
    }
}
int spin_trylock(struct spin_lock_t * spin_lock)
{
    if(spin_lock->count == 0){
        ++(spin_lock->count);
        return 1; // means successful
    }
    else{
        pr_debug("DEBUG: spin-lock is locking, count[%d", spin_lock->count);
        return 0; // trylock failed
    }
}
int spin_is_locked(struct spin_lock * spin_lock)
{
    return (spin_lock->count >0);
}


//sempaphore:  down first and then up
void down(struct semaphore *sem)
{
    if(sem->count == 1){
        --(sem->count);
    }
    else{
        pr_debug("DEBUG:count[%d] != 1, can not down", sem->count);
    }
}


void up(struct semaphore *sem)
{
    if(sem->count == 0){
        ++(sem->count);
    }
    else{
        pr_debug("DEBUG: count[%d] != 0, can not up", sem->count);
    }
}
int down_interruptible(struct semaphore *sem){
    down(sem);
    return 0;
}
int down_killable(struct semaphore *sem){
    down(sem);
    return 0;
}
int down_trylock(struct semaphore *sem){
    if(sem->count > 0){
        --(sem->count);
        return 0;
    }
    else{
        pr_debug("trylock semphone failed, count:%d", sem->count);
        return 1;
    }
}
int down_timeout(struct semaphore *sem, long jiffies){
    down(sem);
    return 0;
}
