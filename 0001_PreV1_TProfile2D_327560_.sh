#!/bin/bash
cd /cvmfs/cms.cern.ch/slc7_amd64_gcc700/cms/cmssw/CMSSW_10_3_3/src ## your preferred CMSSW version
eval `scramv1 runtime -sh`
cd -
cd ${_CONDOR_SCRATCH_DIR}
root -l -b n "/home/ebadams/Final_Thesis_Code/HF_RXN_Plane_Method/PreV1_TProfile2D.C+(327560, 7, 16, 2, 2, 1)"