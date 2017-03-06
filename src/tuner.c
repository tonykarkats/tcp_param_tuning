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

	/* Read default parameters from file */
	


	/* Perform a test and get back the metrics */
	struct iperf_test *test;

	test = iperf_new_test();
	if (!test) {
		iperf_errexit(NULL, "Error while creating new test - %s", iperf_strerror(i_errno));
	}
	iperf_defaults(test);

	/* Set new parameters accordingly */
	/*TODO : Put everything in a while loop!! */


	return 0;
}
