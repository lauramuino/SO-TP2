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
		bool hay_escribiendo;	
		bool hay_escritor_esperando;
		bool lectores_esperando;
		
		pthread_mutex_t mtx_RWL;
		pthread_cond_t barrera_lectores;
		pthread_cond_t primera_barrera_lectores;
		pthread_cond_t sem_escritores;
		
};

#endif
    
