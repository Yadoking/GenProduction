## Common configuration for the HYU MC generator studies
This repository is to collect scripts, cards and macros for the generator level studies
for the particle physics group undergraduate student programs at the Hanyang University.

Software instruction is written and tested at a Scientific Linux 6 (SLC6) based Rocks cluster
system, with devtools-4 packages.

To do:
  * Tune the Delphes cards for the updated CMS simulation
  * Test CMS simulation with pileup
  * Give more details on the analysis ntuple

### Install packages
Install aMC@NLO generator and Delphes Fast simulator.
This installation script applies some modifications to run on the EPP-HYU cluster:
  * Properly build packages with the correct order
  * Build OneLoop package with the gfortran fix
  * Rebuild base libraries to run the Pythia8 generator
  * Install PDFSet (NNPDF23 for the moment)
  * Prepare a envvar setup script (setup.shrc)

```
git clone https://github.com/Yadoking/GenProduction
cd sw
## Install Madgraph_aMC@NLO
./install_madgraph.sh
## Install Delphes
./install_delphes.sh
cd ..
```

### Produce MC samples
Produce MC samles using the aMC@NLO generator.

For an example, below is to generate ttbar+jets at the leading order.
```
cd sw/MG5_aMC_v2_6_0
source setup.shrc
bin/mg5_aMC
> generate p p > t t~, (t > w+ b, w+ > l+ vl), (t~ > w- b~, w- > l- vl~) @1
> add process p p > t t~, (t > w- b, w- > l- vl~), (t > w+ b, w+ > l+ vl) @2
> add process p p > t t~ j, (t > w+ b, w+ > l+ vl ), (t~ > w- b~, w- > l- vl~) @3
> add process p p > t t~ j, (t > w- b, w- > l- vl~), (t  > w+ b , w+ > l+ vl ) @4
> add process p p > t t~ j j, (t > w+ b, w+ > l+ vl ), (t~ > w- b~, w- > l- vl~) @5
> add process p p > t t~ j j, (t > w- b, w- > l- vl~), (t  > w+ b , w+ > l+ vl ) @6
> output TTJets_MG
> quit

cd TTJets_MG
bin/generate_events
```

Speed up the generation process with the multiprocessing or cluster option.
```
bin/generate_events --multicore
```
or
```
bin/generate_events --cluster
```

### Fast simulation with Delphes
Perform the detector simulation using the Delphes Fast Simulator.
```
cd 
./DelphesHepMC cards/delphes_card_CMS.tcl OUTPUT.root INPUT1.hepmc INPUT2.hepmc ...
```

### Produce analysis ntuples
A flat ntuple format is more suitable for the physics analysis. The macro we wrote produces
a slimmed ntuple without dependency to the Delphes library.

```
cd Delphes2Flat
./run.py INPUT1.root INPUT2.root ... output_prefix
```

will produce root files with prefix 'output_prefix'
