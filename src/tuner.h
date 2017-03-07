#ifndef __TUNER_H
#define __TUNER_H
#endif

/* Define TCP parameters */
#define TCP_RMEM_MIN 0
#define TCP_RMEM_DEF 1
#define TCP_RMEM_MAX 2
#define TCP_WMEM_MIN 3
#define TCP_WMEM_DEF 4
#define TCP_WMEM_MAX 5
#define TCP_SACK	 6

/* Define metrics to be used for decisions */
struct metrics {
	float latency;
	float throughput;
	float fct;
};


/* Parameter specific functions */
int load_default_params(char *config_file);
int set_param(int param, int value);
int get_param(int param);


/* Test specific functions */
struct metrics * execute_test(char *iperf_test);

