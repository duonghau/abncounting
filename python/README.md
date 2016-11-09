This is a program pre-process for abundance counting step. This programe aim to filter reads valid (by length, identity...), then prepaire data for abundance counting.

## Require:

1. Python 2.7 or higer
2. pysam package.([On github](https://github.com/pysam-developers/pysam))

## How to run:

```
python samprocess.py [-h] [-sam samfile] [-f final file] [-l len file] [-i identity] [-m minlen]
```

Example:

``` 
python samprocess.py -sam MH0009.sam -f MH0009.final -l MH0009.len -i 0.9 -m 35
```
## Algorithm  
### Workflow  

1. Filter unmapped reads
2. Filter reads by length and identity
3. Ajusment:
⋅⋅* If a read mapped multiple time on to a scaffold, remove it if their positions are close together(< read_length)
⋅⋅* If foward sequence and reverse sequence are overlap in a scaffold, ajust their position

### Filter reads

- Remove reads if their mapped length are smaller than min length
- Remove reads if identity is smaller than min identity

**Calculate identity:**

```
identity=match/(match + mismatch)
```
Example:  

```
pos: 6050, cigar: 2S73M
Alignment length M:73
MD:3A2A0G0A3C2C28A28
Mismath(edit) XM:7
Identity:0.904109589041
```

### Ajustment

We have 3 case:

Case 1: one read mapped one time to a scaffold, do nothing  
Case 2: one read(1 or 2) mapped several time to a scaffold. Check if they are closer together, if two
alignment is close (distance<read length), remove the shorter. Other wise, do nothing.  
Example:  
read_1:             -----------  
read_1:                             --------------  
scaffold_1: ----------------------------------------------------------  
Concluse: They are close, remove the fisrt one.  
Case 3: read_1 and read_2 mapped to a scaffold, check if they are overlap. If they are overlap, ajust the position of read_2. Check read_2 mapped length after ajustment.  
Example:  
read_1:             --------------  
read_2:                      ---------------  
scaffold_1: ----------------------------------------------------------  
Concluse: Overlap  

## References

[Calculate percent identity](https://zombieprocess.wordpress.com/2013/05/21/calculating-percent-identity-from-sam-files/)
