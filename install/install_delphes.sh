#!/bin/bash

BASEURL=http://cp3.irmp.ucl.ac.be/downloads/Delphes-3.4.1.tar.gz
TEMP=`basename $BASEURL`

[ -f $TEMP ] || wget $BASEURL
tar xzf $TEMP
mv ${TEMP/.tar.gz/} ../
TEMP=`readlink -f ../${TEMP/.tar.gz/}`
ln -sf $TEMP Delphes
ln -sf $TEMP ../Delphes2Flat/Delphes

cd Delphes
./configure
make -j $(nproc)
cd ..

