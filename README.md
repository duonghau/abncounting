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
### C++

**Require:**
1. Boost library ([Boost](http://www.boost.org/))
2. c++11

**Build**

```
git clone https://github.com/duonghau/abncounting.git

```