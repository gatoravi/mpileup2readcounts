[![Build Status](https://travis-ci.org/gatoravi/mpileup2readcounts.svg?branch=master)](https://travis-ci.org/gatoravi/mpileup2readcounts)


## Synopsis

Get the readcounts at a locus by piping samtools mpileup output.
This allows us to be flexible with the version of samtools used.
This program has been tested on samtools v1.1

## Install
```
mkdir build
cd build
cmake ..
make
```

## Usage
```
samtools mpileup -f ref.fa -l regions.bed alignments.bam | mpileup2readcounts sample1
```

##Example output
```
chr        pos        depth      ref_base           refcount           altcount           acount     ccount     gcount     tcount     ncount     indelcount         identifier
1          10205      3          A          3          0          3          0          0          0          0          0          sample1
1          10206      3          A          3          0          3          0          0          0          0          0          sample1
1          10207      4          C          4          0          0          4          0          0          0          0          sample1
1          10208      4          C          4          0          0          4          0          0          0          0          sample1
1          10209      4          C          4          0          0          4          0          0          0          0          sample1
1          10210      4          T          4          0          0          0          0          4          0          0          sample1
1          10211      4          A          4          0          4          0          0          0          0          0          sample1
1          10212      3          A          3          0          3          0          0          0          0          0          sample1
1          10213      4          C          4          0          0          4          0          0          0          0          sample1
```
