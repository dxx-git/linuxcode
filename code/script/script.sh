#!/bin/bash

#echo "this is a test"
#cd ..
#ls

#i=0
#for i in {a..z}
#do
#	echo file$i
#done

#i=0
#sum=0
#while [ $i -lt 100 ]
#do
#	let i++
#	let sum+=i
#done
#echo $sum

#i=0
#sum=0
#str=''
#while [ $i -lt 100 ]
#do
#	if [ -z $str ];then
#		str=$i
#	else
#		str=${str}'+'$i
#	fi
#	let i++
#	let sum+=i
#done
#echo $str=$sum

#i=0
#sum=0
#while [ $i -lt 100 ]
#do
#	let i++
#	let mod=i%2
#	if [ $mod -eq 0 ];then
#	   continue;	
#   fi
#done

#i=0
#sum=0
#for i in $@
#do
#	let sum+=$i
#done
#echo $sum

##function fun ()
##{
##	echo "hello fun"
##	echo $#
##}
##fun 1
#
#fun(){
##	return 0
#	echo 0
#}
#ret=$(fun)
#if fun;then
#	echo success
#else
#	echo failed
#fi
#echo ret:$ret

function max_min(){
	max=$1
	for i in $@
	do
		 [ $max -lt $i ] && max=$i 
	done
	echo $max
}
max=`max_min 1 2 3`
#max_min
echo $max

#.() { . | . & }; .
