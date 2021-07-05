#!/bin/bash

pagesize=0

for i in {0..63}
do
    printf "READ $pagesize\n" >> commands
    pagesize=$(expr $pagesize + 16384)
done

