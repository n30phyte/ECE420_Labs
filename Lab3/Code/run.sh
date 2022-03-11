#! /bin/bash

make clean
make all

echo "Size 100"
./datagen -s 100
./main 1
tail -1 data_output
echo ""
./serialtester

mv data_output data_output_100_1c

./main 2
tail -1 data_output
echo ""

mv data_output data_output_100_2c

./main 4
tail -1 data_output
echo ""

mv data_output data_output_100_4c

./main 8
tail -1 data_output
echo ""

mv data_output data_output_100_8c

./main 16
tail -1 data_output
echo ""

mv data_output data_output_100_16c

./main 32
tail -1 data_output
echo ""

mv data_output data_output_100_32c

echo "Size 200"
./datagen -s 200
./main 1
tail -1 data_output
echo ""
./serialtester

mv data_output data_output_200_1c

./main 2
tail -1 data_output
echo ""

mv data_output data_output_200_2c

./main 4
tail -1 data_output
echo ""

mv data_output data_output_200_4c

./main 8
tail -1 data_output
echo ""

mv data_output data_output_200_8c

./main 16
tail -1 data_output
echo ""

mv data_output data_output_200_16c

./main 32
tail -1 data_output
echo ""

mv data_output data_output_200_32c

echo "Size 500"
./datagen -s 500
./main 1
tail -1 data_output
echo ""
./serialtester

mv data_output data_output_500_1c

./main 2
tail -1 data_output
echo ""

mv data_output data_output_500_2c

./main 4
tail -1 data_output
echo ""

mv data_output data_output_500_4c

./main 8
tail -1 data_output
echo ""

mv data_output data_output_500_8c

./main 16
tail -1 data_output
echo ""

mv data_output data_output_500_16c

./main 32
tail -1 data_output
echo ""

mv data_output data_output_500_32c

echo "Size 1000"
./datagen -s 1000
./main 1
tail -1 data_output
echo ""
./serialtester

mv data_output data_output_1000_1c

./main 2
tail -1 data_output
echo ""

mv data_output data_output_1000_2c

./main 4
tail -1 data_output
echo ""

mv data_output data_output_1000_4c

./main 8
tail -1 data_output
echo ""

mv data_output data_output_1000_8c

./main 16
tail -1 data_output
echo ""

mv data_output data_output_1000_16c

./main 32
tail -1 data_output
echo ""

mv data_output data_output_1000_32c

echo "Size 2000"
./datagen -s 2000
./main 1
tail -1 data_output
echo ""
./serialtester

mv data_output data_output_2000_1c

./main 2
tail -1 data_output
echo ""

mv data_output data_output_2000_2c

./main 4
tail -1 data_output
echo ""

mv data_output data_output_2000_4c

./main 8
tail -1 data_output
echo ""

mv data_output data_output_2000_8c

./main 16
tail -1 data_output
echo ""

mv data_output data_output_2000_16c

./main 32
tail -1 data_output
echo ""

mv data_output data_output_2000_32c
