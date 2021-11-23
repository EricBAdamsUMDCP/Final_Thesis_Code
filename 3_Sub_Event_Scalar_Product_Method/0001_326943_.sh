#!/bin/bash
cd /cvmfs/cms.cern.ch/slc7_amd64_gcc700/cms/cmssw/CMSSW_10_3_3/src ## your preferred CMSSW version
eval `scramv1 runtime -sh`
cd -
cd ${_CONDOR_SCRATCH_DIR}
root -l -b n "/home/ebadams/Final_Thesis_Code/3_Sub_Event_Scalar_Product_Method/Three_Sub_Event_Scalar_Product_Method_V1_Only_580.C+(7, 16, 0, 0, 0, 326943, 1)"