from optparse import OptionParser
from  subprocess import Popen, PIPE
import time
import os
import sys 

defaultModules = [
    ["leptonJetFastReCleanerTTH_step1,leptonJetFastReCleanerTTH_step2_data",""]
    ]

# this can be changed with option -I, --import
defaultImport = ['CMGTools.TTHAnalysis.tools.functionsTTH']

# this can be changed with -a, --accept
defaultAccept = []

# can be changed with -e, --exclude
defaultExclude = []

# these can be changed with --Pa, --Ou, --FT
defaultPath = "/pool/ciencias/HeppyTrees/RA7/estructura/treesM17/"
defaultOutpath = "/pool/ciencias/HeppyTrees/ttH/fts/"
defaultFTpath = "/pool/ciencias/userstorage/sscruz/Boost/ProdMCJan23_v2/merged/rcl/"


class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'

def cmd(command):
    # env={'PYTHONPATH': os.pathsep.join(sys.path)}
    result = Popen( command, stdout=PIPE, stderr=PIPE)
    out, err = result.communicate()
    return out,err

def mkdir(outpath):
	if os.path.isdir(outpath): return
	cmd("mkdir " + outpath)


class SubmitAndCheck:
    def __init__(self, options):
        self.options = options
        self.commandOut = []
    def submit(self):
        ''' Integrated version of Nachos submit.py''' 
        modules = defaultModules
        path    = self.options.path    if len(self.options.path) > 0 else defaultPath
        accept  = self.options.accept  if len(self.options.accept) > 0 else defaultAccept
        exclude = self.options.exclude if len(self.options.exclude) > 0 else defaultExclude
        outpath = self.options.outpath if len(self.options.outpath) > 0 else defaultOutpath
        path    = path.rstrip('/')
        listdir = os.listdir(path)
        subPath = ''
        for module in modules:
            for mod in module[0].split(','):
		subPath = subPath + mod
            mkdir(outpath + "/" + subPath)
            for d in listdir:
		if not os.path.isdir(path + "/" + d): continue
		if not ( os.path.exists(path + "/" + d + "/treeProducerSusyMultilepton/tree.root") or os.path.exists(path + "/" + d + "/treeProducerSusyMultilepton/tree.root.url") ): continue
                if accept  != [] and all([d.find(a) == -1 for a in accept ]): continue
		if exclude != [] and any([d.find(e) >  -1 for e in exclude]): continue
		self.runIt(d, module, subPath)

        outName = time.strftime('submision_%b%d_%H_%M_%S.txt')
        print 'Saving output commands to', outName
        subFile = open(outName,'w')
        self.commandOut = '\n'.join(self.commandOut)
        subFile.write(self.commandOut)
        subFile.close()
        

    def runIt(self, sample, module, subPath):
        path    = self.options.path    if len(self.options.path) > 0 else defaultPath
        outpath = self.options.outpath if len(self.options.outpath) > 0 else defaultOutpath
        ftpath  = self.options.ftpath  if len(self.options.ftpath) > 0 else defaultFTpath
        Import  = self.options.Import  if len(self.options.Import) > 0 else defaultImport

        super = [path, outpath + "/" + subPath, '-d ' + sample, '--tra2',"--tree", "treeProducerSusyMultilepton" ]

        for I in Import:
            super.append('-I ' + I)
        for mod in module[0].split(','):
            super.append('-m ' + mod )
	if not module[1].strip() == "":
            sm = module[1].strip().split(",")
            for f in sm: super.append(" -F sf/t " + ftpath + "/" + f + "/evVarFriend_" + sample + ".root")

	if self.options.batch and self.options.queue == 'batch':
            super.extend(["--env", "oviedo"])
            super.append("-q " + self.options.queue)
            super.append("-N 250000")

	elif self.options.batch:
            super.append("-q " + self.options.queue)
            super.append("-N 50000")
	else:
	    super.append("-j 18")


        os.system(' '.join(['python', 'prepareEventVariablesFriendTree.py']+super) + ' > stdout')
        # i tried to do it with popen, but miserably failed :(
        #out, err = cmd(['python', 'prepareEventVariablesFriendTree.py']+super)
        stdout = open('stdout')
        for i in stdout.readlines():
            if not 'prepareEventVariablesFriendTree.py' in i: continue
            self.commandOut.append(i)
        os.system('rm stdout')


    def check(self):
        outpath = options.outpath if len(options.outpath) > 0 else defaultOutpath
        out, err = cmd(['/bin/bash','-c','${PWD}/chunkDealer.sh %s check '%outpath])
        print '${PWD}/chunkDealer.sh %s check '%outpath
        if err != '':
            print bcolors.FAIL+'Some error/warning happend when running chuck dealer'+bcolors.ENDC
            print bcolors.FAIL+err+bcolors.ENDC


        notPresent = []

        for lin in out.splitlines():
            if '# not present' in lin:
                notPresent.append( [lin.split('evVarFriend_')[1].split('.chunk')[0], lin.split('.chunk')[1].split('.root')[0]])

        if len(notPresent) == 0:
            print bcolors.OKGREEN+'Everything seems to be in place'+bcolors.ENDC
            print bcolors.OKGREEN+'Lets proceed with the merging'+bcolors.ENDC
            print '${PWD}/chunkDealer.sh %s merge '%outpath
            out, err = cmd(['/bin/bash','-c','${PWD}/chunkDealer.sh %s merge '%outpath])

        else:
            print bcolors.FAIL + '[W] Apparently some chunks (%d) failed and are not present. Checking in submision file to resubmit'%len(notPresent) + bcolors.ENDC
            torun = []
            sub = open(options.submitFile,'r').readlines()
            for notP in notPresent:
                solved=False
                for lin in sub:
                    if  notP[0] not in lin        : continue
                    if '-c %s'%notP[1] not in lin: continue
                    solved=True
                    print bcolors.BOLD + "Running..." + bcolors.ENDC
                    os.system(lin)
                    break
                if not solved:
                    print bcolors.FAIL + 'Command for resubmitting sample', '"'+notP[0]+'"','and chunk',notP[1],'not found'  + bcolors.ENDC
                    
