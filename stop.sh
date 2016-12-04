#! /bin/bash

PID=`ps -ef | awk -F ' ' '{if($8 == "./Main_64"){print $2}}'`
echo ${PID};
kill -9 ${PID}

