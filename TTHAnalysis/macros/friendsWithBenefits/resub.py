for i in [
'evVarFriend_TTHnobb_pow.chunk9.root',  
#'evVarFriend_TTJets.chunk65.root',  # not present
#'evVarFriend_TTJets.chunk66.root',  # not present
#'evVarFriend_TTJets.chunk67.root',  # not present
#'evVarFriend_TTJets.chunk68.root',  # not present

]:

    i = i.replace('evVarFriend_TTHnobb_pow.chunk','').replace('.root','')
    kk = kk = """echo "/mnt_pool/ciencias_users/user/sscruz/Boost/CMSSW_8_0_28/src/CMGTools/TTHAnalysis/macros/lxbatch_runner.sh /mnt_pool/ciencias_users/user/sscruz/Boost/CMSSW_8_0_28/src/CMGTools/TTHAnalysis/macros /mnt_pool/ciencias_users/user/sscruz/Boost/CMSSW_8_0_28 python prepareEventVariablesFriendTree.py -N 250000 -T sf -t treeProducerSusyMultilepton /pool/ciencias/userstorage/sscruz/Boost/ProdMCJan23_v2/merged /pool/cienciasrw/userstorage/sscruz/Boost/ProdMCJan23_v2/merged/rcl/eventBTagWeight --vector  --tra2  -d TTHnobb_pow -c 3   -F  sf/t /pool/ciencias/userstorage/sscruz/Boost/ProdMCJan23_v2/merged/rcl//leptonJetFastReCleanerTTH_step1leptonJetFastReCleanerTTH_step2_mc/evVarFriend_TTHnobb_pow.root   -m  'eventBTagWeight'    -I  'CMGTools.TTHAnalysis.tools.functionsTTH'  " | qsub -q batch -N happyTreeFriend -o /pool/ciencias/userstorage/sscruz/Boost/ProdMCJan23_v2/merged/rcl/eventBTagWeight/log/TTHnobb_pow_13.out -e /pool/ciencias/userstorage/sscruz/Boost/ProdMCJan23_v2/merged/rcl/eventBTagWeight/log/TTHnobb_pow_13.err"""


    print kk.replace('-c 3','-c ' + i)
