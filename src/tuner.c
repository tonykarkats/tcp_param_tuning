#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <netinet/tcp.h>


#include "iperf_api.h"
#include "iperf.h"
#include "net.h"

#include "tuner.h"


int main(int argc, char *argv[])
{

	/* TODO :  Read default parameters from file */
		


	/* Perform a test and get back the metrics */
	struct iperf_test *test;
	struct timeval start, end;

	test = iperf_new_test();
	if (!test) {
		iperf_errexit(NULL, "Error while creating new test - %s", iperf_strerror(i_errno));
	}
	// TODO: Change the defaults to settings of my choosing
	iperf_defaults(test);
	test->stats_interval = 5;
	test->reporter_interval = 5;
	test->duration = 0;
	test->settings->bytes = 100000000000;

	// Test one receive
	iperf_parse_arguments(test, argc, argv);

	/* Set new parameters accordingly */
	/*TODO : Put everything in a while loop!! */
	gettimeofday(&start, NULL);
	iperf_run_client(test);
	gettimeofday(&end, NULL);
	
	//TODO: More accuracy here!
	printf("Flow of %d bytes completed in %ld sec\n", test->settings->bytes, (end.tv_sec - start.tv_sec));


	return 0;
}
