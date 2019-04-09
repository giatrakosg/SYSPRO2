#!/bin/bash
echo -n "Bytes Received "
echo `cat *.log | grep bytes_received | sed 's/[^0-9]*//g' | tr '\n' ' ' | tr ' ' '+' ` "0" | bc
echo -n "Bytes Sent "
echo `cat *.log | grep bytes_sent | sed 's/[^0-9]*//g' | tr '\n' ' ' | tr ' ' '+' ` "0" | bc
