#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <linux/sysctl.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <netinet/tcp.h>

#include "iperf_api.h"
#include "iperf.h"
#include "net.h"

#include "tuner.h"


void perform_experiment(const char *server_name, const char *param, int pstart, int pend, int pstride,
						int fstart, int fend, int fstride)
{
	//int flowsize[] = {10000, 100000, 500000, 1000000, 5000000, 10000000, 50000000, 100000000};
	char filename[256];
	strcpy(filename, param);
	printf("%s\n", filename);
	FILE *fp = fopen(strcat(filename, ".out"), "w");
	int param_value;
	fprintf(fp, "Experiment\n--------------------------------------------\nParameter = %s\nValues [%d, %d] with stride %d\n"
				 "Flow sizes = [%d, %d] with stride %d\n--------------------------------------------\n",
			param, pstart, pend, pstride, fstart, fend, fstride);
	
	/* Get initial value of parameter so as to reset it in the end */
	char * init_value;
	get_param(param, &init_value);
	char init_value_2[10];
	strcpy(init_value_2, init_value);
	printf("Initial param value = %s\n", init_value_2);

	for (param_value = pstart; param_value <= pend; param_value += pstride) {
		int fsize;
		char param_val[30];
		if (strcmp(param, "tcp_rmem") && strcmp(param, "tcp_wmem")){
			sprintf(param_val, "%d", param_value);
		}
		else
			sprintf(param_val, "4096 %d %d", param_value, param_value);

		if (set_param(param, param_val) < 0) {
			fprintf(fp, "Parameter %s either does not exist or was not set correctly!!\n", param);
			return;
		};
		//for (i=0; i<sizeof(flowsize)/sizeof(flowsize[0]); i++) {
		for (fsize = fstart; fsize <= fend; fsize += fstride) {
			struct metrics *ret_metrics;
			error_t err;
			while (err=execute_test(server_name, fsize, &ret_metrics) < 0) {
				printf("Error executing test.. Retrying..");
				sleep(2);
			}
			sleep(1);
			fprintf(fp, "%d %d %ld\n", fsize, param_value, ret_metrics->fct);
		}
		fprintf(fp, "\n");
	}
	fclose(fp);

	printf("Initial param value = %s\n", init_value_2);
	printf("Resetting parameter value to %s\n", init_value_2);
	set_param(param, init_value_2);
}


void print_error(error_t error) {
	printf("Error: %s\n", error_desc[error]);
}

/*
 * Set the parameter @name to the value @value using sysctl.
 * Only TCP parameters are taken into account, so net.ipv4. is appended to the 
 * parameter name for sysctl.
 */
error_t set_param(const char *name,  const char *value)
{

	//XXX: Faster if I write to /proc ? --> Check!
	char command[256];
	if (strstr(value, " "))
		snprintf(command, sizeof(command), "/sbin/sysctl -w net.ipv4.%s=\"%s\"", name, value);
	else
		snprintf(command, sizeof(command), "/sbin/sysctl -w net.ipv4.%s=%s", name, value);

	printf("SET COMMAND = %s\n", command);

	if (system(command) != 0) {
		return E_INVALID_PARAM;	
	}

	return E_SUCCESS;
}

/*
 * Return the value of the parameter @name into @ret_value.
 */
error_t get_param(const char *name, char **ret_value)
{
	char command[256];
	snprintf(command, sizeof(command), "/sbin/sysctl net.ipv4.%s", name);
	printf("GET COMMAND = %s\n", command);

	FILE *fp = popen(command, "r");
	if (fp == NULL) {
		return E_INVALID_PARAM;
	}

	*ret_value = (char *)malloc(128*sizeof(char));

	char output[128];
	if (fgets(output, 128, fp) != NULL) {
		strtok(output, "=");
		*ret_value = strtok(NULL, "=");
	}

	pclose(fp);

	return E_SUCCESS;
}

/* Executes an iperf test of flow size @flow_size to the specified host */

