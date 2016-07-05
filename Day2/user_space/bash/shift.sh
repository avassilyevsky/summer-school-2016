#!/bin/bash

declare -i SRC_INT
declare -i SHIFT_N
HexDigits=({0..9} {A..F})
SRC_INT=$1
SHIFT_DIR=$2
SHIFT_N=$3

OutShiftReg()
{

    for i in `seq 63 -1 0`;do
	echo -n ${SHIFT_REG[$i]}
    done
    echo 
}
NumToBitArray()
{
    for i in `seq 0 63`;do
	if [ `expr $SRC_INT % 2` -eq 1 ]
	then
	    SHIFT_REG[$i]=1
	else
	    SHIFT_REG[$i]=0
	fi
        SRC_INT=`expr $SRC_INT/2`
    done
}

DoShift()
{

case	$SHIFT_DIR in
    l)
    SHIFT_DIR_NAME="left"
    for ((i=63; i>=SHIFT_N;i--));do
	SHIFT_REG[$i]=${SHIFT_REG[`expr $i-$SHIFT_N`]}
    done
    for i in `seq 1 $SHIFT_N`;do
    SHIFT_REG[`expr $i-1`]=0
    done
    ;;
    r)
    SHIFT_DIR_NAME="right"
    for ((i=SHIFT_N; i<63;i++));do
	SHIFT_REG[`expr $i-$SHIFT_N`]=${SHIFT_REG[$i]}
    done
    for i in `seq 1 $SHIFT_N`;do
    SHIFT_REG[`expr 64-$i`]=0
    done

    ;;
    *)
    SHIFT_DIR_NAME="-"
    SHIFT_N=0
    ;;

esac
}

declare -i hex
QuadToHex()
{
    hex=0;
    for ((i=`expr $1+3`;i>=$1;i--));do
	hex=`expr $hex*2`
	hex=`expr $hex+${SHIFT_REG[$i]}`
#	echo i=$i hex=$hex
    done
    hex_dig=${HexDigits[$hex]}
    echo -n $hex_dig
}

NumToBitArray;
OutShiftReg;


DoShift;
echo  Source number: $1
echo  Shift direction: $SHIFT_DIR_NAME
echo  Shift amount: $SHIFT_N
echo -n 0b
OutShiftReg;
echo -n 0x
QuadToHex 60
QuadToHex 56
QuadToHex 52
QuadToHex 48
QuadToHex 44
QuadToHex 40
QuadToHex 36
QuadToHex 32
QuadToHex 28
QuadToHex 24
QuadToHex 20
QuadToHex 16
QuadToHex 12
QuadToHex 8
QuadToHex 4
QuadToHex 0
echo
