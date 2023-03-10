
./get_test_files.sh

echo "RUNNING: kort_bericht"
./benchmark.sh ../persen ./test_files/kort_bericht.txt kort_bericht.csv
echo "RUNNING: trump"
./benchmark.sh ../persen ./test_files/trump.txt trump.csv
echo "RUNNING: random_image"
./benchmark.sh ../persen ./test_files/random_image random_image.csv