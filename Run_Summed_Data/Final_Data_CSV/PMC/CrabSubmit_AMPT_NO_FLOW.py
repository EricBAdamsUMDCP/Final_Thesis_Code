import FWCore.ParameterSet.Config as cms
from CRABClient.UserUtilities import config
#note json file must be in same folder at config file

from Configuration.StandardSequences.Eras import eras

process = cms.Process("Demo",eras.Run2_2018_pp_on_AA)

config = config()
config.General.requestName = 'AMPT_wo_FLOW'
config.General.transferLogs = True
config.General.transferOutputs = True
config.section_('JobType')
config.JobType.outputFiles = ['PMC_NO_FLOW_Final_Erics_MC_PbPb2018_.root']
config.JobType.pyCfgParams = ['noprint']
config.JobType.pluginName = 'PrivateMC'
config.JobType.psetName = 'Eric_AMPT_PbPb_5020GeV_MinimumBias_cfi_py_GEN_SIM.py'
config.JobType.numCores = 4
config.Data.outputPrimaryDataset = 'MinBias'
config.Data.splitting = 'EventBased'
config.Data.unitsPerJob = 20
NJOBS = 100  # This is not a configuration parameter, but an auxiliary variable that we use in the next line.
config.Data.totalUnits = config.Data.unitsPerJob * NJOBS
config.Data.publication = True
config.Data.outputDatasetTag = 'AMPT_wo_FLOW'

config.Data.outLFNDirBase = '/store/user/eadams' #I believe this is correct
#config.Data.runRange = '326776'
config.Site.storageSite = 'T3_US_UMD'