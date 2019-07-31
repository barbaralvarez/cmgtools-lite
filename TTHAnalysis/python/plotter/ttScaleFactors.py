#!/usr/bin/env python
import os
muvars  = ['muon_eta', 'muon_pt']
elevars = ['ele_eta',  'ele_pt']

#loop over variables
#muon probe
for variable in muvars:
    for nbin in range(1,11):
        #print ">>>> Variable: ",variable, " and bin number = ",nbin
        #cmd = "root -l -b -q \'ttScaleFactors.C+(\"2017\", \"eTmuL\", \"eTmuT\", \""+variable+"\", "+str(nbin)+")\'"
        cmdmu  = "root -l -b -q \'ttScaleFactors.C+(\"2017\", \"eTmuL\", \"eTmuT\", \""+variable+"\", "+str(nbin)+")\'"
        #print cmd
        os.system(cmdmu)
#electron probe
for variable in elevars:
    for nbin in range(1,11):
        #print ">>>> Variable: ",variable, " and bin number = ",nbin
        #cmd = "root -l -b -q \'ttScaleFactors.C+(\"2017\", \"eTmuL\", \"eTmuT\", \""+variable+"\", "+str(nbin)+")\'"
        cmdele = "root -l -b -q \'ttScaleFactors.C+(\"2017\", \"muTeL\", \"muTeT\", \""+variable+"\", "+str(nbin)+")\'"
        #print cmd
        os.system(cmdele)
