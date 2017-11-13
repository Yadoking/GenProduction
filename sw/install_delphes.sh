#!/bin/bash

BASEURL=http://cp3.irmp.ucl.ac.be/downloads/Delphes-3.4.1.tar.gz
TEMP=`basename $BASEURL`

[ -f $TEMP ] || wget $BASEURL
tar xzf $TEMP
TEMP=`readlink -f ${TEMP/.tar.gz/}`
ln -sf $TEMP ../Delphes2Flat/Delphes

cd $TEMP
./configure
make -j $(nproc)
cd ..

