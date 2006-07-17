
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

echo $STRIP $NM

echo $* > syms.args

read_all()
{
	$NM `cat syms.args` |
	awk '/[^:]$/ {print $NF}' |
	sort |
	uniq |
	grep -v '^$' |
	grep -v '^main$' |
	grep -v '__gnu_compiled_c' |
	grep -v 'gcc2_compiled' > syms.all
	echo `cat syms.all`;
}

read_defined()
{
	$NM --defined-only `cat syms.args` |
	grep " T " |
	awk '/[^:]$/ {print $NF}' |
	sort |
	uniq |
	grep -v '^$' |
	grep -v '^main$' |
	grep -v '__gnu_compiled_c' |
	grep -v 'gcc2_compiled' > syms.def
	echo `cat syms.def`;
}

read_undefined()
{
	$NM --undefined-only `cat syms.args` |
	grep " U " |
	awk '/[^:]$/ {print $NF}' |
	sort |
	uniq |
	grep -v '^$' |
	grep -v '^main$' |
	grep -v '__gnu_compiled_c' |
	grep -v 'gcc2_compiled' > syms.undef
	echo `cat syms.undef`;
}

let passes=0;
let removed=0;

remove_sym()
{
	sym=$1;
	removed="";
	for file in `cat syms.args`; do
		for found in `$NM --defined-only $file | awk '/[^:]$/ {print $NF}' | grep $sym | grep -v grep`; do
			if [ "$found" == "$sym" ]; then
				$STRIP -N $sym -R .text.$sym -R .data.$sym $file &> /dev/null
				if [ $? != 0 ] ; then
					removed="_U_N_K_N_O_W_N_";
				else
					removed=$file
				fi
				break;
			fi
		done
		if [ "$removed" != "" ]; then
			break;
		fi
	done
	echo $removed;
}

progress=0;

echo "collecting symbols";
all=`read_all`;
echo "collecting defined symbols";
defined=`read_defined`;
echo "collecting undefined (used) symbols";
undefined=`read_undefined`;

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
		if [ $def == $undef ]; then
			found=1;
			break;
		fi
	done
	if [ $found == 0 ]; then
		file=`remove_sym $def`;
		if [ "$file" == "_U_N_K_N_O_W_N_" ]; then
			foo=bar;
		else
			echo -e "\033[0Gremoved $def from $file             ";
			let removed=$removed+1;
		fi
	fi
done

echo -e "\033[0Gremoved $removed symbols, after $passes passes"
