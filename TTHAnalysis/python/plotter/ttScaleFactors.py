#!/usr/bin/env python
import os
vars = ['muon_eta', 'muon_pt']

#loop over variables
for variable in vars:
    for nbin in range(1,11):
        print ">>>> Variable: ",variable, " and bin number = ",nbin
        #cmd = "root -l -b -q \'ttScaleFactors.C+(\"2017\", \"eTmuL\", \"eTmuT\", \""+variable+"\", "+str(nbin)+")\'"
        cmd = "root -l -b -q \'ttScaleFactors.C+(\"2017\", \"eTmuL\", \"eTmuT\", \""+variable+"\", "+str(nbin)+")\'"
        print cmd
        os.system(cmd)
