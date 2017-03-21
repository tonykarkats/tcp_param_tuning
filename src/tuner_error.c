#include <stdio.h>
#include "tuner_error.h"

void print_error(error_t error) {
	printf("Error: %s\n", error_desc[error]);
}
