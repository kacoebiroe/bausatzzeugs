#!/bin/bash

while read foo
do
./calibrate /dev/ttyUSB0 38400

OUT=$'Hacklace Firmware Info\n'
OUT+=$'Date: '
OUT+=$(date +%Y-%m-%d)
OUT+=$'\n'
OUT+=$'Display: TC\n'
OUT+=$'OSCCAL Value: '
OUT+=$(cat Hacklace/calibration-data.h | cut -d ' ' -f3)
OUT+=$'\n'
OUT+=$'\nBitte den OSCCAL Korrektur-\nwert bei dem neu-flashen der\nFirmware in der Datei\ncalibration-data.h setzen.'

echo "$OUT" | lpr -o DocCutType=1PartialCutDoc -P "Star_TSP143_(STR_T-001)"

done
