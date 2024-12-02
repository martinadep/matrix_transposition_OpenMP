#!/bin/bash
cd ..
SOURCE_FILES="./src/implicit/block_size.c ./src/utils.c"
OUTPUT_FILE="block"
OUTPUTDIR="blockbasedsize_out"

echo "Cleaning previous build and outputs"
rm -rf objects $OUTPUTDIR *.o medie.csv

mkdir $OUTPUTDIR
mkdir objects

gcc $SOURCE_FILES -std=c11 -lm -fopenmp -o $OUTPUT_FILE
if [ $? -eq 0 ]; then
    echo "Compiled  successfully : $OUTPUT_FILE"
else
    echo "Errore during compilation"
    exit 1
fi

#module load perf

for block_size in 32 64 128 256; do

    ./$OUTPUT_FILE $block_size >> medie_size.csv
    #perf stat  -e cache-references -e cache-misses  ./$OUTPUT_FILE $block_size

    if [ $? -eq 0 ]; then
        echo "Executed successfully for block_size: $block_size"
    else
        echo "Error during execution for block_size: $block_size"
        continue
    fi

    # Move results to the block size specific directory
    cd $OUTPUTDIR || { echo "Directory 'OUTPUTDIR' not found"; exit 1; }
    mkdir -p "$block_size"
    mv "${block_size}block_"* $block_size
    cd ..
done

mv $OUTPUT_FILE objects