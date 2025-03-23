#!/bin/bash

line=$( sed -rn '/^(.*:){3}[^0]/p' database/signed.dat | wc -l )
exit $line
