#!/bin/bash
echo -n "Clients "
echo `cat *.log | grep client | sed 's/[^0-9]*//g' | tr '\n' ' ' `
echo -n "Min "
echo `cat *.log | grep client | sed 's/[^0-9]*//g' | sort -n | head  -1 `
echo -n "Max "
echo `cat *.log | grep client | sed 's/[^0-9]*//g' | sort -n | tail  -1 `
echo -n "Bytes Received "
echo `cat *.log | grep bytes_received | sed 's/[^0-9]*//g' | tr '\n' ' ' | tr ' ' '+' ` "0" | bc
echo -n "Bytes Sent "
echo `cat *.log | grep bytes_sent | sed 's/[^0-9]*//g' | tr '\n' ' ' | tr ' ' '+' ` "0" | bc
echo -n "Files sent "
echo `cat *.log | grep send_file | wc -l`
echo -n "Files received "
echo `cat *.log | grep received_file | wc -l`
