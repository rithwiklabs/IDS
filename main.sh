#!/bin/bash

tg_ip="<IP-ADDRESS>"
tg_us="<USERNAME>"

echo "Copying auth.log from Metaspolite 2 ..."


if scp -o HostKeyAlgorithms=+ssh-rsa "$tg_us@$tg_ip:/var/log/auth.log" ./auth.log; then
	echo "Log copied successfully"
	echo
else	
	echo "ERROR : Failed to copy auth.log"
	exit 1
fi

if [ ! -f auth.log ]; then
	echo "Error : auth.log was not created"
	exit 1
fi

echo "Processing falied SSH lgins..."

echo

#sed -E 's/pattern/replacment'

grep "sshd.*Failed password" auth.log | 
sed -E 's/^([A-Z][a-z]{2})[[:space:]]+([0-9]+) ([0-9:]+).* from ([0-9.]+) port.*/\4 \3 FAIL/' |
while read -r ip time status
do
	# =~ -----> Evaluates the format on the LHS and RHS 
	if [[ "$time" =~ ^[0-9]{2}:[0-9]{2}:[0-9]{2}$ ]]; then
		IFS=: read -r h m s <<< "$time"
		sec=$((10#$h*3600+10#$m*60+10#$s))
		echo "$ip $sec $status"
	fi
done > log.txt


set -e

echo "Compiling IDC..."
gcc ids.c -o ids

echo

echo "Running IDS..."
echo
./ids
