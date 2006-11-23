#!/bin/bash

if [ "$*" == "" ]; then
	echo "usage : $0 messages.po en_US";
	exit 1;
fi

getlines()
{
    file=${1}
    lines=$(cat ${file} | grep msgid | grep -v \"\" | cut -d " " -f 2- | wc -l)
    echo ${lines}
}

getline()
{
    file=${1}
    line=${2}
    string=$(cat ${file} | grep msgid | grep -v \"\" | cut -d " " -f 2- | head -n ${2} | tail -n 1)
    echo ${string}
}

lines=$(getlines ${1})
echo -e "<application>"
echo -e "\t<localization>"
echo -e "\t\t<language id=\"${2}\">"
for i in $(seq ${lines}); do
    line=$(getline ${1} ${i})
    echo -e "\t\t\t<message>"
    echo -e "\t\t\t\t<id>${line}</id>"
    echo -e "\t\t\t\t<str></str>"
    echo -e "\t\t\t</message>"
done
echo -e "\t\t</language>"
echo -e "\t</localization>"
echo -e "</application>"
