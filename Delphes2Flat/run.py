#!/usr/bin/env python

import sys
if len(sys.argv) < 3:
    print "%s INPUT1.root INPUT2.root INPUT2.root ... OUTPUT_PREFIX" % sys.argv[0]
    sys.exit(1)
inputFiles = sys.argv[1:-1]
prefix = sys.argv[-1]

from ROOT import *
delphesPath = "Delphes"
gSystem.AddIncludePath('-I"%s"' % delphesPath)
gSystem.AddDynamicPath(delphesPath)
gSystem.AddLinkedLibs('-L"%s"' % delphesPath)
gSystem.Load("libDelphes")

gROOT.ProcessLine(".L makeFlatTuple.C++")

for inFile in inputFiles:
    outFile = "%s%s" % (prefix, os.path.basename(inFile))
    gROOT.ProcessLine('makeFlatTuple("%s", "%s");' % (inFile, outFile))

