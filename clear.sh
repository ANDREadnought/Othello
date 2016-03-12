#! /bin/bash
./extract.sh
rm tables/transtable.txt
rm tables/openingbook.txt
rm tables/closingbook.txt
touch tables/transtable.txt
touch tables/openingbook.txt
touch tables/closingbook.txt
./compress.sh
