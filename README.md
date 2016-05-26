[![Build Status](https://travis-ci.org/gatoravi/mpileup2readcounts.svg?branch=master)](https://travis-ci.org/gatoravi/mpileup2readcounts)
##Synopsis
Get the readcounts at a locus by piping samtools mpileup output.
This allows us to be flexible with the version of samtools used.
This program has been tested on samtools v1.1

##Install
```
mkdir build
cd build
cmake ..
make
```

##Usage
```
samtools mpileup -f ref.fa -l regions.bed alignments.bam | mpileup2readcounts
```

##Example output
```
chr     pos     depth   ref_base        refcount        altcount        acount  ccount  gcount  tcount  ncount  indelcount
1       19190   24      G       4       0       0       0       4       0       0       1
1       566231  2457    C       2456    1       1       2456    0       0       0       1
1       568157  709     A       709     0       709     0       0       0       0       1
1       887465  155     A       155     0       155     0       0       0       0       1
1       981860  44      G       44      0       0       0       44      0       0       1
1       982800  39      T       39      0       0       0       0       39      0       1
```
