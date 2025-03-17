#!/bin/bash

ids=$1

while IFS=":" read name effective real; do
    if (( real < effective )); then
        exit 0
    fi
done < database/cart$ids

exit 1
