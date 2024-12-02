#!/bin/bash
rm -r objects
rm medie_implicit.csv
cd ..
SOURCE_FILES="./src/implicit/implicit.c ./src/utils.c"
OUTPUTFILE="medie_implicit.csv"

gcc $SOURCE_FILES -std=c11 -lm -fopenmp -O0 -o esegO0.o
echo "- compiled with -O0" >> $OUTPUTFILE
if [ $? -eq 0 ]; then
    echo "Compiled  successfully -O0"
else
    echo "Errore during compilation"
    exit 1
fi
./esegO0.o >> $OUTPUTFILE



gcc $SOURCE_FILES  -std=c11 -lm -fopenmp -O1 -o esegO1.o
echo "- compiled with -O1" >> $OUTPUTFILE
if [ $? -eq 0 ]; then
    echo "Compiled  successfully -O1"
else
    echo "Error during compilation"
    exit 1
fi
./esegO1.o >> $OUTPUTFILE



gcc $SOURCE_FILES  -std=c11 -lm -fopenmp -O2 -o esegO2.o
echo "- compiled with -O2" >> $OUTPUTFILE
if [ $? -eq 0 ]; then
    echo "Compiled  successfully -O2"
else
    echo "Error during compilation"
    exit 1
fi
./esegO2.o >> $OUTPUTFILE



gcc $SOURCE_FILES  -std=c11 -lm -fopenmp -O1 -march=native -o esegO1march.o
echo "- compiled with -O1 -march=native" >> $OUTPUTFILE
if [ $? -eq 0 ]; then
    echo "Compiled  successfully -O1 -march=native"
else
    echo "Error during compilation"
    exit 1
fi
./esegO1march.o >> $OUTPUTFILE



gcc $SOURCE_FILES  -std=c11 -lm -fopenmp -O2 -march=native -o esegO2march.o
echo "- compiled with -O2 -march=native" >> $OUTPUTFILE
if [ $? -eq 0 ]; then
    echo "Compiled  successfully -O2 -march=native"
else
    echo "Error during compilation"
    exit 1
fi
./esegO2march.o >> $OUTPUTFILE



gcc $SOURCE_FILES  -std=c11 -lm -fopenmp -O1 -march=native -funroll-loops -o esegO1marchfun.o
echo "- compiled with -O1 -march=native -funroll-loops" >> $OUTPUTFILE
if [ $? -eq 0 ]; then
    echo "Compiled  successfully -O1 -march=native -funroll-loops"
else
    echo "Error during compilation"
    exit 1
fi
./esegO1marchfun.o >> $OUTPUTFILE



mkdir objects
mv *.o objects