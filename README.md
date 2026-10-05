# mpileup2readcounts

Fast, per-nucleotide read counts from `samtools mpileup` output.

## Why

Many variant-calling and QC workflows (somatic mutation calling, allele-specific
expression, sample-identity checks) need simple counts of how many reads support
A, C, G, T, N and indels at each genomic position. `samtools mpileup` produces
that information, but in a compact, hard-to-parse string format.

`mpileup2readcounts` is a small C++ program that reads mpileup output from a pipe
and turns it into one tab-separated row of counts per position. Because it reads
from `stdin`, it works with whichever `samtools` version you already have
installed.

It is used upstream of [Arnav](https://github.com/gatoravi/arnav), a statistical
method for calling variants from RNA-seq data.

## Install

Requires a C++ compiler and CMake.

```bash
git clone https://github.com/gatoravi/mpileup2readcounts
cd mpileup2readcounts
mkdir build && cd build
cmake ..
make
```

## Usage

```bash
samtools mpileup -f ref.fa -l regions.bed alignments.bam | ./mpileup2readcounts sample1
```

The optional argument (`sample1`) is an identifier written to the last column of
every row (default `NA`), which makes it easy to concatenate output from many
samples.

Tested with samtools v1.1.

## Example output

```
chr  pos    depth  ref_base  refcount  altcount  acount  ccount  gcount  tcount  ncount  indelcount  identifier
1    10205  3      A         3         0         3       0       0       0       0       0           sample1
1    10206  3      A         3         0         3       0       0       0       0       0           sample1
1    10207  4      C         4         0         0       4       0       0       0       0           sample1
1    10208  4      C         4         0         0       4       0       0       0       0           sample1
1    10209  4      C         4         0         0       4       0       0       0       0           sample1
1    10210  4      T         4         0         0       0       0       4       0       0           sample1
1    10211  4      A         4         0         4       0       0       0       0       0           sample1
1    10212  3      A         3         0         3       0       0       0       0       0           sample1
1    10213  4      C         4         0         0       4       0       0       0       0           sample1
```

| Column | Meaning |
| --- | --- |
| `chr`, `pos` | Genomic position |
| `depth` | Read depth reported by mpileup |
| `ref_base` | Reference base at this position |
| `refcount` | Reads supporting the reference base |
| `altcount` | Reads supporting a non-reference base (A, C, G, T or N) |
| `acount` … `ncount` | Reads supporting each nucleotide |
| `indelcount` | Reads with an insertion or deletion at this position |
| `identifier` | Sample name passed on the command line |

## License

MIT. See [LICENSE](LICENSE).
