#nanoaod 5TeV WZ
#2017 5 TeV Muon
nanopy_batch.py  run_TnP_Data.py -b 'run_condor_simple.sh -t 2880 ./batchScript.sh' --option flavor=Muon --option year=2017  -r /store/group/phys_muon/balvarez/TnP_WZ5TeV/Muon/Data/ 
#nanopy_batch.py  run_TnP_MC.py -b 'run_condor_simple.sh -t 2880 ./batchScript.sh' --option flavor=Muon --option year=2017  -r /store/group/phys_muon/balvarez/TnP_WZ5TeV/Muon/MC/ 

#2017 5 TeV Electron
#nanopy_batch.py  run_TnP_Data.py -b 'run_condor_simple.sh -t 2880 ./batchScript.sh' --option flavor=Electron --option year=2017  -r /store/group/phys_muon/balvarez/TnP_WZ5TeV/Electron/Data/ 
#nanopy_batch.py  run_TnP_MC.py -b 'run_condor_simple.sh -t 2880 ./batchScript.sh' --option flavor=Electron --option year=2017  -r /store/group/phys_muon/balvarez/TnP_WZ5TeV/Electron/MC/ 


