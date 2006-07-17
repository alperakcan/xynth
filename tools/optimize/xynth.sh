#!/bin/bash

rm -f objs.txt
echo > objs.txt

for file in `find ../../ -type f -name \*.o`; do
	echo $file |
	     grep -v widget |
	     grep -v tools |
	     grep -v themes |
	     grep -v demoalp >> objs.txt
done

sh optim.sh `cat objs.txt`
