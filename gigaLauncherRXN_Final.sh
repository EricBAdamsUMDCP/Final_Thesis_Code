##bin/bash!

echo "Enter program number to launch..."

read programnum


ListHF=("PreV1_TProfile2D" "PreV1_TProfile2D1060" "PreV1_TProfile2D540" "PreV1_TProfile2D580")
ListRPD=("RPD_PreV1_TProfile2D" "RPD_PreV1_TProfile2D1060" "RPD_PreV1_TProfile2D540" "RPD_PreV1_TProfile2D580")


for i in  0 # 1 3
do 

    for j in 0 1 2

    do

        python3  /home/ebadams/Final_Thesis_Code/HF_RXN_Plane_Method/Condor_Program_Launcher_FinalVar.py ${ListHF[$programnum]} $i $j 0001

        python3  /home/ebadams/Final_Thesis_Code/SRPD_RXN_Plane_Method/Condor_Program_Launcher_FinalVar.py ${ListRPD[$programnum]} $i $j 0001


    done

done
