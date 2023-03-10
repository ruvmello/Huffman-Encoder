PAD=$1
input=$2
ouput=$3

echo "STARTING BENCHMARK"

rm $ouput
touch $ouput

echo "MaxLength,MemoryLimit,Compressietijd (nanoseconds),Compressieratio,geheugenverbruik_bij_compressie, geheugenverbruik_bij_decompressie, decompressietijd (nanoseconds)" >> $ouput

echo "RUNNING..."

for len in {1..9}
do

  for mem in {0..9}
  do
    clear
    echo "CURRENTLY RUNNING len: $len, mem: $mem"
    echo -n "$len,$mem," >> $ouput

    # Compressietijd
    echo -n "Compressietijd: "

    start=$(date +%s%N)
    cat "$input" | $PAD -c "$len,$mem" > encode_bench.bin
    end=$(date +%s%N)
    echo -n "$(($end-$start))," >> $ouput

    echo "$(($end-$start)) (nanoseconds)"

    #Compressieratio
    echo -n "Compressieratio: "

    input_size=$(du -sb "$input" | grep -o "[0-9]*")
    new_size=$(du -sb encode_bench.bin | grep -o "[0-9]*")
    compression_ratio=$(python3 -c "print($input_size/$new_size)")
    echo -n "$compression_ratio," >> $ouput

    echo "$compression_ratio"

    # Geheugenverbruik compressie
    echo -n "Geheugenverbruik compressie: "

    cat "$input" | valgrind -q --tool=massif --stacks=yes --massif-out-file=memory.ms -- $PAD -c "$len,$mem" > encode_bench.bin
    total_bytes=$(cat ./memory.ms | grep -B 3 "peak" | grep "mem_[^_]*_B" | grep -o "[0-9]*" | paste -sd+ | bc)
    echo -n "$total_bytes," >> $ouput

    echo "$total_bytes bytes"

    # Geheugenverbruik decompressie
    echo -n "Geheugenverbruik decompressie: "

    cat ./encode_bench.bin | valgrind -q --tool=massif --stacks=yes --massif-out-file=memory.ms -- $PAD -d > decode_bench.bin
    total_bytes=$(cat ./memory.ms | grep -B 3 "peak" | grep "mem_[^_]*_B" | grep -o "[0-9]*" | paste -sd+ | bc)
    echo -n "$total_bytes," >> $ouput

    echo "$total_bytes bytes"

    # Deompressietijd
    echo -n "Decompressietijd: "

    start=$(date +%s%N)
    cat ./encode_bench.bin | $PAD -d > decode_bench.bin
    end=$(date +%s%N)
    echo "$(($end-$start))" >> $ouput

    echo "$(($end-$start)) (nanoseconds)"

  done
done

rm decode_bench.bin encode_bench.bin memory.ms
echo "ALL DONE"