#!/bin/sh
ps aux | awk '$3 > 30 {print $2}' | xargs kill -9
# 砍掉吃資源 > 30 的 process
