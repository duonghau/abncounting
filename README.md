# abncounting
This is a implementation of smart counting method. It is written in C++ and Python. This program follow 3 steps:
## Steps
1. Filter alignment by length, identity, mismatch
2. Ajust the position of reads mapped if they are overlap
3. Do smart counting: TODO

## Install

### Python
**Require:**

1. Python 2.7 or higer
2. pysam package.([On github](https://github.com/pysam-developers/pysam))

**Run:**

```
python samprocess.py [-h] [-sam samfile] [-f final file] [-l len file] [-i identity] [-m minlen]
```
Example:

``` 
python samprocess.py -sam MH0009.sam -f MH0009.final -l MH0009.len -i 0.9 -m 35
```

### C++

**Require:**

1. Boost library ([Boost](http://www.boost.org/))
2. c++11

**Build**

```
git clone https://github.com/duonghau/abncounting.git
cd abncounting/cpp
make
make Clean
```
**Run**

```
./Samprocess [-sam samfile] [-f final file] [-l len file] [-i identity] [-m minlen]
```

## Algorithm  
### Workflow  

1. Filter unmapped reads
2. Filter reads by length and identity
3. Ajusment:  
 3. If a read mapped multiple time on to a scaffold, remove it if their positions are close together(< read_length)  
 3. If foward sequence and reverse sequence are overlap in a scaffold, ajust their position

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
Identity: (73-7)/73= 0.904109589041
```

### Ajustment

We have 3 case:

**Case 1:** one read mapped one time to a scaffold, do nothing  

**Case 2:** one read(1 or 2) mapped several time to a scaffold. Check if they are closer together, if two
alignment is close (distance<read length), remove the shorter. Other wise, do nothing.  
Example:

```
read_1:             -----------  
read_1:                             --------------  
scaffold_1: ----------------------------------------------------------  
Concluse: They are close, remove the fisrt one.  

Before:  
ERR011129.2993931_1 scaffold_19 3924 3968  
ERR011129.2993931_1 scaffold_19 3905 3946  
After:
ERR011129.2993931_1 scaffold_19 3924 3968

```
**Case 3:** read_1 and read_2 mapped to a scaffold, check if they are overlap. If they are overlap, ajust the position of read_2. Check read_2 mapped length after ajustment.  

```
Example:  
read_1:             --------------  
read_2:                      ---------------  
scaffold_1: ----------------------------------------------------------  
```
Concluse: Overlap, need to ajust position of read_2. The read after ajustment:

```
Example:  
read_1:             --------------  
read_2:                           ----------  
scaffold_1: ----------------------------------------------------------  
```

There are 4 subcases:

**Case 3.1:**: read_1 is in the left of read_2(principal)

```
read_1:             --------------  
read_2:                      ---------------  
scaffold_1: ----------------------------------------------------------  

Condition: read_1.end - read_2.start >=0 and read_2.end - read_1.start >=0 and read_2.start> read_1.start
Action: read_2.start=read_1.end+1

before
ERR011130.16675_1 scaffold_222 758 802
ERR011130.16675_2 scaffold_222 769 813
after
ERR011130.16675_1 scaffold_222 758 802
ERR011130.16675_2 scaffold_222 803 813(will be remove soon because mapped length< min length)
```

**Case 3.2:**: read_1 is in the right of read_2

```
read_1:                       --------------  
read_2:             ---------------  
scaffold_1: ----------------------------------------------------------  

Condition: read_2.end - read_1.start >=0 and read_1.end - read_2.start >=0 and read_1.start>read_2.start
Action: read_2.end=read_1.start-1

before
ERR011130.16498_1 scaffold_16 3928 3972
ERR011130.16498_2 scaffold_16 3924 3968
after
ERR011130.16498_1 scaffold_16 3928 3972
ERR011130.16498_2 scaffold_16 3924 3927(will be remove soon because mapped length < min length)
```

**Case 3.3:**: read_2 in read_1

```
read_1:              --------------  
read_2:               -----------
scaffold_1: ----------------------------------------------------------  

Condition: read_2.start - read_1.start >= 0 and read_2.end - read_1.end <= 0
Action: remove read_2

ERR011130.75314_1 scaffold_33 3848 3891
ERR011130.75314_2 scaffold_33 3848 3890(**Will be remove**)

```

**Case 3.4:**: read_1 in read_2

```
read_1:              ----------  
read_2:            --------------
scaffold_1: ----------------------------------------------------------  

Condition: read_1.start-read_2.start >= 0 and read_1.end - read_2.end <=0
Action: remove read_1

ERR011130.221256_1 scaffold_336 256 297(**will be remove**)
ERR011130.221256_2 scaffold_336 256 299
```

## References

[Calculate percent identity](https://zombieprocess.wordpress.com/2013/05/21/calculating-percent-identity-from-sam-files/)
