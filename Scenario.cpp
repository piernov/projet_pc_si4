#include "Scenario.h"

#include <pthread.h>

bool Scenario::benchmark_mode = false;
pthread_mutex_t Scenario::mt;
