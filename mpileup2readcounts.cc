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
class mpileup_line {
    public:
        string chr;
        int pos;
        string ref_base;
        int depth;
        string bases;
        string qual;
        //Counts for different bases
        int refcount, altcount;
        int acount, ccount, gcount, tcount, ncount, indelcount;
        mpileup_line() {
            chr = ref_base = bases = qual = "NA";
            depth = pos = 0;
            refcount = altcount = 0;
            acount = ccount = gcount = tcount = ncount = 0;
            indelcount = 0;
        }
        //Set the appropriate count for total alt
        //Needs to be called before set_ref_nuc_count()
        void set_alt_count() {
            //This works since ref bases are . and ,
            altcount = acount + ccount + gcount + tcount +
                        ncount;
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
        static void print_header(ostream& out = cout) {
            out << "chr" << "\t"
                << "pos" << "\t"
                << "depth" << "\t"
                << "ref_base" << "\t"
                << "refcount" << "\t"
                << "altcount" << "\t"
                << "acount" << "\t"
                << "ccount" << "\t"
                << "gcount" << "\t"
                << "tcount" << "\t"
                << "ncount" << "\t"
                << "indelcount"
                << endl;
        }
        void print(ostream& out = cout) {
            out << chr << "\t"
                << pos << "\t"
                << depth << "\t"
                << ref_base << "\t"
                << refcount << "\t"
                << altcount << "\t"
                << acount << "\t"
                << ccount << "\t"
                << gcount << "\t"
                << tcount << "\t"
                << ncount << "\t"
                << indelcount
                << endl;
        }
};


//Parse the pileup string
void parse_bases_to_readcounts(mpileup_line& ml1) {
    for(int i = 0; i < ml1.bases.length(); i++) {
        char base = ml1.bases[i];
        string indelsize_string;
        int indelsize_int = 0;
        switch(base) {
            //Match to reference
            case '.':
            case ',':
                ml1.refcount += 1;
                break;
            case 'a':
            case 'A':
                ml1.acount += 1;
                break;
            case 'c':
            case 'C':
                ml1.ccount += 1;
                break;
            case 'g':
            case 'G':
                ml1.gcount += 1;
                break;
            case 't':
            case 'T':
                ml1.tcount += 1;
                break;
            case 'n':
            case 'N':
                ml1.ncount += 1;
                break;
            //Reference skips
            case '<':
            case '>':
                break;
            //Insertion or deletion
            case '-':
            case '+':
                ml1.indelcount += 1;
                i++;
                while(ml1.bases[i] >= 48 &&
                      ml1.bases[i] <= 57) {
                    indelsize_string = indelsize_string + ml1.bases[i];
                    i = i+1;
                }
                indelsize_int = str_to_num(indelsize_string);
                i += indelsize_int - 1;
                break;
            //End of read segment
            case '$':
                break;
            //Beginning of read segment
            case '^':
                i = i + 1;//Skip quality
                break;
            default:
                throw runtime_error("Unknown ref base: " +
                                    base);
        }
    }
    ml1.set_alt_count();
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
    mpileup_line::print_header();
    getline(cin, line);
    while(cin) {
        try {
            process_mpileup_line(line);
        } catch(const std::runtime_error& e) {
            cerr << e.what() << endl;
            cerr << "\nError parsing line " << line;
            break;
        }
        getline(cin, line);
    }
}
