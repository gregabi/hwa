#!/bin/bash

#  We need to be in the examples/ directory
#
# if ! ls ??-* >/dev/null 2>&1 ; then
#     echo "No example in this directory."
#     exit 1
# fi
if [ $(basename ${PWD}) != "examples" ] ; then
    echo "No example in this directory."
    exit 1
fi

#  We need a board name
#
BOARDS=${BOARD}
if [ -z "$BOARDS" ] ; then
    # BOARD=$(readlink boards/default | sed -e 's/.h//' 2>/dev/null)
    # if [ -z "$BOARD" ] ; then
    # 	echo BOARD is not defined and no default board is set
    # 	exit 1
    # fi
    BOARDS="attiny84 attiny85 nanodccduino"
fi


dirname $0
MFD=$(cd $(dirname $0) ; pwd)
echo ${MFD}

#exit 0

compiled=0 ; passed=0 ; failed=0 ; skipped=0
for BOARD in ${BOARDS} ; do

    echo -e "\nChecking all examples in \"${PWD}\" for board \"${BOARD}\":"

    for i in $(find */ -name "main.c" | sort) ; do
	echo -n "$(dirname $i): "
	cd $(dirname $i)
	bash ${MFD}/check.sh
	R=$?
	if [ $R -eq 0 ] ; then
	    compiled=$((compiled+1))
	    passed=$((passed+1))
	elif [ $R -eq 4 ] ; then
	    skipped=$((skipped+1))
	elif [ $R -eq 6 ] ; then
	    skipped=$((skipped+1))
	else
	    compiled=$((compiled+1))
	    failed=$((failed+1))
	fi
	#echo "Result = " $R
	cd - >/dev/null
    done
done
echo -e "\n$compiled compiled, $passed successed, $failed failed, $skipped skipped."