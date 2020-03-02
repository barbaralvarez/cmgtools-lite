import ROOT as r
import os

treeDir = '/pool/ciencias/userstorage/sscruz/HeppyTrees/2018/TREES_TTH_120218_Fall17_JECV4/'
frieDir = '/pool/ciencias/userstorage/sscruz/HeppyTrees/2018/TREES_TTH_120218_Fall17_JECV4/1_recleaner_withTigth_apr6/'

lstMissing = []
lstUncompl = []

for fil in os.listdir(treeDir):
    tfile = r.TFile.Open(treeDir + '/' + fil + 'treeProducerSusyMultilepton/tree.root')
    if not tfile: continue

    ffile = r.TFile.Open(frieDir + '/evVarFriend_' + fil + '.root')
    if not ffile:
        lstMissing.append(fil)
        continue

    tree = tfile.Get('tree')
    frnd = ffile.Get('sf/t')

    if not tree: raise RuntimeError('File %s exist, but no tree there'%fil)
    if not frnd:
        lstMissing.append(fil)
        continue

    entries       = tree.GetEntries()
    friendentries = frnd.GetEntries()

    if (entries != friendentries):
        lstUncompl.append( (fil, entries, friendentries))

print 'Some friends are missing: ', lstMissing
print 'Some friends are uncomplete:'
for fl in lstUncompl:
    print '{fil} has {entries} entries but friend only has {friendentries}'.format(fil=fil, entries=entries,
                                                                                   friendentries=friendentries)
