#!/bin/bash
cd /cvmfs/cms.cern.ch/slc7_amd64_gcc700/cms/cmssw/CMSSW_10_3_3/src ## your preferred CMSSW version
eval `scramv1 runtime -sh`
cd -
cd ${_CONDOR_SCRATCH_DIR}
root -l -b n "/home/ebadams/Final_Thesis_Code/SRPD_RXN_Plane_Method/RPD_PreV1_TProfile2D.C+(326776, 7, 16, 0, 2, 0)"