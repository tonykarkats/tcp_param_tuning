#ifndef __TUNER_H
#define __TUNER_H
#endif

#include "tuner_error.h"

/* Define metrics to be used for decisions */
struct metrics {
	float latency;
	float throughput;
	float fct;
};

/* Parameter specific functions */
int load_default_params(char *config_file);
error_t set_param(const char *param, const char *value);
error_t get_param(const char *param, char **ret_value);


/* Test specific functions */
struct metrics * execute_test(char *iperf_test);

