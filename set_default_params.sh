#!/bin/bash

PARAMS_FILE="dryad_default_params.txt"

echo "Setting TCP parameters.."
while IFS= read line; do
    sudo sysctl -w "$line" > /dev/null
done < $PARAMS_FILE

echo "Done!!"
