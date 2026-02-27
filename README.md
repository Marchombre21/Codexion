*This project has been created as part of the 42 curriculum by bfitte.*

# Description

**Codexion** is a multithreaded synchronization and resource management simulator. The program simulates coders (threads) who must share dongles in order to compile, debug, and refactor without reaching the burnout time limit.

# Instructions

### Execution

The source code is provided with a Makefile containing the following rules:

```bash
make : Compiles the standard codexion executable.
make clean : Deletes object files and the build folder.
make fclean : Deletes object files, the build folder and the executable.
make re : Call fclean rule and rebuild the executable.
```

### Usage

The execution needs exactly 8 arguments:
```bash
./codexion [nb_coders] [time_to_burnout] [time_to_compile] [time_to_debug] [time_to_refactor] [nb_compiles] [dongle_cooldown] [scheduler]
```
#### Parameters

- **nb_coders** : The number of coders around the table
- **time_to_burnout** : Limit time (in millisecond) to compile before burnout
- **time_to_compile** : Time (in milliseconds) during which an encoder keeps its two dongles locked.
- **time_to_debug** : Time (in milliseconds) to debug.
- **time_to_refactor** : Time (in milliseconds) to refactor.
- **nb_compiles** : Number of compilations each coder must execute to ending program.
- **dongle_cooldown** : Time (in milliseconds) during which a dongle is unavailable.
- **scheduler** : Scheduling method for resolving access conflicts. Must be strictly FIFO or EDF.

### Blocking cases handled

**Deadlock prevention:**
Circular locking is prevented by an asymmetric acquisition strategy. Even-index encoders lock their index dongle and then the previous dongle. Odd-index encoders lock the previous dongle and then their index dongle.

**Starvation prevention:**
Access to dongles is governed by dynamically evaluated queues (FIFO or EDF). EDF mode guarantees access to the mutex to the encoder whose last_comp_time is the oldest (closest to burnout), preventing resource monopolization.
FIFO mode, on the other hand, gives access to the mutex to the first encoder that requests it.

**Cooldown handling:**
The wait time due to a cooldown is exactly the time remaining before the end of that cooldown. Furthermore, when a dongle is recharging, the encoder projects its request_time to the exact timestamp of its wake-up. The scheduler treats any request located in the future as inactive, allowing other threads to use shared resources without being blocked by a waiting thread.

**Precise burnout detection:**
An independent monitor thread evaluates the survival of the encoders. If one of the encoders dies, it immediately sets simulation_state to 0, allowing for instant loop exit and a clean shutdown of the threads.

**Log serialization:**
The display function is blocked by a mutex. When an encoder wants to announce an event, it locks this access, which prevents other encoders from displaying a message. It unlocks access after displaying its message. This serialization prevents display data races and guarantees absolute chronology of events.

### Thread synchronization mechanisms

#### The mutexes :

**Resource locking :** Each dongle has its own mutex (dongles[i].lock) guaranteeing exclusive access to the resource.

**Data protection (Data Races) :** Other mutexes isolate sensitive variables. lock_coder_time protects the last compilation time, lock_coder_request protects the access to the request_time value, lock_coder_data secures the access to standard output and lock_coder_finish secures the access to the finish variable.

**Global state :** lock_sim_state secures the reading and modification of the simulation state.

#### The conditions :

Waiting for a cooldown uses ```cond_free``` via ```pthread_cond_wait```, suspending the thread until the release signal (pthread_cond_broadcast).

Cooldown time management uses ```cond_available``` via ```pthread_cond_timedwait```. The thread is put to sleep for the exact duration of the wait time calculated by ```get_end_cooldown```.

Priority conflicts trigger a wait on the ```cond_priority``` condition. The release signal is called when the dongles are released.

![Three coders with 100 compiling, 200 cooldown and fifo](schemas/codexion-stats(1).png)
## Resources

- **Threads**: (https://www.codequoi.com/threads-mutex-et-programmation-concurrente-en-c/#cr%C3%A9er-un-thread) / (https://www.geeksforgeeks.org/c/multithreading-in-c/)

- **Deadlocks**: (https://www.cs.yale.edu/homes/aspnes/pinewiki/Deadlock.html)

- **Visualizer**: (https://codexion-visualizer.sacha-dev.me/)

### AI Usage
Generative AI tools were used during development for:
- Debugging.
- Helping for the structuring of the codebase.
