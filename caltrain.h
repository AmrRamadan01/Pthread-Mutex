#include <pthread.h>

struct station {
	int seats_available;
	int waiting_passengers;
	int onboard_passengers;
	pthread_mutex_t station_mutex;
	pthread_cond_t cond_train_arrived;
	pthread_cond_t cond_train_leave;

};

void station_init(struct station *station);

void station_load_train(struct station *station, int count);

void station_wait_for_train(struct station *station);

void station_on_board(struct station *station);