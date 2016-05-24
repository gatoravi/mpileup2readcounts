##Synopsis
Get the readcounts at a locus by piping samtools mpileup output.
This allows us to be flexible with the version of samtools used.

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
