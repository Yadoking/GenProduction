#!/bin/bash

BASEURL="https://raw.githubusercontent.com/Yadoking/GenProduction/master"
MGURL="https://launchpad.net/mg5amcnlo/2.0/2.6.x/+download/MG5_aMC_v2.6.3.2.tar.gz"
MG=MG5_aMC_v2_6_3_2
WORKDIR=`pwd`

### For the users who don't want git-clone
#if [ ! -d patch ]; then
#    mkdir patch
#    for FILE in avh_olo_print.f90 cluster.py; do
#        wget $BASEURL/install/patch/$FILE -O patch/$FILE
#    done
#fi

## Download packages
[ -f `basename $MGURL` ] || wget $MGURL
tar xzf `basename $MGURL`
MG=`readlink -f $MG`

cd $MG

## Install necessary components
bin/mg5_aMC <<EOF
install update
install ninja
install pythia8
install collier
EOF

## Download lhapdf6 PDFSet(s)
## >> Please add other PDFSets if you think they are necessary
cd $MG/HEPTools/lhapdf6
bin/lhapdf get NNPDF23_lo_as_0130_qed

### For the HYU HTOP SGE cluster
#\cp $WORKDIR/patch/cluster.py $MG/madgraph/various/
#echo 'cluster_type = sge' >> $MG/input/mg5_configuration.txt
#echo 'cluster_size = 96' >> $MG/input/mg5_configuration.txt

## First NLO generation just to compile packages (CutTools, IREGI, etc)
cd $MG
source setup.shrc
bin/mg5_aMC <<EOF
generate p p > t t~ [QCD]
output _to_be_removed_
EOF
rm -rf _to_be_removed_

echo "@@@@@@@@@@ DONE @@@@@@@@@@"
