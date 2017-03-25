/* Includes all the errors that can occur in our system */

enum _tuner_error
{
	E_SUCCESS = 0,
	E_INVALID_PARAM = -1,
	E_IPERF_TEST = -2
};

typedef enum _tuner_error error_t;

static struct _error_desc {
	int code;
	char *message;
} error_desc[] = {
	{E_SUCCESS, "No error"},
	{E_INVALID_PARAM, "Invalid Parameter"},
	{E_IPERF_TEST, "Iperf test failed"}
};