error_t execute_test(char *server_hostname, int flow_size, struct metrics **ret_metrics)
{
	struct iperf_test *test;
	struct timeval start, end;

	test = iperf_new_test();
	if (!test) {
		printf("COULD NOT INITIALIZE TEST\n");
		return E_IPERF_TEST;	
	}

	/* Test settings */
	iperf_defaults(test);
	iperf_set_test_role(test, 'c');
	test->stats_interval = 0;
	test->reporter_interval = 0;
	test->server_hostname = server_hostname;
	test->duration = 0;
	test->settings->bytes = flow_size;

	/* Run the test */
	(void) gettimeofday(&start, NULL);
	if (iperf_run_client(test) < 0) {
		printf("COULD NOT RUN CLIENT\n");
		return E_IPERF_TEST;
	}
	(void) gettimeofday(&end, NULL);

	/* Report results */
	*ret_metrics = (struct metrics *) malloc(sizeof(struct metrics));
	(*ret_metrics)->fct = ((end.tv_sec * 1000000 + end.tv_usec)
                       - (start.tv_sec * 1000000 + start.tv_usec));
	int i=0;
	for (i=0; i<3; i++) {
		(*ret_metrics)->cpu_util[i] = test->cpu_util[i];
		//printf("%lf ", test->cpu_util[i]);
	}
	printf("\n");
	//TODO: Also add throughput and size sent
	iperf_reset_test(test);
	return E_SUCCESS;
}

void run_all_experiments(int fstart, int fend, int fstride) {

//perform_experiment("dryad02", "tcp_rmem", 4096, 65536, 4096, fstart, fend, fstride);
perform_experiment("dryad02", "tcp_timestamps", 0, 1, 1, fstart, fend, fstride);
//perform_experiment("dryad02", "tcp_autocorking", 0, 1, 1, fstart, fend, fstride);
perform_experiment("dryad02", "tcp_adv_win_scale", 0, 3, 1, fstart, fend, fstride);
perform_experiment("dryad02", "tcp_app_win", 29, 31, 1, fstart, fend, fstride);
perform_experiment("dryad02", "tcp_abort_on_overflow", 0, 1, 1, fstart, fend, fstride);
perform_experiment("dryad02", "tcp_window_scaling", 0, 1, 1, fstart, fend, fstride);
perform_experiment("dryad02", "tcp_sack", 0, 1, 1, fstart, fend, fstride);
perform_experiment("dryad02", "tcp_dsack", 0, 1, 1, fstart, fend, fstride);
perform_experiment("dryad02", "tcp_syn_retries", 1, 5, 1, fstart, fend, fstride);
perform_experiment("dryad02", "tcp_synack_retries", 1, 5, 1, fstart, fend, fstride);
perform_experiment("dryad02", "tcp_retries1", 1, 10, 1, fstart, fend, fstride);
perform_experiment("dryad02", "tcp_retries2", 1, 10, 1, fstart, fend, fstride);
perform_experiment("dryad02", "tcp_syncookies", 0, 1, 1, fstart, fend, fstride);
perform_experiment("dryad02", "tcp_tw_recycle", 0, 1, 1, fstart, fend, fstride);
perform_experiment("dryad02", "tcp_fack", 0, 1, 1, fstart, fend, fstride);
perform_experiment("dryad02", "tcp_reordering", 0, 10, 1, fstart, fend, fstride);
perform_experiment("dryad02", "tcp_ecn", 0, 2, 1, fstart, fend, fstride);
perform_experiment("dryad02", "tcp_tw_reuse", 0, 1, 1, fstart, fend, fstride);
perform_experiment("dryad02", "tcp_frto", 0, 2, 1, fstart, fend, fstride);
perform_experiment("dryad02", "tcp_frto_response", 0, 1, 1, fstart, fend, fstride);
perform_experiment("dryad02", "tcp_no_metrics_save", 0, 1, 1, fstart, fend, fstride);
perform_experiment("dryad02", "tcp_low_latency", 0, 1, 1, fstart, fend, fstride);
perform_experiment("dryad02", "tcp_mtu_probing", 0, 1, 1, fstart, fend, fstride);
perform_experiment("dryad02", "tcp_slow_start_after_idle", 0, 1, 1, fstart, fend, fstride);
//perform_experiment("dryad02", "tcp_early_retrans", 0, 4, 1, fstart, fend, fstride);
perform_experiment("dryad02", "tcp_base_mss", 256, 2048, 256, fstart, fend, fstride);
perform_experiment("dryad02", "tcp_mtu_probing", 0, 1, 1, fstart, fend, fstride);
perform_experiment("dryad02", "tcp_slow_start_after_idle", 0, 1, 1, fstart, fend, fstride);
perform_experiment("dryad02", "tcp_orphan_retries", 0, 5, 1, fstart, fend, fstride);
//perform_experiment("dryad02", "tcp_stdurg", 0, 1, 1, fstart, fend, fstride);

}



int main(int argc, char *argv[])
{
	/* All TCP experiments for */
	run_all_experiments(10000, 100000, 10000);
	return 0;
}
