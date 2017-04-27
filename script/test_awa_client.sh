#!/bin/sh
COUNT=""
if [ ${#@} == 0 ]; then
        echo "Usage: $0 -c <count of repeats>"
        exit
fi
while getopts c: option
do
        case "${option}"
        in
                c) COUNT=${OPTARG};;
        esac
done

for (( c=1; c<=5; c++ ))
do
   echo "Welcome $c times"
done
