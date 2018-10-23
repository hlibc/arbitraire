./tests/convscaled $1 $2 $3 > log1

echo "ibase=$2;obase=$3; $1 + 0" | bc -l > log2

diff log1 log2
