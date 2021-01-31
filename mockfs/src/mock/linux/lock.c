#include "mock/linux/lock.h"
#include "mock/linux/printk.h"

void spin_lock_init(spinlock_t * lock)
{
    lock->count = 0;
}
void spin_lock(spinlock_t * lock)
{
    if(lock->count == 0){
        ++(lock->count);
    }
    else{
        pr_debug("DEBUG: spin lock failed, count:[%d]\n", lock->count);
    }
}
void spin_unlock(spinlock_t * lock)
{
    if(lock->count > 0){
        --(lock->count);
    }
    else{
        pr_debug("DEBUG: spinlock unlock failed, count:[%d]\n", lock->count);
    }
}
// 1 - trylock sucess; 0 -> trylock failed
int spin_trylock(spinlock_t *lock)
{
    if(lock->count == 0){
        ++(lock->count);
        return 1;
    }
    else{
        return 0;
    }
}

// 1 - means(count>0) - locked, 0 - means(count<=0) - unlocked
int spin_is_locked(spinlock_t *lock)
{
    return lock->count > 0;
}




void down(struct semaphore *sem)
{
	if(sem->count > 0){
		sem->count --;
	}
	else{
		pr_debug("DEBUG: down wait, count:[%d]\n", sem->count);//wait;
	}
}
void up(struct semaphore *sem)
{
	if(sem->count == 0){
		sem->count ++;
	}
	else{
		pr_debug("DEBUG: up wait, count:[%d]\n", sem->count);
	}
}
// 0 -> acquired successfully, 1 -> try acquired failed
int down_trylock(struct semaphore *sem)
{
	int count = sem->count - 1;	
	if(count >= 0){
		sem->count = count;
	}
    return count < 0;	
}



void init_rwsem(struct rw_semaphore *sem)
{
    sem->count = RWSEM_UNLOCKED_VALUE;
    sem->write_count = RWSEM_UNLOCKED_VALUE;
}


void down_read(struct rw_semaphore *sem)
{
    if(sem->count >0){
        sem->count --;
    }
    else{
        pr_debug("DEBUG: down_read wait, count:[%d]\n", sem->count);
    }
}
/*
 * trylock for reading -- returns 1 if successful, 0 if contention
 */
int down_read_trylock(struct rw_semaphore *sem)
{
	int count = sem->count - 1;	
	if(count >= 0){
		sem->count = count;
	}
    return count < 0;	
}
void up_read(struct rw_semaphore *sem)
{
	if(sem->count == 0){
		sem->count ++;
	}
	else{
		pr_debug("DEBUG: up wait, count:[%d]\n", sem->count);
	}
}



void down_write(struct rw_semaphore *sem)
{
    if(sem->write_count >0){
        sem->write_count --;
    }
    else{
        pr_debug("DEBUG: down_write wait, write_count:[%d]\n", sem->write_count);
    }
}
/*
 * trylock for writing -- returns 1 if successful, 0 if contention
 */
int down_write_trylock(struct rw_semaphore *sem)
{
	int count = sem->write_count - 1;	
	if(count >= 0){
		sem->write_count = count;
	}
    return count < 0;	
}
void up_write(struct rw_semaphore *sem)
{
	if(sem->write_count == 0){
		sem->write_count ++;
	}
	else{
		pr_debug("DEBUG: up_write wait, write count:[%d]\n", sem->write_count);
	}
}

void downgrade_write(struct rw_semaphore *sem)
{
    up_write(sem);
    down_read(sem);
}


void mutex_init(struct mutex * lock)
{
    lock->count = 0;
}
void mutex_lock(struct mutex * lock)
{
    if(lock->count == 0){
        lock->count ++;
    }
    else{
        pr_debug("mutex lock wait....., count:[%d]\n", lock->count);
    }
}
void mutex_unlock(struct mutex * lock)
{
    if(lock->count > 0){
        lock->count --;
    }
    else{
        pr_debug("mutex unlock failed, count:[%d]\n", lock->count);
    }
}
// 1 - locked successfully
int mutex_trylock(struct mutex * lock)
{
    if(lock->count == 0){
        lock->count --;
        return 1;
    }
    return 0;
}





//=======
//
//int spin_lock_init(struct spin_lock_t * spin_lock)
//{
//    spin_lock->count = 0;
//    return 0;
//}
//void spin_lock(struct spin_lock_t * spin_lock)
//{
//    if(spin_lock->count == 0)
//        ++(spin_lock->count);
//    else{
//        pr_debug("please wait.....");
//    }
//}
//void spin_unlock(struct spin_lock_t * spin_lock)
//{
//    if(spin_lock->count > 0)
//        --(spin_lock->count);
//    else{
//        pr_debug("spin-lock is unlocked");
//    }
//}
//int spin_trylock(struct spin_lock_t * spin_lock)
//{
//    if(spin_lock->count == 0){
//        ++(spin_lock->count);
//        return 1; // means successful
//    }
//    else{
//        pr_debug("DEBUG: spin-lock is locking, count[%d", spin_lock->count);
//        return 0; // trylock failed
//    }
//}
//int spin_is_locked(struct spin_lock * spin_lock)
//{
//    return (spin_lock->count >0);
//}
//
//
////sempaphore:  down first and then up
//void down(struct semaphore *sem)
//{
//    if(sem->count == 1){
//        --(sem->count);
//    }
//    else{
//        pr_debug("DEBUG:count[%d] != 1, can not down", sem->count);
//    }
//}
//
//
//void up(struct semaphore *sem)
//{
//    if(sem->count == 0){
//        ++(sem->count);
//    }
//    else{
//        pr_debug("DEBUG: count[%d] != 0, can not up", sem->count);
//    }
//}
//int down_interruptible(struct semaphore *sem){
//    down(sem);
//    return 0;
//}
//int down_killable(struct semaphore *sem){
//    down(sem);
//    return 0;
//}
//int down_trylock(struct semaphore *sem){
//    if(sem->count > 0){
//        --(sem->count);
//        return 0;
//    }
//    else{
//        pr_debug("trylock semphone failed, count:%d", sem->count);
//        return 1;
//    }
//}
//int down_timeout(struct semaphore *sem, long jiffies){
//    down(sem);
//    return 0;
//}
