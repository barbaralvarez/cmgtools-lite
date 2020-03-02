import os, sys
directory = sys.argv[1]
files = os.listdir(directory)
for fil in files:
    if not 'evVarFriend_' in fil: continue
    if 'chunk' in fil: continue
    dataset = fil.replace('evVarFriend_','').replace('.root','')
    os.system('python verifyFTree.py ' + directory + '../ ' + directory + ' '+dataset)
