# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: Erics_Private_MC_Hydjet/AMPT/python/Eric_AMPT_PPb_5020GeV_MinimumBias_cfi.py --mc --eventcontent RAWSIM --no_exec --datatier GEN-SIM --conditions 103X_upgrade2018_realistic_HI_v11 --beamspot RealisticPbPbCollision2018 --step GEN,SIM --nThreads 4 --scenario HeavyIons --geometry DB:Extended --era Run2_2018_pp_on_AA
import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras

process = cms.Process('SIM',eras.Run2_2018_pp_on_AA)

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContentHeavyIons_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.GeometrySimDB_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.Generator_cff')
process.load('IOMC.EventVertexGenerators.VtxSmearedRealisticPbPbCollision2018_cfi')
process.load('Configuration.StandardSequences.GeneratorHI_cff')
process.load('GeneratorInterface.Core.genFilterSummary_cff')
process.load('Configuration.StandardSequences.SimIdeal_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1)
)

# Input source
process.source = cms.Source("EmptySource")

process.options = cms.untracked.PSet(

)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('AMPT PPb 5020 GeV Minimum Bias'),
    name = cms.untracked.string('$Source$'),
    version = cms.untracked.string('$Revision$')
)

# Output definition

process.RAWSIMoutput = cms.OutputModule("PoolOutputModule",
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('generation_step')
    ),
    compressionAlgorithm = cms.untracked.string('LZMA'),
    compressionLevel = cms.untracked.int32(1),
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('GEN-SIM'),
        filterName = cms.untracked.string('')
    ),
    eventAutoFlushCompressedSize = cms.untracked.int32(20971520),
    fileName = cms.untracked.string('PMC_NO_FLOW_Final_Erics_MC_PbPb2018_.root'),
    outputCommands = process.RAWSIMEventContent.outputCommands,
    splitLevel = cms.untracked.int32(0)
)

# Additional output definition

# Other statements
process.XMLFromDBSource.label = cms.string("Extended")
process.genstepfilter.triggerConditions=cms.vstring("generation_step")
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '103X_upgrade2018_realistic_HI_v11', '')

process.generator = cms.EDFilter("AMPTGeneratorFilter",
    alpha = cms.double(0.47140452),
    amptmode = cms.int32(1),
    bMax = cms.double(15),
    bMin = cms.double(0),
    comEnergy = cms.double(5020.0),
    deuteronfactor = cms.int32(5),
    deuteronmode = cms.int32(0),
    deuteronxsec = cms.int32(1),
    diquarkembedding = cms.int32(0),
    diquarkpx = cms.double(7.0),
    diquarkpy = cms.double(0.0),
    diquarkx = cms.double(0.0),
    diquarky = cms.double(0.0),
    doInitialAndFinalRadiation = cms.int32(3),
    dpcoal = cms.double(1000000.0),
    drcoal = cms.double(1000000.0),
    dt = cms.double(0.2),
    firstEvent = cms.untracked.uint32(1),
    firstRun = cms.untracked.uint32(1),
    frame = cms.string('CMS'),
    iap = cms.int32(208),
    iat = cms.int32(1),
    IHPR2 = cms.int32(5), #added in to turn off soft scattering
    izp = cms.int32(82),
    izpc = cms.int32(100), #was 0 now set to 100 to turn off flow
    izt = cms.int32(1),
    ks0decay = cms.bool(False),
    ktkick = cms.int32(1),
    maxmiss = cms.int32(1000),
    minijetpt = cms.double(-7.0),
    mu = cms.double(3.2264),
    ntmax = cms.int32(1000),
    phidecay = cms.bool(True),
    popcornmode = cms.bool(True),
    popcornpar = cms.double(1.0),
    proj = cms.string('A'),
    pthard = cms.double(2.0),
    quenchingmode = cms.bool(False),
    quenchingpar = cms.double(2.0),
    rotateEventPlane = cms.bool(True),
    shadowingmode = cms.bool(True),
    stringFragA = cms.double(0.5),
    stringFragB = cms.double(0.9),
    targ = cms.string('P')
)


# Path and EndPath definitions
process.generation_step = cms.Path(process.pgen)
process.simulation_step = cms.Path(process.psim)
process.genfiltersummary_step = cms.EndPath(process.genFilterSummary)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.RAWSIMoutput_step = cms.EndPath(process.RAWSIMoutput)

# Schedule definition
process.schedule = cms.Schedule(process.generation_step,process.genfiltersummary_step,process.simulation_step,process.endjob_step,process.RAWSIMoutput_step)
from PhysicsTools.PatAlgos.tools.helpers import associatePatAlgosToolsTask
associatePatAlgosToolsTask(process)

#Setup FWK for multithreaded
process.options.numberOfThreads=cms.untracked.uint32(4)
process.options.numberOfStreams=cms.untracked.uint32(0)
# filter all path with the production filter sequence
for path in process.paths:
	getattr(process,path)._seq = process.generator * getattr(process,path)._seq 


# Customisation from command line

# Add early deletion of temporary data products to reduce peak memory need
from Configuration.StandardSequences.earlyDeleteSettings_cff import customiseEarlyDelete
process = customiseEarlyDelete(process)
# End adding early deletion
