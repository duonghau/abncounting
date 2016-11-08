This is a program pre-process for abundance counting step. This programe aim to filter reads valid (by length, identity...), then prepaire data for abundance counting. There are somme basic function:  

1. Filter unmapped reads
2. Filter alignment by length, identity, mismatchs
3. Ajust position(start, end) of reads

## Require:

1. Python 2.7 or higer
2. pysam package.([On github](https://github.com/pysam-developers/pysam))

## How to run:

``` python samprocess.py [-h] [-sam samfile] [-f final file] [-l len file] [-i identity] [-m minlen]
```

Example:

``` python samprocess.py -sam MH0009.sam -f MH0009.final -l MH0009.len -i 0.9 -m 35
```
## Algorithm
### Filter reads

- Remove reads if their mapped length are smaller than min length
- Remove reads if identity is smaller than min identity

## References

How calculate identity
https://zombieprocess.wordpress.com/2013/05/21/calculating-percent-identity-from-sam-files/
identity=match/(match+mismatch)