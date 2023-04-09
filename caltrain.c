#include <pthread.h>
#include "caltrain.h"



void
station_init(struct station *station)
{
	station->seats_available = 0;
	station-> waiting_passengers = 0; 
	station-> onboard_passengers = 0; 

	pthread_mutex_init(&station->station_mutex, NULL);
	pthread_cond_init(&station->cond_train_arrived, NULL);
	pthread_cond_init(&station->cond_train_leave, NULL);
}

void
station_load_train(struct station *station, int count)
{	
	pthread_mutex_lock(&station->station_mutex);

	// check first if the train has seats available
	if(count == 0 || station->waiting_passengers == 0){
		pthread_mutex_unlock(&station->station_mutex);
	}

	else{
		// update number of seats available on the train
		station->seats_available = count;
		
		pthread_cond_broadcast(&station->cond_train_arrived);
		pthread_cond_wait(&station->cond_train_leave, &station->station_mutex);
		
		// train leaved the station
		station->seats_available = 0;
	}

	pthread_mutex_unlock(&station->station_mutex);

}

void
station_wait_for_train(struct station *station)
{
	pthread_mutex_lock(&station->station_mutex);

	station->waiting_passengers ++ ;
	
	while(station->seats_available==0){
		pthread_cond_wait(&station->cond_train_arrived, &station->station_mutex);
	}

	// get passenger onboard when a train arrives with available seats
	station->seats_available -- ;
	station->onboard_passengers ++ ;
	station->waiting_passengers -- ;

	pthread_mutex_unlock(&station->station_mutex);
}

void
station_on_board(struct station *station)
{
	pthread_mutex_lock(&station->station_mutex);

	// make sure the passenger is onboard (to be removed)
	station->onboard_passengers -- ;

	if(station->onboard_passengers==0 && (station->seats_available==0 || station->waiting_passengers==0)){
		pthread_cond_signal(&station->cond_train_leave);
	}

	pthread_mutex_unlock(&station->station_mutex);
}
