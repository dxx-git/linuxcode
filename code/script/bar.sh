#!/bin/bash

##字体颜色：30黑 31红 32绿 33黄 34蓝 35紫 36深绿 37白
##字背景：40黑 41深红 42绿 43黄 44蓝 45紫色 46深绿 47白 

i=0
str=''
lable=('|' '/' '-' '\\')
index=0
while [ $i -le 100 ]
do
	((color=30+i%8))
	echo -en "\e[1;"$color"m"
	#printf "[%-100s][%d%%][%c]\r" "$str" "$i" "${lable[$index]}"
	printf "\033[?25lmprogress: \033[47m\033[1m [%-100s][%d%%][%c]\r\033[0m" "$str" "$i" "${lable[$index]}"
	str=$str"#"
	sleep 0.1
	let i++
	let index%=4
	let index++
done
echo -e "\e[1;30;m"
#echo ""

#str="hello world"
#printf "\e[35m %s \e[0m\n" "$str"
#printf "\033[41m %s \e[0m\n" "$str"
