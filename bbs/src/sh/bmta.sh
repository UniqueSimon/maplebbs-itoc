#!/bin/sh
#本程式用來分析連接 bbs 的 smtp 連接來源數量
cat /dev/null | awk 'BEGIN {printf("%10s    %-20s\n", "連線次數", "連線來源")} {} END{}'

cat /home/bbs/run/bmta.log.* | grep CONN | sort -k 3 -r | awk '{print $3}'| awk 'BEGIN {} {Number[$1]++} END {
  for(course in Number)
     printf("%10d    %-20s\n", Number[course], course)
}' | sort -n -r
