# abncounting
This is a implementation of smart counting method. It is written in C++. This program follow 4 steps:
## Steps
1. Filter alignment by length, identity, mismatch
2. Ajust the position of reads mapped if they are overlap
3. Do smart counting

## This program has 4 main files:
- Samparser.cpp
- AlignmentRecord.cpp
- Samprocess.cpp
- Main.cpp

### Samparser.cpp
Read the SAM file and convert each line to an object c++
**Function**
parser(string filename): read SAM file and return a hasmap of AlignmentRecord

### AlignmentRecord.cpp
**Properties:**
-qname: Query NAME
-flag: bitwise FLAG
-rname: Reference sequence NAME
-pos: 1-based leftmost mapping POSition
-mapq: MAPping Quality
-cigar: CIGAR string
-rnext: Ref. name of the mate/next read
-pnext: Position of the mate/next read
-tlen: observed Template LENgth
-seq: segment SEQuence
-qual: ASCII of Phred-scaled base QUALity+33
-XM, MN: obtional fields
**Functions**
-void AlignmentRecord(string alignment_record): Constructor
-int start(void): return start mapped position
-int end(void): return end position
-int mapped_length(void) return mapped length of read
