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


void tcp_no_metrics_save() {
	int flowsize[] = {10000, 100000, 1000000, 10000000, 100000000, 1000000000};
	FILE *fp = fopen("tcp_no_metrics_save.out", "w");
	int tcp_nms;

	for (tcp_nms=0; tcp_nms<2; tcp_nms++) {
		int i;
		for (i=0; i<sizeof(flowsize)/sizeof(flowsize[0]); i++) {
			struct metrics *ret_metrics;
			error_t err;
			char param_value[2];
			sprintf(param_value, "%d", tcp_nms);
			set_param("tcp_no_metrics_save", param_value);
			if (err=execute_test("dryad02", flowsize[i], &ret_metrics) < 0) {
				printf("Error executing test");
			}
			fprintf(fp, "%d %d %ld\n",tcp_nms, flowsize[i], ret_metrics->fct);
		}
		fprintf(fp, "\n");
	}
	fclose(fp);

}

void tcp_sack() {
	int flowsize[] = {10000, 100000, 1000000, 10000000, 100000000, 1000000000};

	FILE *fp = fopen("tcp_sack.out", "w");

	int tcp_sack;
	for (tcp_sack=0; tcp_sack<2; tcp_sack++) {
		int i;
		for (i=0; i<sizeof(flowsize)/sizeof(flowsize[0]); i++) {
			struct metrics *ret_metrics;
			error_t err;
			char param_value[2];
			sprintf(param_value, "%d", tcp_sack);
			set_param("tcp_sack", param_value);
			if (err=execute_test("dryad02", flowsize[i], &ret_metrics) < 0) {
				printf("Error executing test");
			}
			fprintf(fp, "%d %d %ld\n",tcp_sack, flowsize[i], ret_metrics->fct);
		}
		fprintf(fp, "\n");
	}
	fclose(fp);
}

void tcp_timestamps() {

	int flowsize[] = {10000, 100000, 1000000, 10000000, 100000000, 1000000000};
	FILE *fp = fopen("tcp_timestamps.out", "w");

	int tcp_timestamps;
	for (tcp_timestamps=0; tcp_timestamps<2; tcp_timestamps++) {
		int i;
		for (i=0; i<sizeof(flowsize)/sizeof(flowsize[0]); i++) {
			struct metrics *ret_metrics;
			error_t err;
			char param_value[2];
			sprintf(param_value, "%d", tcp_timestamps);
			set_param("tcp_timestamps", param_value);
			if (err=execute_test("dryad02", flowsize[i], &ret_metrics) < 0) {
				printf("Error executing test");
			}
			fprintf(fp, "%d %d %ld\n",tcp_timestamps, flowsize[i], ret_metrics->fct);
		}
		fprintf(fp, "\n");
	}
	fclose(fp);
}	

void tcp_mem_experiments() {
	
	// Testing impact of tcp_wmem value
	int start = 1024;
	int end = start * 100;
	int stride = 1024;
	int wmem;
	int flowsize[] = {10000, 100000, 1000000, 10000000, 100000000};
	FILE *fp = fopen("tcp_mem.out", "w");

	int i=0;
	for (i=0; i<sizeof(flowsize)/sizeof(flowsize[0]); i++) {
		for (wmem = start; wmem < end; wmem += stride) {
			struct metrics *ret_metrics;
			error_t err;
			char param_value[50];
			sprintf(param_value, "1024 %d %d", wmem, wmem);
			printf("%s", param_value);
			set_param("tcp_wmem", param_value);
			printf("Running test for flow_size = %d and wmem = %d\n", flowsize[i], wmem);
			if (err=execute_test("dryad02", flowsize[i], &ret_metrics) < 0) {
				printf("Error executing test");
			}
			size_t written = fprintf(fp, "%d %d %ld\n", flowsize[i], wmem, ret_metrics->fct);

		}
		fprintf(fp, "\n");	
	}
	fclose(fp);

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
		return E_IPERF_TEST;
	}
	(void) gettimeofday(&end, NULL);

	/* Report results */
	*ret_metrics = (struct metrics *) malloc(sizeof(struct metrics));
	(*ret_metrics)->fct = ((end.tv_sec * 1000000 + end.tv_usec)
                       - (start.tv_sec * 1000000 + start.tv_usec));
	//TODO: Also add throughput and size sent
	iperf_reset_test(test);
	return E_SUCCESS;
}

int main(int argc, char *argv[])
{

	//tcp_mem_experiments();
	//tcp_timestamps();
	//tcp_sack();
	tcp_no_metrics_save();
//	struct metrics *ret_metrics;
//	error_t err;
//	if (err=execute_test("dryad02", 10000000, &ret_metrics) < 0) {
//		printf("Error executing test");
//	}
//	printf("Flow of %d bytes completed in %.3f microseconds\n", 10000000, ret_metrics->fct);
//
//	/* Perform a test and get back the metrics */
//	char *param_value;
//	get_param("tcp_mem", &param_value);
//	printf("tcp_mem = %s", param_value);

	return 0;
}
