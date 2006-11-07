#!/bin/bash

rm -f objs.txt
echo > objs.txt

for file in `find ../../ -type f -name \*.o`; do
	echo $file |
	     grep -v tools |
	     grep -v themes >> objs.txt
done

sh optim.sh `cat objs.txt`
