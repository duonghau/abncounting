## Require:

1. g++ 4.8.4 or higer
2. Boost library 1.62.0

## Install boost library

```
wget https://sourceforge.net/projects/boost/files/boost/1.62.0/boost_1_62_0.zip
unzip boost_1_62_0.zip
cd boost_1_62_0


./bootstrap.sh \
    --prefix=$HOME/opt/boost \
    --libdir=$HOME/opt/boost/lib \
    --with-libraries=regex

./bjam --build-dir=build --layout=system install

<!-- You need insert  LD_LIBRARY_PATH -->
export LD_LIBRARY_PATH=$HOME/opt/boost/lib:$LD_LIBRARY_PATH
```

## Install abncount

```
cd abncounting
make
```