#!/usr/bin/env python

import sys, os
if len(sys.argv) < 3:
    print "%s INPUT.root OUTPUT.root" % sys.argv[0]
    sys.exit(1)
inFile = sys.argv[1]
outFile = sys.argv[2]

from ROOT import *
delphesPath = "Delphes"
gSystem.AddIncludePath('-I"%s"' % delphesPath)
gSystem.AddDynamicPath(delphesPath)
gSystem.AddLinkedLibs('-L"%s"' % delphesPath)
gSystem.Load("libDelphes")

gROOT.ProcessLine(".L makeFlatTuple.C++")
gROOT.ProcessLine('makeFlatTuple("%s", "%s");' % (inFile, outFile))

