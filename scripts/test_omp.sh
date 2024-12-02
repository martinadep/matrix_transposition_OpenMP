#!/bin/bash

SOURCE_FILES="../explicit/explicit.c src/utils.c"
OUTPUT_FILE="open"
OUTPUTDIR="explicit_out"


rm -rf objects $OUTPUTDIR *.o medie_explicit.csv
echo "Cleaning previous build and outputs"

mkdir $OUTPUTDIR
mkdir objects

# Compile the source files
gcc $SOURCE_FILES -std=c11 -lm -fopenmp -o $OUTPUT_FILE
if [ $? -eq 0 ]; then
    echo "Compiled  successfully : $OUTPUT_FILE"
else
    echo "Errore during compilation"
    exit 1
fi
echo "matrix,tempo,threads,algo" >> medie_explicit.csv

# Loop to run the program with different thread numbers
for num_thr in 1 2 4 8 16 32 64 96; do
    ./$OUTPUT_FILE $num_thr >> medie_explicit.csv

    if [ $? -eq 0 ]; then
        echo "Executed successfully for num_thr: $num_thr"
    else
        echo "Error during execution for num_thr: $num_thr"
        continue
    fi

    # Move results to the block size specific directory
    cd $OUTPUTDIR || { echo "Directory 'OUTPUTDIR' not found"; exit 1; }
    mkdir -p "$num_thr"
    mv "${num_thr}threads_"* $num_thr
    cd ..
done

mv $OUTPUT_FILE objects