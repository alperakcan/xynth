
# cool and ugly binary optimizer

if [ "$*" == "" ]; then
	echo "usage : $0 a.o b.o c.o ...";
	exit 1;
fi

if [ "$NM" == "" ]; then
	NM=nm;
fi

if [ "$STRIP" == "" ]; then
	STRIP=strip;
fi

echo "strip command: $STRIP, nm command:$NM";

echo $* > syms.args

read_all()
{
	echo > syms.all;
	objs=$*;
	for file in $objs; do
		for sym in `$NM -o $file |
			    awk '/[^:]$/ {print $NF}' |
			    sort |
			    uniq |
			    grep -v '^$' |
			    grep -v '^main$' |
			    grep -v '__gnu_compiled_c' |
			    grep -v 'gcc2_compiled'`; do
			echo "$file:$sym" >> syms.all;
		done
		echo `cat syms.all`;
	done;
}

read_defined()
{
	objs=$*;
	$NM -o --defined-only $objs |
	grep " T " |
	awk '/[^:]$/ {print $NF}' |
	sort |
	uniq |
	grep -v '^$' |
	grep -v '^main$' |
	grep -v '__gnu_compiled_c' |
	grep -v 'gcc2_compiled' > syms.def;
	echo `cat syms.def`;
}

read_undefined()
{
	objs=$*;
	$NM -o --undefined-only $objs |
	grep " U " |
	awk '/[^:]$/ {print $NF}' |
	sort |
	uniq |
	grep -v '^$' |
	grep -v '^main$' |
	grep -v '__gnu_compiled_c' |
	grep -v 'gcc2_compiled' > syms.undef;
	echo `cat syms.undef`;
}

remove_sym()
{
	sym=$1;
	file=$2;
	$STRIP -N $sym -R .text.$sym -R .data.$sym $file &> /dev/null
	if [ $? != 0 ] ; then
		echo "";
	else
		echo "OK";
	fi
}

let passes=0;
let removed=0;
let progress=0;

echo "collecting objects";
objs=`cat syms.args`;
echo "collecting symbols";
all=`read_all $objs`;
echo "collecting defined symbols";
defined=`read_defined $objs`;
echo "collecting undefined (used) symbols";
undefined=`read_undefined $objs`;

for def in $defined; do
	found=0;
	for undef in $undefined; do
		let progress=$passes%8;
		if [ $progress == 0 ]; then
			echo -e '\033[0Gprocessing cross references... -\c';
		elif [ $progress == 2 ]; then
			echo -e '\033[0Gprocessing cross references... \\\c';
		elif [ $progress == 4 ]; then
			echo -e '\033[0Gprocessing cross references... |\c';
		elif [ $progress == 6 ]; then
			echo -e '\033[0Gprocessing cross references... /\c';
		fi
		let passes=$passes+1;
		
		if [ "$def" == "$undef" ]; then
			found=1;
			break;
		fi
	done
	if [ $found == 0 ]; then
		files=`grep $def syms.all`;
		for file in $files; do
			sym=`echo $file | cut -d : -f 2`;
			if [ "$sym" == "$def" ]; then
				file=`echo $file | cut -d : -f 1`;
				removed_ok=`remove_sym $sym $file`;
				if [ "$removed_ok" == "OK" ]; then
					echo -e "\033[0Gremoved $sym from $file             ";
					let removed=$removed+1;
				fi
			fi
		done
	fi
done

echo -e "\033[0Gremoved $removed symbols, after $passes passes"
