# This script loads the parametes values from the params_file and updates them with sysctl

#!/bin/bash

PARAMS_FILE="tcp_params.txt"
SERVER_IP="127.0.0.1"
SERVER_PORT="5201"

echo "Setting TCP parameters.."
while IFS= read line; do
    sysctl -w "$line" > /dev/null
done < $PARAMS_FILE

echo "Running iperf experiment.."
iperf3 -c $SERVER_IP | grep receiver | awk '{print $7}'









