##bin/bash!


for i in 0 # 1 2 3
do 

    for j in 1 # 1 2

    do

        python3  Condor_Program_Launcher_SPM.py Three_Sub_Event_Scalar_Product_Method_V1_Only 7 16 1 $i $j 0 0001

        python3  Condor_Program_Launcher_SPM.py Three_Sub_Event_Scalar_Product_Method_V1_Only 7 16 0 $i $j 0 0001

        #python3  Condor_Program_Launcher_SPM.py Three_Sub_Event_Scalar_Product_Method_V1_Only_540 7 16 1 0 0 1 0001
#
        #python3  Condor_Program_Launcher_SPM.py Three_Sub_Event_Scalar_Product_Method_V1_Only_1060 7 16 1 0 0 1 0001
#
        #python3  Condor_Program_Launcher_SPM.py Three_Sub_Event_Scalar_Product_Method_V1_Only_580 7 16 1 0 0 1 0001
#
        #python3  Condor_Program_Launcher_SPM.py Three_Sub_Event_Scalar_Product_Method_V1_Only_540 7 16 0 0 0 1 0001
#
        #python3  Condor_Program_Launcher_SPM.py Three_Sub_Event_Scalar_Product_Method_V1_Only_1060 7 16 0 0 0 1 0001
#
        #python3  Condor_Program_Launcher_SPM.py Three_Sub_Event_Scalar_Product_Method_V1_Only_580 7 16 0 0 0 1 0001

        ##turn back on RPD when needed

    done

done