if __name__ == "__main__":

    parser = OptionParser()
    parser.add_option("-s", "--submitFile",
                      dest="submitFile", default='',
                      help="File with the submision commands as produced by submit.py")
    parser.add_option("-I", "--import",
                      help="Libraries to be imported in the FT production",
                      dest="Import", default=[], action='append')
    parser.add_option('-a','--accept',
                      help="Samples to be processed",
                      dest="accept", default=[], action='append')
    parser.add_option('-e','--exclude',
                      help="Samples to be excluded",
                      dest="exclude",default=[], action='append')
    parser.add_option('','--Pa',
                      help="Input path", default='',type='string',dest="path")
    parser.add_option('','--Ou',
                      help="Output path", default='',type='string',dest="outpath")
    parser.add_option('','--FT',
                      help="Friend tree input path", default='',type='string',dest="ftpath")
    parser.add_option('-b','--batch',
                      help='Do it in batch system?', default=True, action='store_false')
    parser.add_option('-q','--queue',
                      help="Queue (batch for Oviedo, 8nh or others in lxplus)",
                      default="batch", type='string')

    (options, args) = parser.parse_args()
    
    print bcolors.HEADER + '##########################################' + bcolors.ENDC
    print bcolors.HEADER + 'Welcome to dealWithChunksAndResubmit.py'    + bcolors.ENDC
    print bcolors.HEADER + 'Author: Sergio Sanchez Cruz'    + bcolors.ENDC
    print bcolors.HEADER + '##########################################' + bcolors.ENDC

    core = SubmitAndCheck(options)
                
    if len(options.submitFile) > 0: 
        print bcolors.HEADER + "Submitfile detected in input, checking if everything is correct" + bcolors.ENDC
        core.check()
    else:
        print bcolors.HEADER + "No Submitfile detected in input. Launching production" + bcolors.ENDC
        core.submit()
