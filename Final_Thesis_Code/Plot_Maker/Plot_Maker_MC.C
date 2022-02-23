/*take weightedf average of error and value weighted by number of events in the run

and be abkle to make the plots


want to take an input from all run numbers and by centrality


so all run # input then all cent and go with that
and by type even odd

so i want to make a code that takes input or does all at once
no inpiut makes more sense


4 type input rxn hf rxn rpd spm hf spm rpd

and eve n and odd done automatically

gets bin content and bin error

has different modes can make plots so use classes i thin kand used storeed plots change how they look andfunction
*/
//Plot Maker 
#include <map>
#include <cstring>
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include "TFile.h"
#include "TProfile.h"
#include "TH1.h"
#include "TH2.h"
#include "TProfile2D.h"
#include "TMath.h"
#include <math.h> 
#include "TCanvas.h"
#include "TStyle.h"
using namespace std;

int runnumber[10] = {1, 326822, 326883, 326943, 327078, 327211, 327327, 327464, 327524, 327560};
int num_events[10] = {7003688, 554328, 1256669, 2915939, 4313507, 5989364, 7381724, 2615391, 3584837, 2969136}; //in runnumber order

void Plot_Maker_MC( int numSPM_or_RXN, int Centrality_Bin, bool Special_Cuts){

//due to a bug in root software you cannot delete file handles in this format. Use the centraility bin number from 0 to 9 to call each centrality its pretty fast. You should only need to do this once.
	string SPM_or_RXN = "nothing";

	if (numSPM_or_RXN == 0){
		SPM_or_RXN = "RXN";
	}
	else{
		SPM_or_RXN = "SPM";
	}
	string centralitystring[10];// used for both ttypes of cuts

	string centralitystring_OG[10] = {"0-10%", "10-20%", "20-30%", "30-40%", "40-50%", "50-60%", "60-70%", "70-80%", "80-90%", "90-100%"};
	string centralitystring_Special_Cuts[3] = {"10-60_10-20%", "5-40_10-20%", "5-80_10-20%"}; //used a code technicality to store the centrality data for non standard centrality units in the 10-20% bin
	string Special[2] = {"Not_Special", "Special"};
	int Special_Num = 0;

	if (Special_Cuts == true){
		Special_Num = 1;
	}


	if (Special_Cuts == true){
		for (int i = 0; i < 3; i++){
			centralitystring[i] = centralitystring_Special_Cuts[i];
		}
	}
	if (Special_Cuts != true){
		for (int i = 0; i < 10; i++){
			centralitystring[i] = centralitystring_OG[i];
		}
	}
	
	TCanvas* D = new TCanvas("D", "D", 3000, 3000);//it is expected to launch a window at this point DO NOT CLOSE IT >>THE PROGRAM WILL CRASH<<
	gStyle->SetOptStat(0);
	D->SetWindowSize(3000 + (3000 - D->GetWw()), 3000 + (3000 - D->GetWh()));// force sets the window to the right size

	
	
	string Systematics[4] = {"Normal", "LOOSE", "NOMINAL", "TIGHT"};
	
	string Verticies[3] = {"Normal", "WIDE", "NARROW"};
	
	//FOR LOOP TO SHORTEN LINE LENGTH SO I CAN INPUT WHICH CENT AND RUN Number
	//this is utterly gargantuan and may be dififcult to read, this was designed this way for ease of use for the less code inclined.
	//keep in mind all this code does is the same thing but like 40 times
	int CentMax = 10;
	int SysMax = 1;
	int VertMax = 1;

	//create storage plots
	TH1D* HF_Even_Storage [10][4][3];
	TH1D* HF_Odd_Storage  [10][4][3];
	TH1D* RPD_Even_Storage[10][4][3];
	TH1D* RPD_Odd_Storage [10][4][3];

	TH1D* HF_Even_Storage_Special_Cuts [3][4][3];
	TH1D* HF_Odd_Storage_Special_Cuts  [3][4][3];
	TH1D* RPD_Even_Storage_Special_Cuts[3][4][3];
	TH1D* RPD_Odd_Storage_Special_Cuts [3][4][3];

	double Odd_Data_Storage[4][4][3][7] = {0}; // data storage so it can later be printed to a text file
	double Even_Data_Storage[4][4][3][16] = {0};

	if(Special_Cuts == true){
		CentMax = 3;
		SysMax = 1;
		VertMax = 1;
	}


	std::ostringstream OddOut;
	std::ostringstream EvenOut;
	//this section allows us to output data. Its compilcated because ofstream is being obnoxious

	OddOut << "/home/ebadams/Final_Thesis_Code/Run_Summed_Data/Final_Data_CSV/MC_EricsFinalThesisData_ODD_" << centralitystring_OG[Centrality_Bin].c_str() << "_" << Special[Special_Num].c_str() << "_.csv";
	std::string OddOutStr = OddOut.str();
	const char* OddOutCStr = OddOutStr.c_str();

	EvenOut << "/home/ebadams/Final_Thesis_Code/Run_Summed_Data/Final_Data_CSV/MC_EricsFinalThesisData_EVEN_" << centralitystring_OG[Centrality_Bin].c_str() << "_" << Special[Special_Num].c_str() << "_.csv";
	std::string EvenOutStr = EvenOut.str();
	const char* EvenOutCStr = EvenOutStr.c_str();
	// csv is used to store data for graphing software // https://www.geeksforgeeks.org/csv-file-management-using-c/

	std::ofstream ODD( OddOutCStr, ios::out | ios::app);	//either creates the file or appends (adds to it or writes over parts) https://stackoverflow.com/questions/51345024/read-text-file-and-parse-in-python
		
	ODD << " Detector [0 = SRPD 1 = HF], Technique [0 = RXN 1 = SPM],  Stored [0 = Data 1 = Errors], Systematic [0 = Normal 1 = LOOSE 2 = NOMINAL 3 = TIGHT], Vertex [0 = Normal 1 = WIDE 2 = NARROW], Bin 0, Bin 1, Bin 2, Bin 3, Bin 4, Bin 5, Bin 6, \n";

	std::ofstream EVEN( EvenOutCStr, ios::out | ios::app);	
		
	EVEN << " Detector [0 = SRPD 1 = HF], Technique [0 = RXN 1 = SPM], Stored [0 = Data 1 = Errors], Systematic [0 = Normal 1 = LOOSE 2 = NOMINAL 3 = TIGHT], Vertex [0 = Normal 1 = WIDE 2 = NARROW], Bin 0, Bin 1, Bin 2, Bin 3, Bin 4, Bin 5, Bin 6, Bin 7, Bin 8, Bin 9, Bin 10, Bin 11, Bin 12, Bin 13, Bin 14, Bin 15, \n";


	//The XX is used so searching can be done for specific lines with ease


	double Bin_Content_HF_Even_1[2][16] = {0}; double Bin_Content_HF_Odd_1[2][7] = {0}; //[0][0..15||6] are the histogram values


	TProfile* Get_HF_Even_1;	TProfile* Get_HF__Odd_1;

	
	double WA_HF_Even_value = 0;

	double WA_HF_Even_error = 0;


	double WA_HF_Odd_value = 0;

	double WA_HF_Odd_error = 0;


	string centrality_string_storage_value;
	int c = 0;
	c = Centrality_Bin;
	//for (int c = 0; c <CentMax; c++){ //cent bins
		for (int s = 0; s < SysMax; s++){//systematics
			for(int v = 0; v < VertMax; v++){//vertex
	
				if(Special_Cuts == false){
					//creating storage plots
					HF_Even_Storage[c][s][v]  = new TH1D(Form("%s_HF_Even_Sys_%s_Ver_%s_Cent_%s", SPM_or_RXN.c_str(), Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str()),  Form("%s_HF_Even_Sys_%s_Ver_%s_Cent_%s; NOT FOR THESIS USE STORAGE ONLY", SPM_or_RXN.c_str(), Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str()),  16, 0, 8);
					HF_Odd_Storage[c][s][v]   = new TH1D(Form("%s_HF_Odd_Sys_%s_Ver_%s_Cent_%s", SPM_or_RXN.c_str(), Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str()),   Form("%s_HF_Odd_Sys_%s_Ver_%s_Cent_%s; NOT FOR THESIS USE STORAGE ONLY", SPM_or_RXN.c_str(), Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str()),   7, -1.25, 1.25);
				}
				else if(Special_Cuts == true){
					//creating storage plots
					HF_Even_Storage_Special_Cuts[c][s][v]  = new TH1D(Form("%s_HF_Even_Sys_%s_Ver_%s_Cent_%s", SPM_or_RXN.c_str(), Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str()),  Form("%s_HF_Even_Sys_%s_Ver_%s_Cent_%s; NOT FOR THESIS USE STORAGE ONLY", SPM_or_RXN.c_str(), Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str()),  16, 0, 8);
					HF_Odd_Storage_Special_Cuts[c][s][v]   = new TH1D(Form("%s_HF_Odd_Sys_%s_Ver_%s_Cent_%s", SPM_or_RXN.c_str(), Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str()),   Form("%s_HF_Odd_Sys_%s_Ver_%s_Cent_%s; NOT FOR THESIS USE STORAGE ONLY", SPM_or_RXN.c_str(), Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str()),   7, -1.25, 1.25);
				}

				if (SPM_or_RXN == "RXN"){
				//(A) Part A draws in and loads the data files
					//RXN PLANE METHOD//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					//HF RXN Plane Method
					//Even
					TFile* HF_RXN_Even_1 = new TFile(Form("/data2/users/ebadams/Eric_HF_RXN_Data_Sets/1/ROOT/%s/%s/1_HF_V1_Even_Histo_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str())); //allocating mem for the TP

					//Odd
					TFile* HF_RXN_Odd_1 = new TFile(Form("/data2/users/ebadams/Eric_HF_RXN_Data_Sets/1/ROOT/%s/%s/1_HF_V1_Odd_Histo_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str())); //allocating mem for the TP



/*					RPD_RXN_Odd_326943->ls();
					RPD_RXN_Odd_326943->cd();
					RPD_RXN_Odd_326943->Draw();
					cin.ignore();*/



					if (Special_Cuts == false){
						centrality_string_storage_value = centralitystring[c].c_str();
					}
					else{
						centrality_string_storage_value = "10-20%;1";
					}

					//(B) Part B get the actual data from the loaded in plots using the "Get" command 10-20%
					Get_HF_Even_1 = (TProfile*) HF_RXN_Even_1 ->Get(Form("HF_V1_Even_%s", centrality_string_storage_value.c_str()));


					Get_HF__Odd_1 = (TProfile*) HF_RXN_Odd_1 ->Get(Form("HF_V1_Odd_%s", centrality_string_storage_value.c_str()));


				}
				else{ //if else not needed due to rejection return at begining			
					//SPM METHOD/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					//HF  SPM Method RunNumber_1_Sys_Normal_Vtx_Normal_HF_V1_Even_Histo_10-60_10_20
					//Even
					TFile* HF_SPM_Even_1 = new TFile(Form("/data2/users/ebadams/Eric_SP_Data_Sets/HF/1/ROOT/%s/%s/RunNumber_1_Sys_%s_Vtx_%s_HF_V1_Even_Histo_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str())); //allocating mem for the TP

					TFile* HF_SPM_Odd_1 = new TFile(Form("/data2/users/ebadams/Eric_SP_Data_Sets/HF/1/ROOT/%s/%s/RunNumber_1_Sys_%s_Vtx_%s_HF_V1_Odd_Histo_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str())); //allocating mem for the TP

					if (Special_Cuts == false){
						centrality_string_storage_value = centralitystring[c].c_str();
					}
					else{
						centrality_string_storage_value = "10-20%;1";
					}
					//(B) Part B get the actual data from the loaded in plots using the "Get" command
					Get_HF_Even_1  = (TProfile*) HF_SPM_Even_1 ->Get(Form("HF_V1_Even_%s", centrality_string_storage_value.c_str()));

		 
					Get_HF__Odd_1  = (TProfile*) HF_SPM_Odd_1 ->Get(Form("HF_V1_Odd_%s", centrality_string_storage_value.c_str()));


				}

				//(B.2) this part draws the data so it can be accessed and loaded.
				Get_HF_Even_1 ->Draw();

				Get_HF__Odd_1 ->Draw();

	
				//End of (B) 
	
	
				//(C) storing the data


				//WEIGHTED AVERAGE FOR ALL THE RUN VALUES ADDED AND SUMMED
				for (int bin = 0; bin < 7; bin++){

					Bin_Content_HF_Odd_1[0][bin]  = (Get_HF__Odd_1 ->GetBinContent(bin+1) >0.9) ? 0 : (Get_HF__Odd_1 ->GetBinContent(bin+1));
					cout << "Bin_Content_HF_Odd_1[0][bin] " << Bin_Content_HF_Odd_1[0][bin] << endl;

					Bin_Content_HF_Odd_1[1][bin]  = abs(Get_HF__Odd_1 ->GetBinError(bin+1));
					cout << "Bin_Content_HF_Odd_1[1][bin] " << Bin_Content_HF_Odd_1[1][bin] << endl;

					if ( bin == 0){
						if ( isnan(Bin_Content_HF_Odd_1[0][bin])){ cout << "NAN at HF_Odd_1 for Sys" << Systematics[s].c_str() << " and Vert " << Verticies[v].c_str() << endl;} 
					}


					WA_HF_Odd_value = ((Bin_Content_HF_Odd_1[0][bin])); //taking the weighted average to fill the final plots                                  

					WA_HF_Odd_error = ((Bin_Content_HF_Odd_1[1][bin]));//taking the weighted average to fill the final plots
                                  
					if(Special_Cuts == false){

						HF_Odd_Storage[c][s][v]->SetBinContent((bin+1), WA_HF_Odd_value);

						HF_Odd_Storage[c][s][v]->SetBinError((bin+1), WA_HF_Odd_error);

					}

					if(Special_Cuts == true){
						HF_Odd_Storage_Special_Cuts[c][s][v]->SetBinContent((bin+1),  WA_HF_Odd_value);
						HF_Odd_Storage_Special_Cuts[c][s][v]->SetBinError((bin+1),    WA_HF_Odd_error);

					}
					// Detector [0 = SRPD 1 = HF], Technique [0 = RXN 1 = SPM], Systematic [0 = Normal 1 = LOOSE 2 = NOMINAL 3 = TIGHT], Vertex [0 = Normal 1 = WIDE 2 = NARROW], Stored [0 = Data 1 = Errors], Bin 0, Bin 1, Bin 2, Bin 3, Bin 4, Bin 5, Bin 6, \n";
					Odd_Data_Storage[0][s][v][bin] = WA_HF_Odd_value;

					Odd_Data_Storage[2][s][v][bin] = WA_HF_Odd_error;

					if (isnan(WA_HF_Odd_value) or isnan(WA_HF_Odd_error)){
						cout << "NANANANANANANNAANNA" << endl;
					}

				}



				for (int bin = 0; bin < 16 ; bin++){

					Bin_Content_HF_Even_1[0][bin] = (Get_HF_Even_1 ->GetBinContent(bin+1) > 0.9) ? 0 : (Get_HF_Even_1 ->GetBinContent(bin+1));


					Bin_Content_HF_Even_1[1][bin] = abs(Get_HF_Even_1 ->GetBinError(bin+1));

					if ( bin == 0){
						if ( isnan(Bin_Content_HF_Even_1[0][bin])){ cout << "NAN at HF_Even_1 for Sys" << Systematics[s].c_str() << " and Vert " << Verticies[v].c_str() << endl;} 
}

					WA_HF_Even_value = 		((Bin_Content_HF_Even_1[0][bin]));                                
                              
					WA_HF_Even_error = 		((Bin_Content_HF_Even_1[1][bin]));

					//cout << "WA_HF_Even_value " << WA_HF_Even_value << endl;

					//cout << "WA_HF_Even_error " << WA_HF_Even_error << endl;

                                
					if (Special_Cuts == false){
						HF_Even_Storage[c][s][v]->SetBinContent((bin+1), WA_HF_Even_value);

						HF_Even_Storage[c][s][v]->SetBinError((bin+1), WA_HF_Even_error);

					}
					else if (Special_Cuts == true){
						HF_Even_Storage_Special_Cuts[c][s][v]->SetBinContent((bin+1),  WA_HF_Even_value);

						HF_Even_Storage_Special_Cuts[c][s][v]->SetBinError((bin+1),    WA_HF_Even_error);

					}

					//write code to save all the images may want to do inside loop? qand the n i need to make sure everything is present but i should be good then i can do more thesis edits!
					Even_Data_Storage[0][s][v][bin] = WA_HF_Even_value; // x
					Even_Data_Storage[2][s][v][bin] = WA_HF_Even_error;
					//cout << "Even_Data_Storage[2][s][v][bin] " << Even_Data_Storage[2][s][v][bin] << endl;
				}
			}
		}
	//}

	for (int x = 0; x < 4; x++){
		for (int s = 0; s < 1; s++){
			for (int v = 0; v < 1; v++){

				int Detector = 0;
				int Stored = 0;
				if ( x == 1 or x == 2) {Detector = 1;}
				if ( x > 1) {Stored = 1;}

				//ODD << "Detector [0 = SRPD 1 = HF], Technique [0 = RXN 1 = SPM], Stored [0 = Data 1 = Errors], Systematic [0 = Normal 1 = LOOSE 2 = NOMINAL 3 = TIGHT], Vertex [0 = Normal 1 = WIDE 2 = NARROW], Bin 0, Bin 1, Bin 2, Bin 3, Bin 4, Bin 5, Bin 6, \n";
				ODD << Detector << "," << numSPM_or_RXN << "," << Stored << "," << s << "," << v << "," << Odd_Data_Storage[x][s][v][0] << "," << Odd_Data_Storage[x][s][v][1] << "," <<  Odd_Data_Storage[x][s][v][2] << "," <<  Odd_Data_Storage[x][s][v][3] << "," <<  Odd_Data_Storage[x][s][v][4] << "," <<  Odd_Data_Storage[x][s][v][5] << "," <<  Odd_Data_Storage[x][s][v][6] << "\n";
				ODD << Detector << "," << numSPM_or_RXN << "," << 1 << "," << s << "," << v << "," << Odd_Data_Storage[2][s][v][0] << "," << Odd_Data_Storage[2][s][v][1] << "," <<  Odd_Data_Storage[2][s][v][2] << "," <<  Odd_Data_Storage[2][s][v][3] << "," <<  Odd_Data_Storage[2][s][v][4] << "," <<  Odd_Data_Storage[2][s][v][5] << "," <<  Odd_Data_Storage[2][s][v][6] << "\n";
	
				EVEN << Detector << "," << numSPM_or_RXN << "," << Stored << "," << s << "," << v << "," << Even_Data_Storage[x][s][v][0] << "," << Even_Data_Storage[x][s][v][1] << "," <<  Even_Data_Storage[x][s][v][2] << "," <<  Even_Data_Storage[x][s][v][3] << "," <<  Even_Data_Storage[x][s][v][4] << "," <<  Even_Data_Storage[x][s][v][5] << "," <<  Even_Data_Storage[x][s][v][6] << "," << Even_Data_Storage[x][s][v][7] << "," <<  Even_Data_Storage[x][s][v][8] << "," <<  Even_Data_Storage[x][s][v][9] << "," <<  Even_Data_Storage[x][s][v][10] << "," <<  Even_Data_Storage[x][s][v][11] << "," <<  Even_Data_Storage[x][s][v][12] << "," << Even_Data_Storage[x][s][v][13] << "," << Even_Data_Storage[x][s][v][14] << "," << Even_Data_Storage[x][s][v][15] << "\n";
				EVEN << Detector << "," << numSPM_or_RXN << "," << 1 << "," << s << "," << v << "," << Even_Data_Storage[2][s][v][0] << "," << Even_Data_Storage[2][s][v][1] << "," <<  Even_Data_Storage[2][s][v][2] << "," <<  Even_Data_Storage[2][s][v][3] << "," <<  Even_Data_Storage[2][s][v][4] << "," <<  Even_Data_Storage[2][s][v][5] << "," <<  Even_Data_Storage[2][s][v][6] << "," << Even_Data_Storage[2][s][v][7] << "," <<  Even_Data_Storage[2][s][v][8] << "," <<  Even_Data_Storage[2][s][v][9] << "," <<  Even_Data_Storage[2][s][v][10] << "," <<  Even_Data_Storage[2][s][v][11] << "," <<  Even_Data_Storage[2][s][v][12] << "," << Even_Data_Storage[2][s][v][13] << "," << Even_Data_Storage[2][s][v][14] << "," << Even_Data_Storage[2][s][v][15] << "\n";

			}
		}
	}

	ODD.close();
	EVEN.close();

	return;
}