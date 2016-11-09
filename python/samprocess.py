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
            self.qname=alignment_record.query_name
            self.rname=alignment_record.reference_name
            self.start=alignment_record.reference_start
            self.end=alignment_record.reference_end
            if self.start > self.end:
                self.start,self.end= self.end, self.start
            self.strand=1 if alignment_record.is_read1 else 2

    def get_length(self):
        return self.end-self.start

    def is_valid(self, minlen):
        return True if (self.start < self.end) and (self.end-self.start>minlen) else False
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
    M=alignment_record.reference_length
    mismatch=alignment_record.get_tag("XM")
    ident = (M-mismatch)/M
    # alignment_record.reference_length = mapped length
    if alignment_record.reference_length < minlen:
        return False
    if ident < identity:
        return False
    return True

# Filter alignment record if they are unmapped, low identity or too short.
def filter_alignment(samfile, minlen, identity):
    records=dict()
    i=0 #count number of record
    lengths=0
    # for record in samfile.fetch():
    for record in samfile.head(100000):
        i+=1
        lengths+=record.infer_query_length()
        # remove if unmapped
        if record.is_unmapped:
            continue
        # check if alignment is valid
        if is_valid_alignment(record, minlen, identity):
            valid_record=AlignRecords(record)
            qname=record.query_name #query name
            rname=record.reference_name #reference name
            if qname not in records:
                records[qname]=dict()
            if rname not in records[qname]:
                records[qname][rname]=list()
            records[qname][rname].append(valid_record)
    mean_length=int(lengths/i)
    return records, mean_length

def ajust_record(record1, record2, read_length, minlen):
    # check if one read mapped multiple time to a scaffold
    if record1.strand==record2.strand:
        # check distance between 2 reads
        distance=min(abs(record2.start-record1.end),abs(record1.start-record2.end))
        # if two reads is close, keep the longer.
        if distance < read_length:
            if record1.get_length() < record2.get_length():
                record1=None
            else:
                record2=None
    # check if read pairs are overlap
    else:
        # record1 is in the left of record2
        if (record1.end - record2.start)>=0 and (record2.end - record1.start)>=0 and record2.start> record1.start:
            # determine which record is read_1
            if record1.strand==1:
                record2.start=record1.end+1
            else:
                record2.end=record1.start-1

        # record1 is in the right of record2
        elif (record2.end - record1.start)>=0 and (record1.end - record2.start)>=0 and record1.start>record2.start:
            if record1.strand==1:
                record2.end=record1.start-1
            else:
                record2.start=record1.end+1
        # record2 is in record1
        elif (record2.start - record1.start)>=0 and (record2.end - record1.end)<=0:
            record2=None
        # record1 in record2
        elif (record1.start-record2.start)>=0 and (record1.end - record2.end)<=0:
            print("before")
            print(record1)
            print(record2)
            record1=None
    # remove record if mapped length < minlen, start>end
    if record1 is not None:
        if not record1.is_valid(minlen):
            record1=None
    if record2 is not None:
        if not record2.is_valid(minlen):
            record2=None
    return (record1, record2)

def ajust_records(records, scaffolds, read_length, minlen):
    for readid in records:
        for scaffold in records[readid]:
            # if there are only one record in a group, do nothing
            # else ajust position
            if len(records[readid][scaffold])<=1:
                continue
            else:
                for record1 in records[readid][scaffold]:
                    for record2 in records[readid][scaffold]:
                        if record1==record2:
                            continue
                        if (record1 not in records[readid][scaffold]) or (record2 not in records[readid][scaffold]):
                            continue
                        index1=records[readid][scaffold].index(record1)
                        index2=records[readid][scaffold].index(record2)
                        records_tmp=ajust_record(record1, record2, read_length, minlen)
                        if records_tmp[0] is not None:
                            records[readid][scaffold][index1]=records_tmp[0]
                        if records_tmp[1] is not None:
                            records[readid][scaffold][index2]=records_tmp[1]
                        if records_tmp[0] is None:
                            records[readid][scaffold].remove(record1)
                        if records_tmp[1] is None:
                            records[readid][scaffold].remove(record2)
    return records

def write_length_file(scaffolds, output_file):
    with open(output_file,"w") as output_handle:
        for scaffold in scaffolds:
            output_handle.write("{0} {1}\n".format(scaffold, scaffolds[scaffold]))
        output_handle.close()

def strands_count(records):
    strand_count=dict()
    strand_count[1]=0
    strand_count[2]=0
    for scaffold in records:
        for record in records[scaffold]:
            strand_count[record.strand]+=1
    return strand_count

def write_final_file(records, output_file):
    with open(output_file,"w") as output_handle:
        for readid in records:
            strands=strands_count(records[readid])
            for scaffold in records[readid]:
                for record in records[readid][scaffold]:
                    output_handle.write("{0} {1}\n".format(str(record),strands[record.strand]))
        output_handle.close()

def main(sam_path, final_path, len_path, minlen, ident):
    # check if file is exist
    if not os.path.exists(sam_path):
        print("Can't find samfile at: {0}".format(sam_path))
        return
    # read samfile
    samfile=pysam.AlignmentFile(sam_path,"r")
    scaffolds=get_scaffolds_length(samfile)
    records, mean_readlength=filter_alignment(samfile, minlen, ident)
    samfile.close()
    records=ajust_records(records,scaffolds,mean_readlength, minlen)
    write_final_file(records, final_path)
    write_length_file(scaffolds, len_path)
    print("done")
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
    main(args.input_file, args.final_file, args.len_file,int(args.minlen),float(args.ident))