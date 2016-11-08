# _*_ coding:utf-8 _*_
from __future__ import division
import sys
import os
import pysam
import argparse

class AlignRecords(object):
    """Tiny version of pysam.AlignedSegment"""
    qname=""
    rname=""
    start=1
    end=1
    strand=1

    def __init__(self, alignment_record=None):
        if alignment_record is not None:
            this.qname=alignment_record.query_name
            this.rname=alignment_record.reference_id
            this.start=alignment_record.reference_start
            this.end=alignment_record.reference_end
            this.strand=1 if alignment_record.is_read1 else 2

    # qname_strand rname start end
    def __str__(self):
        return "{0}_{1} {2} {3} {4}".format(self.qname, self.strand, self.rname,
            self.start,self.end)
def get_scaffolds_length(samfile):
    scaffolds=dict()
    for scaffold in samfile.header["SQ"]:
        scaffolds[scaffold["SN"]]=scaffold["LN"]
    return scaffolds

def is_valid_alignment(alignment_record, minlen, identity):
    # print("----------------------------------------------")
    # print("pos: {0}, cigar: {1}, start:{2}, end:{3}".format(alignment_record.pos, alignment_record.cigarstring, alignment_record.reference_start, alignment_record.reference_end))
    # print("Alignment length M:{0}".format(alignment_record.reference_length))
    # print("MD:{0}".format(alignment_record.get_tag("MD")))
    # print("Mismath(edit) XM:{0}".format(alignment_record.get_tag("XM")))
    # identity=match/(match+mismatch)=(M-XM)/M
    M=alignment_record.reference_length
    mismatch=alignment_record.get_tag("XM")
    ident = (M-mismatch)/M
    # print("Identity:{0}".format(ident))
    # alignment_record.reference_length = mapped length
    if alignment_record.reference_length < minlen:
        return False
    if ident < identity:
        return False
    return True

# Filter alignment record if they are unmapped, low identity or too short.
def filter_alignment(samfile, minlen, identity):
    for record in samfile.head(10):
        print("Is valid:{0}".format(is_valid_alignment(record,minlen,identity)))
    records=dict()


def write_length_file(scaffolds, output_file):
    with open(output_file,"w") as output_handle:
        for scaffold in scaffolds:
            output_handle.write("{0} {1}\n".format(scaffold, scaffolds[scaffold]))
        output_handle.close()

def write_final_file(records, output_file):
    pass

def main(sam_path, final_path, len_path, ident, minlen):
    print("Min ident:{0}".format(ident))
    print("Min len:{0}".format(minlen))
    # check if file is exist
    if not os.path.exists(sam_path):
        print("Can't find samfile at: {0}".format(sam_path))
        return
    # read samfile
    samfile=pysam.AlignmentFile(sam_path,"r")
    scaffolds=get_scaffolds_length(samfile)
    filter_alignment(samfile, minlen, ident)
    samfile.close()
if __name__=="__main__":
    if len(sys.argv)==1:
        print("python samprocess.py [-h] [-sam samfile] [-f final file] [-l len file] [-i identity] [-m minlen]")
        quit()
    parser = argparse.ArgumentParser()
    parser.add_argument('-sam', action="store", default="input.sam", dest="input_file",help='Input samfile')
    parser.add_argument('-f', action="store", default="output.final", dest="final_file",help='Final file')
    parser.add_argument('-l', action="store", default="output.len", dest="len_file",help='Len file')
    parser.add_argument('-i', action="store", default=0.9, dest="ident",help='Identity')
    parser.add_argument('-m', action="store", dest="minlen",help='Min length')
    args = parser.parse_args()
    main(args.input_file, args.final_file, args.len_file,float(args.ident),int(args.minlen))