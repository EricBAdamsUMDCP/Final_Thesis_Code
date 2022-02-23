import os
import time
import sys

#read in all the rootfile names and store them in an array
content_array = [326776, 326822, 326883, 326943, 327078, 327211, 327327, 327464, 327524, 327560] #this is outside so the size of the array can be read later

nameofprogram = sys.argv[1]

nameofsystematics = sys.argv[2]

nameofvertex = sys.argv[3]

nameofshell = sys.argv[4]

for RunNumber in range(len(content_array)): #generates the number of .sh necessary for all the given root files
	nameofshellscript = nameofshell + "_" + nameofprogram + "_" + str(content_array[RunNumber]) + "_" + ".sh"
	pythonfileholder = ('root -l -b n ' + '"/home/ebadams/Final_Thesis_Code/HF_RXN_Plane_Method/' + str(nameofprogram) + ".C+(" + str(content_array[RunNumber]) + ", 7, 16, " + str(nameofsystematics) + ", " + str(nameofvertex) + ', 0)"')
	file_object = open(nameofshellscript, "w")
	file_object.writelines([
		"#!/bin/bash", "\n",
		"cd /cvmfs/cms.cern.ch/slc7_amd64_gcc700/cms/cmssw/CMSSW_10_3_3/src ## your preferred CMSSW version","\n",
		"eval `scramv1 runtime -sh`","\n",
		"cd -","\n",
		"cd ${_CONDOR_SCRATCH_DIR}", "\n",
		str(pythonfileholder)
	])
	file_object.close()
	give_execute_permission = "chmod u+x " + nameofshellscript
	os.system(give_execute_permission)
	print('Made .sh')


nameofjdlscript =  nameofshell + ".jdl"
jdl_object = open(nameofjdlscript, "w")
#jdl_transfer_input_files = "/home/ebadams/PbPb2018_CMS_DATA_MB2/Trimming_Software/ZDCtrimmerParallelization.C, " + "/mnt/hadoop/cms/store/user/eadams/HIMinimumBias2/crab_rereco_PbPb2018_AOD_MinBias2_326943_ZDC/191023_220414/0000/" + "$(RunNumberName)" + ".root"

#generate the .jdl for submitting the .sh files
jdl_object.writelines([
	"universe = vanilla", "\n",
	"Executable = " + nameofshell + "_"  + nameofprogram + "_" + "$(RunNumberName)" + "_" + ".sh", "\n",
	"getenv = True", "\n",
	"Should_Transfer_Files = YES", "\n",
	'Requirements = TARGET.FileSystemDomain == "privnet"',"\n", 
	"Error = /home/ebadams/CMSSW_10_3_2/src/RawtoDigi/Errors/condor_pythonstep1_$(Cluster)_$(Process).stderr", "\n",
	"Log = /home/ebadams/CMSSW_10_3_2/src/RawtoDigi/Log/condor_ROOT_MACRO_TEST_$(Cluster)_$(Process).log", "\n",
	"should_transfer_files = YES", "\n",
	"when_to_transfer_output = ON_EXIT", "\n",
	"notification = never", "\n",
	"Queue " + "RunNumberName" + " in " + "(326776, 326822, 326883, 326943, 327078, 327211, 327327, 327464, 327524, 327560)"
])
jdl_object.close()
print("made .jdl")


submit_command = "condor_submit " + nameofjdlscript
commandcheck = "condor_q"
os.system(submit_command)

time.sleep(1)
os.system(commandcheck)
time.sleep(5)
os.system(commandcheck)
time.sleep(10)
os.system(commandcheck)