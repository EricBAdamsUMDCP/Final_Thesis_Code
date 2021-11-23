##bin/bash!

echo "Enter program number to launch..."

read programnum

ListHF=("PlotterForEric" "Centering_PlotterForEric" "HFV1AngularCorrections_W_Centering" "HFV1EPPlotting_w_Centering_adds_tracks")
ListRPD=("RPD_PlotterForEric" "RPD_Centering_PlotterForEric" "RPD_HFV1AngularCorrections_W_Centering" "RPD_HFV1EPPlotting_w_Centering_adds_tracks")



for i in 0 1 #2 3
do 

    #for j in 0 1 2

   # do

        python3  /home/ebadams/Final_Thesis_Code/HF_RXN_Plane_Method/Condor_Program_Launcher_2Var.py ${ListHF[$programnum]} $i 0001

        python3  /home/ebadams/Final_Thesis_Code/SRPD_RXN_Plane_Method/Condor_Program_Launcher_2Var.py ${ListRPD[$programnum]} $i 0001


    #done

done
