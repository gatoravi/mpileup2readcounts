/*  mpileup2readcounts.cc -- Get base counts from mpileup output

    Copyright (c) 2016, Avinash Ramu

    Author: Avinash Ramu <aramu@genome.wustl.edu>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.  */

#include <iostream>
#include <stdexcept>
#include <sstream>

using namespace std;

void usage() {
    std::cerr << "samtools mpileup -f ref.fa -l regions.bed"
              << " alignments.bam | mpileup2readcounts";
}

//Convert a number to a string
inline int str_to_num(string num) {
    stringstream ss;
    int num_uint;
    ss << num;
    ss >> num_uint;
    return num_uint;
}

//DS to hold the pertinent information
struct mpileup_line {
    string chr;
    int pos;
    string ref_base;
    int depth;
    string bases;
    string qual;
    //Counts for different bases
    int refcount, altcount;
    int acount, ccount, gcount, tcount, ncount;
    mpileup_line() {
        chr = ref_base = bases = qual = "NA";
        depth = pos = 0;
        refcount = altcount = 0;
        acount = ccount = gcount = tcount = ncount = 0;
    }
    //Set the appropriate count for ref nucleotide
    void set_ref_nuc_count() {
        switch(ref_base[0]) {
            case 'A':
                acount = refcount;
                break;
            case 'C':
                ccount = refcount;
                break;
            case 'G':
                gcount = refcount;
                break;
            case 'T':
                tcount = refcount;
                break;
            case 'N':
                ncount = refcount;
                break;
            default:
                throw runtime_error("Unknown ref base: " +
                                    ref_base);
        }
    }
    void print(ostream& out = cout) {
       out << chr << "\t"
           << pos << "\t"
           << depth << "\t"
           << refcount << "\t"
           << acount << "\t"
           << ccount << "\t"
           << gcount << "\t"
           << tcount << "\t"
           << ncount << "\t"
           << endl;
    }
};


//Parse the pileup string
void parse_bases_to_readcounts(mpileup_line& ml1) {
    for(int i = 0; i < ml1.bases.length(); i++) {
        char base = ml1.bases[i];
        switch(base) {
            case '.':
            case ',':
                ml1.refcount += 1;
                break;
        }
    }
    ml1.set_ref_nuc_count();
}

//Split the line into the required fields and parse
void process_mpileup_line(std::string line) {
    stringstream ss(line);
    mpileup_line ml1;
    string pos, depth;
    getline(ss, ml1.chr, '\t');
    getline(ss, pos, '\t');
    ml1.pos = str_to_num(pos);
    getline(ss, ml1.ref_base, '\t');
    getline(ss, depth, '\t');
    ml1.depth = str_to_num(depth);
    getline(ss, ml1.bases, '\t');
    getline(ss, ml1.qual, '\t');
    parse_bases_to_readcounts(ml1);
    ml1.print();
}

int main(int argc, char* argv[]) {
    string line;
    getline(cin, line);
    while(cin) {
        try {
            process_mpileup_line(line);
        } catch(const std::runtime_error& e) {
            cerr << e.what();
            cerr << "\nError parsing line " << line;
            break;
        }
        getline(cin, line);
    }
    cerr << endl << "Done!";
}
