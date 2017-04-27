awa-client-set /1/1/2=0
sleep 1
awa-client-define -o 3335 -j Colour -y single -r 5706 -n "Colour Value" -t string -u single -q optional -k r
sleep 1
awa-client-set -c /3335/0
sleep 1
awa-client-set -c /3335/0/5706
