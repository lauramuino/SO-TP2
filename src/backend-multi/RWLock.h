#ifndef RWLock_h
#define RWLock_h
#include <iostream>

class RWLock {
    public:
        RWLock();
        void rlock();
        void wlock();
        void runlock();
        void wunlock();

    private:
        
		int cant_lectores;		//la cantidad de lectores leyendo 
		int cant_escritores;	
		int cant_escritores_esperando;
		
		pthread_mutex_t mtx_RWL;
		pthread_cond_t barrera_lectores;
		pthread_cond_t barrera_escritores;

        //pthread_rwlock_t rwlock;
};

#endif
