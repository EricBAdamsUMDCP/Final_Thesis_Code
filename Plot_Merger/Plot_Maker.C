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
#include <iostream>
#include <iomanip>
#include "TFile.h"
#include "TProfile.h"
#include "TH1.h"
#include "TH2.h"
#include "TProfile2D.h"
#include "TMath.h"
#include "TCanvas.h"
#include "TStyle.h"
using namespace std;

int runnumber[10] = {326776, 326822, 326883, 326943, 327078, 327211, 327327, 327464, 327524, 327560};
int num_events[10] = {7003688, 554328, 1256669, 2915939, 4313507, 5989364, 7381724, 26153915, 3584837, 2969136}; //in runnumber order

void Thesis_Plot_Maker( string SPM_or_RXN, bool Special_Cuts){

	if (SPM_or_RXN != "SPM" or SPM_or_RXN != "RXN"){
		cout << "Error: SPM or RXN must be specified to determine analysis type... " << endl;
		return;
	}
	string centralitystring[10] = {0};// used for both ttypes of cuts

	string centralitystring_OG[10] = {"0-10%", "10-20%", "20-30%", "30-40%", "40-50%", "50-60%", "60-70%", "70-80%", "80-90%", "90-100%"};
	string centralitystring_Special_Cuts[3] = {"10-60_10-20%", "5-40_10-20%", "5-80_10-20%"}; //used a code technicality to store the centrality data for non standard centrality units in the 10-20% bin


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

	//create storage plots
	TH1D* HF_Even_Storage [10][4][3];
	TH1D* HF_Odd_Storage  [10][4][3];
	TH1D* RPD_Even_Storage[10][4][3];
	TH1D* RPD_Odd_Storage [10][4][3];

	TH1D* HF_Even_Storage_Special_Cuts [3][4][3];
	TH1D* HF_Odd_Storage_Special_Cuts  [3][4][3];
	TH1D* RPD_Even_Storage_Special_Cuts[3][4][3];
	TH1D* RPD_Odd_Storage_Special_Cuts [3][4][3];

	if(Special_Cuts == true){
		CentMax = 3;
	}




	double Bin_Content_HF_Even_326776[2][16] = {0}; double Bin_Content_HF_Odd_326776[2][7] = {0}; double Bin_Content_RPD_Even_326776[2][16] = {0}; double Bin_Content_RPD_Odd_326776[2][7] = {0}; //[0][0..15||6] are the histogram values
	double Bin_Content_HF_Even_326822[2][16] = {0}; double Bin_Content_HF_Odd_326822[2][7] = {0}; double Bin_Content_RPD_Even_326822[2][16] = {0}; double Bin_Content_RPD_Odd_326822[2][7] = {0}; //[1][0..15||6] are the histogram statistical errors
	double Bin_Content_HF_Even_326883[2][16] = {0}; double Bin_Content_HF_Odd_326883[2][7] = {0}; double Bin_Content_RPD_Even_326883[2][16] = {0}; double Bin_Content_RPD_Odd_326883[2][7] = {0};
	double Bin_Content_HF_Even_326943[2][16] = {0}; double Bin_Content_HF_Odd_326943[2][7] = {0}; double Bin_Content_RPD_Even_326943[2][16] = {0}; double Bin_Content_RPD_Odd_326943[2][7] = {0};
	double Bin_Content_HF_Even_327078[2][16] = {0}; double Bin_Content_HF_Odd_327078[2][7] = {0}; double Bin_Content_RPD_Even_327078[2][16] = {0}; double Bin_Content_RPD_Odd_327078[2][7] = {0};
	double Bin_Content_HF_Even_327211[2][16] = {0}; double Bin_Content_HF_Odd_327211[2][7] = {0}; double Bin_Content_RPD_Even_327211[2][16] = {0}; double Bin_Content_RPD_Odd_327211[2][7] = {0};
	double Bin_Content_HF_Even_327327[2][16] = {0}; double Bin_Content_HF_Odd_327327[2][7] = {0}; double Bin_Content_RPD_Even_327327[2][16] = {0}; double Bin_Content_RPD_Odd_327327[2][7] = {0};
	double Bin_Content_HF_Even_327464[2][16] = {0}; double Bin_Content_HF_Odd_327464[2][7] = {0}; double Bin_Content_RPD_Even_327464[2][16] = {0}; double Bin_Content_RPD_Odd_327464[2][7] = {0};
	double Bin_Content_HF_Even_327524[2][16] = {0}; double Bin_Content_HF_Odd_327524[2][7] = {0}; double Bin_Content_RPD_Even_327524[2][16] = {0}; double Bin_Content_RPD_Odd_327524[2][7] = {0};
	double Bin_Content_HF_Even_327560[2][16] = {0}; double Bin_Content_HF_Odd_327560[2][7] = {0}; double Bin_Content_RPD_Even_327560[2][16] = {0}; double Bin_Content_RPD_Odd_327560[2][7] = {0};

	double WA_HF_Even_value = 0;
	double WA_RPD_Even_value = 0;
	double WA_HF_Even_error = 0;
	double WA_RPD_Even_error = 0;
	
	for (int c = 0; c <CentMax; c++){ //cent bins
		for (int s = 0; s < 4; s++){//systematics
			for(int v = 0; v < 3; v++){//vertex
	
				if(Special_Cuts == false){
					//creating storage plots
					HF_Even_Storage[c][s][v]  = new TH1D(Form("%s_HF_Even_Sys_%s_Ver_%s_Cent_%s", SPM_or_RXN.c_str(), Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str()),  Form("%s_HF_Even_Sys_%s_Ver_%s_Cent_%s; NOT FOR THESIS USE STORAGE ONLY", SPM_or_RXN.c_str(), Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str()),  16, 0, 8);
					HF_Odd_Storage[c][s][v]   = new TH1D(Form("%s_HF_Odd_Sys_%s_Ver_%s_Cent_%s", SPM_or_RXN.c_str(), Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str()),   Form("%s_HF_Odd_Sys_%s_Ver_%s_Cent_%s; NOT FOR THESIS USE STORAGE ONLY", SPM_or_RXN.c_str(), Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str()),   7, -1.25, 1.25);
					RPD_Even_Storage[c][s][v] = new TH1D(Form("%s_RPD_Even_Sys_%s_Ver_%s_Cent_%s", SPM_or_RXN.c_str(), Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str()), Form("%s_RPD_Even_Sys_%s_Ver_%s_Cent_%s; NOT FOR THESIS USE STORAGE ONLY", SPM_or_RXN.c_str(), Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str()), 16, 0, 8);
					RPD_Odd_Storage[c][s][v]  = new TH1D(Form("%s_RPD_Odd_Sys_%s_Ver_%s_Cent_%s", SPM_or_RXN.c_str(), Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str()),  Form("%s_RPD_Odd_Sys_%s_Ver_%s_Cent_%s; NOT FOR THESIS USE STORAGE ONLY", SPM_or_RXN.c_str(), Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str()),  7, -1.25, 1.25);
				}
				else if(Special_Cuts == true){
					//creating storage plots
					HF_Even_Storage_Special_Cuts[c][s][v]  = new TH1D(Form("%s_HF_Even_Sys_%s_Ver_%s_Cent_%s", SPM_or_RXN.c_str(), Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str()),  Form("%s_HF_Even_Sys_%s_Ver_%s_Cent_%s; NOT FOR THESIS USE STORAGE ONLY", SPM_or_RXN.c_str(), Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str()),  16, 0, 8);
					HF_Odd_Storage_Special_Cuts[c][s][v]   = new TH1D(Form("%s_HF_Odd_Sys_%s_Ver_%s_Cent_%s", SPM_or_RXN.c_str(), Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str()),   Form("%s_HF_Odd_Sys_%s_Ver_%s_Cent_%s; NOT FOR THESIS USE STORAGE ONLY", SPM_or_RXN.c_str(), Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str()),   7, -1.25, 1.25);
					RPD_Even_Storage_Special_Cuts[c][s][v] = new TH1D(Form("%s_RPD_Even_Sys_%s_Ver_%s_Cent_%s", SPM_or_RXN.c_str(), Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str()), Form("%s_RPD_Even_Sys_%s_Ver_%s_Cent_%s; NOT FOR THESIS USE STORAGE ONLY", SPM_or_RXN.c_str(), Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str()), 16, 0, 8);
					RPD_Odd_Storage_Special_Cuts[c][s][v]  = new TH1D(Form("%s_RPD_Odd_Sys_%s_Ver_%s_Cent_%s", SPM_or_RXN.c_str(), Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str()),  Form("%s_RPD_Odd_Sys_%s_Ver_%s_Cent_%s; NOT FOR THESIS USE STORAGE ONLY", SPM_or_RXN.c_str(), Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str()),  7, -1.25, 1.25);
				}

				if (SPM_or_RXN == "RXN"){
				//(A) Part A draws in and loads the data files
					//RXN PLANE METHOD//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					//HF RXN Plane Method
					//Even
					TFile* HF_RXN_Even_326776 = new TFile(Form("/data2/users/ebadams/Eric_HF_RXN_Data_Sets/326776/ROOT/%s/%s/326776_V1_Even_Histo_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str())); //allocating mem for the TP
					TFile* HF_RXN_Even_326822 = new TFile(Form("/data2/users/ebadams/Eric_HF_RXN_Data_Sets/326822/ROOT/%s/%s/326822_V1_Even_Histo_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str()));
					TFile* HF_RXN_Even_326883 = new TFile(Form("/data2/users/ebadams/Eric_HF_RXN_Data_Sets/326883/ROOT/%s/%s/326883_V1_Even_Histo_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str()));
					TFile* HF_RXN_Even_326943 = new TFile(Form("/data2/users/ebadams/Eric_HF_RXN_Data_Sets/326943/ROOT/%s/%s/326943_V1_Even_Histo_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str()));
					TFile* HF_RXN_Even_327078 = new TFile(Form("/data2/users/ebadams/Eric_HF_RXN_Data_Sets/327078/ROOT/%s/%s/327078_V1_Even_Histo_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str())); //allocating mem for the TP
					TFile* HF_RXN_Even_327211 = new TFile(Form("/data2/users/ebadams/Eric_HF_RXN_Data_Sets/327211/ROOT/%s/%s/327211_V1_Even_Histo_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str()));
					TFile* HF_RXN_Even_327327 = new TFile(Form("/data2/users/ebadams/Eric_HF_RXN_Data_Sets/327327/ROOT/%s/%s/327327_V1_Even_Histo_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str()));
					TFile* HF_RXN_Even_327464 = new TFile(Form("/data2/users/ebadams/Eric_HF_RXN_Data_Sets/327464/ROOT/%s/%s/327464_V1_Even_Histo_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str()));
					TFile* HF_RXN_Even_327524 = new TFile(Form("/data2/users/ebadams/Eric_HF_RXN_Data_Sets/327524/ROOT/%s/%s/327524_V1_Even_Histo_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str())); //allocating mem for the TP
					TFile* HF_RXN_Even_327560 = new TFile(Form("/data2/users/ebadams/Eric_HF_RXN_Data_Sets/327560/ROOT/%s/%s/327560_V1_Even_Histo_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str()));
					//Odd
					TFile* HF_RXN_Odd_326776 = new TFile(Form("/data2/users/ebadams/Eric_HF_RXN_Data_Sets/326776/ROOT/%s/%s/326776_V1_Odd_Histo_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str())); //allocating mem for the TP
					TFile* HF_RXN_Odd_326822 = new TFile(Form("/data2/users/ebadams/Eric_HF_RXN_Data_Sets/326822/ROOT/%s/%s/326822_V1_Odd_Histo_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str()));
					TFile* HF_RXN_Odd_326883 = new TFile(Form("/data2/users/ebadams/Eric_HF_RXN_Data_Sets/326883/ROOT/%s/%s/326883_V1_Odd_Histo_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str()));
					TFile* HF_RXN_Odd_326943 = new TFile(Form("/data2/users/ebadams/Eric_HF_RXN_Data_Sets/326943/ROOT/%s/%s/326943_V1_Odd_Histo_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str()));
					TFile* HF_RXN_Odd_327078 = new TFile(Form("/data2/users/ebadams/Eric_HF_RXN_Data_Sets/327078/ROOT/%s/%s/327078_V1_Odd_Histo_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str())); //allocating mem for the TP
					TFile* HF_RXN_Odd_327211 = new TFile(Form("/data2/users/ebadams/Eric_HF_RXN_Data_Sets/327211/ROOT/%s/%s/327211_V1_Odd_Histo_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str()));
					TFile* HF_RXN_Odd_327327 = new TFile(Form("/data2/users/ebadams/Eric_HF_RXN_Data_Sets/327327/ROOT/%s/%s/327327_V1_Odd_Histo_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str()));
					TFile* HF_RXN_Odd_327464 = new TFile(Form("/data2/users/ebadams/Eric_HF_RXN_Data_Sets/327464/ROOT/%s/%s/327464_V1_Odd_Histo_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str()));
					TFile* HF_RXN_Odd_327524 = new TFile(Form("/data2/users/ebadams/Eric_HF_RXN_Data_Sets/327524/ROOT/%s/%s/327524_V1_Odd_Histo_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str())); //allocating mem for the TP
					TFile* HF_RXN_Odd_327560 = new TFile(Form("/data2/users/ebadams/Eric_HF_RXN_Data_Sets/327560/ROOT/%s/%s/327560_V1_Odd_Histo_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str()));
	
					//RPD RXN Plane Method
					//Even
					TFile* RPD_RXN_Even_326776 = new TFile(Form("/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/326776/ROOT/%s/%s/326776_V1_Even_Histo_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str())); //allocating mem for the TP
					TFile* RPD_RXN_Even_326822 = new TFile(Form("/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/326822/ROOT/%s/%s/326822_V1_Even_Histo_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str()));
					TFile* RPD_RXN_Even_326883 = new TFile(Form("/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/326883/ROOT/%s/%s/326883_V1_Even_Histo_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str()));
					TFile* RPD_RXN_Even_326943 = new TFile(Form("/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/326943/ROOT/%s/%s/326943_V1_Even_Histo_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str()));
					TFile* RPD_RXN_Even_327078 = new TFile(Form("/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/327078/ROOT/%s/%s/327078_V1_Even_Histo_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str())); //allocating mem for the TP
					TFile* RPD_RXN_Even_327211 = new TFile(Form("/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/327211/ROOT/%s/%s/327211_V1_Even_Histo_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str()));
					TFile* RPD_RXN_Even_327327 = new TFile(Form("/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/327327/ROOT/%s/%s/327327_V1_Even_Histo_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str()));
					TFile* RPD_RXN_Even_327464 = new TFile(Form("/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/327464/ROOT/%s/%s/327464_V1_Even_Histo_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str()));
					TFile* RPD_RXN_Even_327524 = new TFile(Form("/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/327524/ROOT/%s/%s/327524_V1_Even_Histo_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str())); //allocating mem for the TP
					TFile* RPD_RXN_Even_327560 = new TFile(Form("/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/327560/ROOT/%s/%s/327560_V1_Even_Histo_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str()));
					//Odd
					TFile* RPD_RXN_Odd_326776 = new TFile(Form("/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/326776/ROOT/%s/%s/326776_V1_Odd_Histo_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str())); //allocating mem for the TP
					TFile* RPD_RXN_Odd_326822 = new TFile(Form("/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/326822/ROOT/%s/%s/326822_V1_Odd_Histo_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str()));
					TFile* RPD_RXN_Odd_326883 = new TFile(Form("/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/326883/ROOT/%s/%s/326883_V1_Odd_Histo_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str()));
					TFile* RPD_RXN_Odd_326943 = new TFile(Form("/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/326943/ROOT/%s/%s/326943_V1_Odd_Histo_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str()));
					TFile* RPD_RXN_Odd_327078 = new TFile(Form("/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/327078/ROOT/%s/%s/327078_V1_Odd_Histo_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str())); //allocating mem for the TP
					TFile* RPD_RXN_Odd_327211 = new TFile(Form("/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/327211/ROOT/%s/%s/327211_V1_Odd_Histo_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str()));
					TFile* RPD_RXN_Odd_327327 = new TFile(Form("/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/327327/ROOT/%s/%s/327327_V1_Odd_Histo_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str()));
					TFile* RPD_RXN_Odd_327464 = new TFile(Form("/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/327464/ROOT/%s/%s/327464_V1_Odd_Histo_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str()));
					TFile* RPD_RXN_Odd_327524 = new TFile(Form("/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/327524/ROOT/%s/%s/327524_V1_Odd_Histo_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str())); //allocating mem for the TP
					TFile* RPD_RXN_Odd_327560 = new TFile(Form("/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/327560/ROOT/%s/%s/327560_V1_Odd_Histo_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str()));

					//(B) Part B get the actual data from the loaded in plots using the "Get" command
					TProfile* Get_HF_Even_326776 = (TProfile*) HF_RXN_Even_326776 ->Get(Form("HF_V1_Even_%s", centralitystring[c].c_str()));
					TProfile* Get_HF_Even_326822 = (TProfile*) HF_RXN_Even_326822 ->Get(Form("HF_V1_Even_%s", centralitystring[c].c_str()));
					TProfile* Get_HF_Even_326883 = (TProfile*) HF_RXN_Even_326883 ->Get(Form("HF_V1_Even_%s", centralitystring[c].c_str()));
					TProfile* Get_HF_Even_326943 = (TProfile*) HF_RXN_Even_326943 ->Get(Form("HF_V1_Even_%s", centralitystring[c].c_str()));
					TProfile* Get_HF_Even_327078 = (TProfile*) HF_RXN_Even_327078 ->Get(Form("HF_V1_Even_%s", centralitystring[c].c_str()));
					TProfile* Get_HF_Even_327211 = (TProfile*) HF_RXN_Even_327211 ->Get(Form("HF_V1_Even_%s", centralitystring[c].c_str()));
					TProfile* Get_HF_Even_327327 = (TProfile*) HF_RXN_Even_327327 ->Get(Form("HF_V1_Even_%s", centralitystring[c].c_str()));
					TProfile* Get_HF_Even_327464 = (TProfile*) HF_RXN_Even_327464 ->Get(Form("HF_V1_Even_%s", centralitystring[c].c_str()));
					TProfile* Get_HF_Even_327524 = (TProfile*) HF_RXN_Even_327524 ->Get(Form("HF_V1_Even_%s", centralitystring[c].c_str()));
					TProfile* Get_HF_Even_327560 = (TProfile*) HF_RXN_Even_327560 ->Get(Form("HF_V1_Even_%s", centralitystring[c].c_str()));
	
					TProfile* Get_HF__Odd_326776 = (TProfile*) HF_RXN_Odd_326776 ->Get(Form("HF_V1_Odd_%s", centralitystring[c].c_str()));
					TProfile* Get_HF__Odd_326822 = (TProfile*) HF_RXN_Odd_326822 ->Get(Form("HF_V1_Odd_%s", centralitystring[c].c_str()));
					TProfile* Get_HF__Odd_326883 = (TProfile*) HF_RXN_Odd_326883 ->Get(Form("HF_V1_Odd_%s", centralitystring[c].c_str()));
					TProfile* Get_HF__Odd_326943 = (TProfile*) HF_RXN_Odd_326943 ->Get(Form("HF_V1_Odd_%s", centralitystring[c].c_str()));
					TProfile* Get_HF__Odd_327078 = (TProfile*) HF_RXN_Odd_327078 ->Get(Form("HF_V1_Odd_%s", centralitystring[c].c_str()));
					TProfile* Get_HF__Odd_327211 = (TProfile*) HF_RXN_Odd_327211 ->Get(Form("HF_V1_Odd_%s", centralitystring[c].c_str()));
					TProfile* Get_HF__Odd_327327 = (TProfile*) HF_RXN_Odd_327327 ->Get(Form("HF_V1_Odd_%s", centralitystring[c].c_str()));
					TProfile* Get_HF__Odd_327464 = (TProfile*) HF_RXN_Odd_327464 ->Get(Form("HF_V1_Odd_%s", centralitystring[c].c_str()));
					TProfile* Get_HF__Odd_327524 = (TProfile*) HF_RXN_Odd_327524 ->Get(Form("HF_V1_Odd_%s", centralitystring[c].c_str()));
					TProfile* Get_HF__Odd_327560 = (TProfile*) HF_RXN_Odd_327560 ->Get(Form("HF_V1_Odd_%s", centralitystring[c].c_str()));
		
					TProfile* Get_RPD_Even_326776 = (TProfile*) RPD_RXN_Even_326776 ->Get(Form("RPD_V1_Even_%s", centralitystring[c].c_str()));
					TProfile* Get_RPD_Even_326822 = (TProfile*) RPD_RXN_Even_326822 ->Get(Form("RPD_V1_Even_%s", centralitystring[c].c_str()));
					TProfile* Get_RPD_Even_326883 = (TProfile*) RPD_RXN_Even_326883 ->Get(Form("RPD_V1_Even_%s", centralitystring[c].c_str()));
					TProfile* Get_RPD_Even_326943 = (TProfile*) RPD_RXN_Even_326943 ->Get(Form("RPD_V1_Even_%s", centralitystring[c].c_str()));
					TProfile* Get_RPD_Even_327078 = (TProfile*) RPD_RXN_Even_327078 ->Get(Form("RPD_V1_Even_%s", centralitystring[c].c_str()));
					TProfile* Get_RPD_Even_327211 = (TProfile*) RPD_RXN_Even_327211 ->Get(Form("RPD_V1_Even_%s", centralitystring[c].c_str()));
					TProfile* Get_RPD_Even_327327 = (TProfile*) RPD_RXN_Even_327327 ->Get(Form("RPD_V1_Even_%s", centralitystring[c].c_str()));
					TProfile* Get_RPD_Even_327464 = (TProfile*) RPD_RXN_Even_327464 ->Get(Form("RPD_V1_Even_%s", centralitystring[c].c_str()));
					TProfile* Get_RPD_Even_327524 = (TProfile*) RPD_RXN_Even_327524 ->Get(Form("RPD_V1_Even_%s", centralitystring[c].c_str()));
					TProfile* Get_RPD_Even_327560 = (TProfile*) RPD_RXN_Even_327560 ->Get(Form("RPD_V1_Even_%s", centralitystring[c].c_str()));
		
					TProfile* Get_RPD_Odd_326776 = (TProfile*) RPD_RXN_Odd_326776 ->Get(Form("RPD_V1_Odd_%s", centralitystring[c].c_str()));
					TProfile* Get_RPD_Odd_326822 = (TProfile*) RPD_RXN_Odd_326822 ->Get(Form("RPD_V1_Odd_%s", centralitystring[c].c_str()));
					TProfile* Get_RPD_Odd_326883 = (TProfile*) RPD_RXN_Odd_326883 ->Get(Form("RPD_V1_Odd_%s", centralitystring[c].c_str()));
					TProfile* Get_RPD_Odd_326943 = (TProfile*) RPD_RXN_Odd_326943 ->Get(Form("RPD_V1_Odd_%s", centralitystring[c].c_str()));
					TProfile* Get_RPD_Odd_327078 = (TProfile*) RPD_RXN_Odd_327078 ->Get(Form("RPD_V1_Odd_%s", centralitystring[c].c_str()));
					TProfile* Get_RPD_Odd_327211 = (TProfile*) RPD_RXN_Odd_327211 ->Get(Form("RPD_V1_Odd_%s", centralitystring[c].c_str()));
					TProfile* Get_RPD_Odd_327327 = (TProfile*) RPD_RXN_Odd_327327 ->Get(Form("RPD_V1_Odd_%s", centralitystring[c].c_str()));
					TProfile* Get_RPD_Odd_327464 = (TProfile*) RPD_RXN_Odd_327464 ->Get(Form("RPD_V1_Odd_%s", centralitystring[c].c_str()));
					TProfile* Get_RPD_Odd_327524 = (TProfile*) RPD_RXN_Odd_327524 ->Get(Form("RPD_V1_Odd_%s", centralitystring[c].c_str()));
					TProfile* Get_RPD_Odd_327560 = (TProfile*) RPD_RXN_Odd_327560 ->Get(Form("RPD_V1_Odd_%s", centralitystring[c].c_str()));
				}
				else{ //if else not needed due to rejection return at begining			
					//SPM METHOD/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					//HF  SPM Method
					//Even
					TFile* HF_SPM_Even_326776 = new TFile(Form("/data2/users/ebadams/Eric_SP_Data_Sets/HF/326776/ROOT/%s/%s/RunNumber_326776_Sys_%s_Vtx_%s_HF_V1_Even_Histo_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str())); //allocating mem for the TP
					TFile* HF_SPM_Even_326822 = new TFile(Form("/data2/users/ebadams/Eric_SP_Data_Sets/HF/326822/ROOT/%s/%s/RunNumber_326822_Sys_%s_Vtx_%s_HF_V1_Even_Histo_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str()));
					TFile* HF_SPM_Even_326883 = new TFile(Form("/data2/users/ebadams/Eric_SP_Data_Sets/HF/326883/ROOT/%s/%s/RunNumber_326883_Sys_%s_Vtx_%s_HF_V1_Even_Histo_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str()));
					TFile* HF_SPM_Even_326943 = new TFile(Form("/data2/users/ebadams/Eric_SP_Data_Sets/HF/326943/ROOT/%s/%s/RunNumber_326943_Sys_%s_Vtx_%s_HF_V1_Even_Histo_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str()));
					TFile* HF_SPM_Even_327078 = new TFile(Form("/data2/users/ebadams/Eric_SP_Data_Sets/HF/327078/ROOT/%s/%s/RunNumber_327078_Sys_%s_Vtx_%s_HF_V1_Even_Histo_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str())); //allocating mem for the TP
					TFile* HF_SPM_Even_327211 = new TFile(Form("/data2/users/ebadams/Eric_SP_Data_Sets/HF/327211/ROOT/%s/%s/RunNumber_327211_Sys_%s_Vtx_%s_HF_V1_Even_Histo_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str()));
					TFile* HF_SPM_Even_327327 = new TFile(Form("/data2/users/ebadams/Eric_SP_Data_Sets/HF/327327/ROOT/%s/%s/RunNumber_327327_Sys_%s_Vtx_%s_HF_V1_Even_Histo_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str()));
					TFile* HF_SPM_Even_327464 = new TFile(Form("/data2/users/ebadams/Eric_SP_Data_Sets/HF/327464/ROOT/%s/%s/RunNumber_327464_Sys_%s_Vtx_%s_HF_V1_Even_Histo_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str()));
					TFile* HF_SPM_Even_327524 = new TFile(Form("/data2/users/ebadams/Eric_SP_Data_Sets/HF/327524/ROOT/%s/%s/RunNumber_327524_Sys_%s_Vtx_%s_HF_V1_Even_Histo_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str())); //allocating mem for the TP
					TFile* HF_SPM_Even_327560 = new TFile(Form("/data2/users/ebadams/Eric_SP_Data_Sets/HF/327560/ROOT/%s/%s/RunNumber_327560_Sys_%s_Vtx_%s_HF_V1_Even_Histo_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str()));
					//Odd
					TFile* HF_SPM_Odd_326776 = new TFile(Form("/data2/users/ebadams/Eric_SP_Data_Sets/HF/326776/ROOT/%s/%s/RunNumber_326776_Sys_%s_Vtx_%s_HF_V1_Odd_Histo_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str())); //allocating mem for the TP
					TFile* HF_SPM_Odd_326822 = new TFile(Form("/data2/users/ebadams/Eric_SP_Data_Sets/HF/326822/ROOT/%s/%s/RunNumber_326822_Sys_%s_Vtx_%s_HF_V1_Odd_Histo_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str()));
					TFile* HF_SPM_Odd_326883 = new TFile(Form("/data2/users/ebadams/Eric_SP_Data_Sets/HF/326883/ROOT/%s/%s/RunNumber_326883_Sys_%s_Vtx_%s_HF_V1_Odd_Histo_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str()));
					TFile* HF_SPM_Odd_326943 = new TFile(Form("/data2/users/ebadams/Eric_SP_Data_Sets/HF/326943/ROOT/%s/%s/RunNumber_326943_Sys_%s_Vtx_%s_HF_V1_Odd_Histo_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str()));
					TFile* HF_SPM_Odd_327078 = new TFile(Form("/data2/users/ebadams/Eric_SP_Data_Sets/HF/327078/ROOT/%s/%s/RunNumber_327078_Sys_%s_Vtx_%s_HF_V1_Odd_Histo_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str())); //allocating mem for the TP
					TFile* HF_SPM_Odd_327211 = new TFile(Form("/data2/users/ebadams/Eric_SP_Data_Sets/HF/327211/ROOT/%s/%s/RunNumber_327211_Sys_%s_Vtx_%s_HF_V1_Odd_Histo_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str()));
					TFile* HF_SPM_Odd_327327 = new TFile(Form("/data2/users/ebadams/Eric_SP_Data_Sets/HF/327327/ROOT/%s/%s/RunNumber_327327_Sys_%s_Vtx_%s_HF_V1_Odd_Histo_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str()));
					TFile* HF_SPM_Odd_327464 = new TFile(Form("/data2/users/ebadams/Eric_SP_Data_Sets/HF/327464/ROOT/%s/%s/RunNumber_327464_Sys_%s_Vtx_%s_HF_V1_Odd_Histo_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str()));
					TFile* HF_SPM_Odd_327524 = new TFile(Form("/data2/users/ebadams/Eric_SP_Data_Sets/HF/327524/ROOT/%s/%s/RunNumber_327524_Sys_%s_Vtx_%s_HF_V1_Odd_Histo_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str())); //allocating mem for the TP
					TFile* HF_SPM_Odd_327560 = new TFile(Form("/data2/users/ebadams/Eric_SP_Data_Sets/HF/327560/ROOT/%s/%s/RunNumber_327560_Sys_%s_Vtx_%s_HF_V1_Odd_Histo_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str()));
		
					//RPD SPM Method
					//Even
					TFile* RPD_SPM_Even_326776 = new TFile(Form("/data2/users/ebadams/Eric_SP_Data_Sets/RPD/326776/ROOT/%s/%s/RunNumber_326776_Sys_%s_Vtx_%s_RPD_V1_Even_Histo_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str())); //allocating mem for the TP
					TFile* RPD_SPM_Even_326822 = new TFile(Form("/data2/users/ebadams/Eric_SP_Data_Sets/RPD/326822/ROOT/%s/%s/RunNumber_326822_Sys_%s_Vtx_%s_RPD_V1_Even_Histo_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str()));
					TFile* RPD_SPM_Even_326883 = new TFile(Form("/data2/users/ebadams/Eric_SP_Data_Sets/RPD/326883/ROOT/%s/%s/RunNumber_326883_Sys_%s_Vtx_%s_RPD_V1_Even_Histo_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str()));
					TFile* RPD_SPM_Even_326943 = new TFile(Form("/data2/users/ebadams/Eric_SP_Data_Sets/RPD/326943/ROOT/%s/%s/RunNumber_326943_Sys_%s_Vtx_%s_RPD_V1_Even_Histo_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str()));
					TFile* RPD_SPM_Even_327078 = new TFile(Form("/data2/users/ebadams/Eric_SP_Data_Sets/RPD/327078/ROOT/%s/%s/RunNumber_327078_Sys_%s_Vtx_%s_RPD_V1_Even_Histo_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str())); //allocating mem for the TP
					TFile* RPD_SPM_Even_327211 = new TFile(Form("/data2/users/ebadams/Eric_SP_Data_Sets/RPD/327211/ROOT/%s/%s/RunNumber_327211_Sys_%s_Vtx_%s_RPD_V1_Even_Histo_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str()));
					TFile* RPD_SPM_Even_327327 = new TFile(Form("/data2/users/ebadams/Eric_SP_Data_Sets/RPD/327327/ROOT/%s/%s/RunNumber_327327_Sys_%s_Vtx_%s_RPD_V1_Even_Histo_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str()));
					TFile* RPD_SPM_Even_327464 = new TFile(Form("/data2/users/ebadams/Eric_SP_Data_Sets/RPD/327464/ROOT/%s/%s/RunNumber_327464_Sys_%s_Vtx_%s_RPD_V1_Even_Histo_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str()));
					TFile* RPD_SPM_Even_327524 = new TFile(Form("/data2/users/ebadams/Eric_SP_Data_Sets/RPD/327524/ROOT/%s/%s/RunNumber_327524_Sys_%s_Vtx_%s_RPD_V1_Even_Histo_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str())); //allocating mem for the TP
					TFile* RPD_SPM_Even_327560 = new TFile(Form("/data2/users/ebadams/Eric_SP_Data_Sets/RPD/327560/ROOT/%s/%s/RunNumber_327560_Sys_%s_Vtx_%s_RPD_V1_Even_Histo_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str()));
					//Odd
					TFile* RPD_SPM_Odd_326776 = new TFile(Form("/data2/users/ebadams/Eric_SP_Data_Sets/RPD/326776/ROOT/%s/%s/RunNumber_326776_Sys_%s_Vtx_%s_RPD_V1_Odd_Histo_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str())); //allocating mem for the TP
					TFile* RPD_SPM_Odd_326822 = new TFile(Form("/data2/users/ebadams/Eric_SP_Data_Sets/RPD/326822/ROOT/%s/%s/RunNumber_326822_Sys_%s_Vtx_%s_RPD_V1_Odd_Histo_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str()));
					TFile* RPD_SPM_Odd_326883 = new TFile(Form("/data2/users/ebadams/Eric_SP_Data_Sets/RPD/326883/ROOT/%s/%s/RunNumber_326883_Sys_%s_Vtx_%s_RPD_V1_Odd_Histo_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str()));
					TFile* RPD_SPM_Odd_326943 = new TFile(Form("/data2/users/ebadams/Eric_SP_Data_Sets/RPD/326943/ROOT/%s/%s/RunNumber_326943_Sys_%s_Vtx_%s_RPD_V1_Odd_Histo_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str()));
					TFile* RPD_SPM_Odd_327078 = new TFile(Form("/data2/users/ebadams/Eric_SP_Data_Sets/RPD/327078/ROOT/%s/%s/RunNumber_327078_Sys_%s_Vtx_%s_RPD_V1_Odd_Histo_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str())); //allocating mem for the TP
					TFile* RPD_SPM_Odd_327211 = new TFile(Form("/data2/users/ebadams/Eric_SP_Data_Sets/RPD/327211/ROOT/%s/%s/RunNumber_327211_Sys_%s_Vtx_%s_RPD_V1_Odd_Histo_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str()));
					TFile* RPD_SPM_Odd_327327 = new TFile(Form("/data2/users/ebadams/Eric_SP_Data_Sets/RPD/327327/ROOT/%s/%s/RunNumber_327327_Sys_%s_Vtx_%s_RPD_V1_Odd_Histo_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str()));
					TFile* RPD_SPM_Odd_327464 = new TFile(Form("/data2/users/ebadams/Eric_SP_Data_Sets/RPD/327464/ROOT/%s/%s/RunNumber_327464_Sys_%s_Vtx_%s_RPD_V1_Odd_Histo_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str()));
					TFile* RPD_SPM_Odd_327524 = new TFile(Form("/data2/users/ebadams/Eric_SP_Data_Sets/RPD/327524/ROOT/%s/%s/RunNumber_327524_Sys_%s_Vtx_%s_RPD_V1_Odd_Histo_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str())); //allocating mem for the TP
					TFile* RPD_SPM_Odd_327560 = new TFile(Form("/data2/users/ebadams/Eric_SP_Data_Sets/RPD/327560/ROOT/%s/%s/RunNumber_327560_Sys_%s_Vtx_%s_RPD_V1_Odd_Histo_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str()));
					
					//(B) Part B get the actual data from the loaded in plots using the "Get" command
					TProfile* Get_HF_Even_326776 = (TProfile*) HF_SPM_Even_326776 ->Get(Form("HF_V1_Even_%s", centralitystring[c].c_str()));
					TProfile* Get_HF_Even_326822 = (TProfile*) HF_SPM_Even_326822 ->Get(Form("HF_V1_Even_%s", centralitystring[c].c_str()));
					TProfile* Get_HF_Even_326883 = (TProfile*) HF_SPM_Even_326883 ->Get(Form("HF_V1_Even_%s", centralitystring[c].c_str()));
					TProfile* Get_HF_Even_326943 = (TProfile*) HF_SPM_Even_326943 ->Get(Form("HF_V1_Even_%s", centralitystring[c].c_str()));
					TProfile* Get_HF_Even_327078 = (TProfile*) HF_SPM_Even_327078 ->Get(Form("HF_V1_Even_%s", centralitystring[c].c_str()));
					TProfile* Get_HF_Even_327211 = (TProfile*) HF_SPM_Even_327211 ->Get(Form("HF_V1_Even_%s", centralitystring[c].c_str()));
					TProfile* Get_HF_Even_327327 = (TProfile*) HF_SPM_Even_327327 ->Get(Form("HF_V1_Even_%s", centralitystring[c].c_str()));
					TProfile* Get_HF_Even_327464 = (TProfile*) HF_SPM_Even_327464 ->Get(Form("HF_V1_Even_%s", centralitystring[c].c_str()));
					TProfile* Get_HF_Even_327524 = (TProfile*) HF_SPM_Even_327524 ->Get(Form("HF_V1_Even_%s", centralitystring[c].c_str()));
					TProfile* Get_HF_Even_327560 = (TProfile*) HF_SPM_Even_327560 ->Get(Form("HF_V1_Even_%s", centralitystring[c].c_str()));
		
					TProfile* Get_HF__Odd_326776 = (TProfile*) HF_SPM_Odd_326776 ->Get(Form("HF_V1_Odd_%s", centralitystring[c].c_str()));
					TProfile* Get_HF__Odd_326822 = (TProfile*) HF_SPM_Odd_326822 ->Get(Form("HF_V1_Odd_%s", centralitystring[c].c_str()));
					TProfile* Get_HF__Odd_326883 = (TProfile*) HF_SPM_Odd_326883 ->Get(Form("HF_V1_Odd_%s", centralitystring[c].c_str()));
					TProfile* Get_HF__Odd_326943 = (TProfile*) HF_SPM_Odd_326943 ->Get(Form("HF_V1_Odd_%s", centralitystring[c].c_str()));
					TProfile* Get_HF__Odd_327078 = (TProfile*) HF_SPM_Odd_327078 ->Get(Form("HF_V1_Odd_%s", centralitystring[c].c_str()));
					TProfile* Get_HF__Odd_327211 = (TProfile*) HF_SPM_Odd_327211 ->Get(Form("HF_V1_Odd_%s", centralitystring[c].c_str()));
					TProfile* Get_HF__Odd_327327 = (TProfile*) HF_SPM_Odd_327327 ->Get(Form("HF_V1_Odd_%s", centralitystring[c].c_str()));
					TProfile* Get_HF__Odd_327464 = (TProfile*) HF_SPM_Odd_327464 ->Get(Form("HF_V1_Odd_%s", centralitystring[c].c_str()));
					TProfile* Get_HF__Odd_327524 = (TProfile*) HF_SPM_Odd_327524 ->Get(Form("HF_V1_Odd_%s", centralitystring[c].c_str()));
					TProfile* Get_HF__Odd_327560 = (TProfile*) HF_SPM_Odd_327560 ->Get(Form("HF_V1_Odd_%s", centralitystring[c].c_str()));
		
					TProfile* Get_RPD_Even_326776 = (TProfile*) RPD_SPM_Even_326776 ->Get(Form("RPD_V1_Even_%s", centralitystring[c].c_str()));
					TProfile* Get_RPD_Even_326822 = (TProfile*) RPD_SPM_Even_326822 ->Get(Form("RPD_V1_Even_%s", centralitystring[c].c_str()));
					TProfile* Get_RPD_Even_326883 = (TProfile*) RPD_SPM_Even_326883 ->Get(Form("RPD_V1_Even_%s", centralitystring[c].c_str()));
					TProfile* Get_RPD_Even_326943 = (TProfile*) RPD_SPM_Even_326943 ->Get(Form("RPD_V1_Even_%s", centralitystring[c].c_str()));
					TProfile* Get_RPD_Even_327078 = (TProfile*) RPD_SPM_Even_327078 ->Get(Form("RPD_V1_Even_%s", centralitystring[c].c_str()));
					TProfile* Get_RPD_Even_327211 = (TProfile*) RPD_SPM_Even_327211 ->Get(Form("RPD_V1_Even_%s", centralitystring[c].c_str()));
					TProfile* Get_RPD_Even_327327 = (TProfile*) RPD_SPM_Even_327327 ->Get(Form("RPD_V1_Even_%s", centralitystring[c].c_str()));
					TProfile* Get_RPD_Even_327464 = (TProfile*) RPD_SPM_Even_327464 ->Get(Form("RPD_V1_Even_%s", centralitystring[c].c_str()));
					TProfile* Get_RPD_Even_327524 = (TProfile*) RPD_SPM_Even_327524 ->Get(Form("RPD_V1_Even_%s", centralitystring[c].c_str()));
					TProfile* Get_RPD_Even_327560 = (TProfile*) RPD_SPM_Even_327560 ->Get(Form("RPD_V1_Even_%s", centralitystring[c].c_str()));
		
					TProfile* Get_RPD_Odd_326776 = (TProfile*) RPD_SPM_Odd_326776 ->Get(Form("RPD_V1_Odd_%s", centralitystring[c].c_str()));
					TProfile* Get_RPD_Odd_326822 = (TProfile*) RPD_SPM_Odd_326822 ->Get(Form("RPD_V1_Odd_%s", centralitystring[c].c_str()));
					TProfile* Get_RPD_Odd_326883 = (TProfile*) RPD_SPM_Odd_326883 ->Get(Form("RPD_V1_Odd_%s", centralitystring[c].c_str()));
					TProfile* Get_RPD_Odd_326943 = (TProfile*) RPD_SPM_Odd_326943 ->Get(Form("RPD_V1_Odd_%s", centralitystring[c].c_str()));
					TProfile* Get_RPD_Odd_327078 = (TProfile*) RPD_SPM_Odd_327078 ->Get(Form("RPD_V1_Odd_%s", centralitystring[c].c_str()));
					TProfile* Get_RPD_Odd_327211 = (TProfile*) RPD_SPM_Odd_327211 ->Get(Form("RPD_V1_Odd_%s", centralitystring[c].c_str()));
					TProfile* Get_RPD_Odd_327327 = (TProfile*) RPD_SPM_Odd_327327 ->Get(Form("RPD_V1_Odd_%s", centralitystring[c].c_str()));
					TProfile* Get_RPD_Odd_327464 = (TProfile*) RPD_SPM_Odd_327464 ->Get(Form("RPD_V1_Odd_%s", centralitystring[c].c_str()));
					TProfile* Get_RPD_Odd_327524 = (TProfile*) RPD_SPM_Odd_327524 ->Get(Form("RPD_V1_Odd_%s", centralitystring[c].c_str()));
					TProfile* Get_RPD_Odd_327560 = (TProfile*) RPD_SPM_Odd_327560 ->Get(Form("RPD_V1_Odd_%s", centralitystring[c].c_str()));

				}

				//(B.2) this part draws the data so it can be accessed and loaded.
				Get_HF_Even_326776 ->Draw();
				Get_HF_Even_326822 ->Draw();
				Get_HF_Even_326883 ->Draw();
				Get_HF_Even_326943 ->Draw();
				Get_HF_Even_327078 ->Draw();
				Get_HF_Even_327211 ->Draw();
				Get_HF_Even_327327 ->Draw();
				Get_HF_Even_327464 ->Draw();
				Get_HF_Even_327524 ->Draw();
				Get_HF_Even_327560 ->Draw();
	
				Get_HF__Odd_326776 ->Draw();
				Get_HF__Odd_326822 ->Draw();
				Get_HF__Odd_326883 ->Draw();
				Get_HF__Odd_326943 ->Draw();
				Get_HF__Odd_327078 ->Draw();
				Get_HF__Odd_327211 ->Draw();
				Get_HF__Odd_327327 ->Draw();
				Get_HF__Odd_327464 ->Draw();
				Get_HF__Odd_327524 ->Draw();
				Get_HF__Odd_327560 ->Draw();
	
				Get_RPD_Even_326776 ->Draw();
				Get_RPD_Even_326822 ->Draw();
				Get_RPD_Even_326883 ->Draw();
				Get_RPD_Even_326943 ->Draw();
				Get_RPD_Even_327078 ->Draw();
				Get_RPD_Even_327211 ->Draw();
				Get_RPD_Even_327327 ->Draw();
				Get_RPD_Even_327464 ->Draw();
				Get_RPD_Even_327524 ->Draw();
				Get_RPD_Even_327560 ->Draw();
	
				Get_RPD_Odd_326776 ->Draw();
				Get_RPD_Odd_326822 ->Draw();
				Get_RPD_Odd_326883 ->Draw();
				Get_RPD_Odd_326943 ->Draw();
				Get_RPD_Odd_327078 ->Draw();
				Get_RPD_Odd_327211 ->Draw();
				Get_RPD_Odd_327327 ->Draw();
				Get_RPD_Odd_327464 ->Draw();
				Get_RPD_Odd_327524 ->Draw();
				Get_RPD_Odd_327560 ->Draw();
	
				//End of (B) 
	
	
				//(C) storing the data
	
				//WEIGHTED AVERAGE FOR ALL THE RUN VALUES ADDED AND SUMMED
				for (int bin = 0; bin < 7; bin++){

					Bin_Content_HF_Odd_326776[0][bin]  = Get_HF__Odd_326776 ->GetBinContent(bin+1); Bin_Content_RPD_Odd_326776[0][bin] = Get_RPD_Odd_326776 ->GetBinContent(bin+1);
					Bin_Content_HF_Odd_326822[0][bin]  = Get_HF__Odd_326822 ->GetBinContent(bin+1); Bin_Content_RPD_Odd_326822[0][bin] = Get_RPD_Odd_326822 ->GetBinContent(bin+1);
					Bin_Content_HF_Odd_326883[0][bin]  = Get_HF__Odd_326883 ->GetBinContent(bin+1); Bin_Content_RPD_Odd_326883[0][bin] = Get_RPD_Odd_326883 ->GetBinContent(bin+1);
					Bin_Content_HF_Odd_326943[0][bin]  = Get_HF__Odd_326943 ->GetBinContent(bin+1); Bin_Content_RPD_Odd_326943[0][bin] = Get_RPD_Odd_326943 ->GetBinContent(bin+1);
					Bin_Content_HF_Odd_327078[0][bin]  = Get_HF__Odd_327078 ->GetBinContent(bin+1); Bin_Content_RPD_Odd_327078[0][bin] = Get_RPD_Odd_327078 ->GetBinContent(bin+1);
					Bin_Content_HF_Odd_327211[0][bin]  = Get_HF__Odd_327211 ->GetBinContent(bin+1); Bin_Content_RPD_Odd_327211[0][bin] = Get_RPD_Odd_327211 ->GetBinContent(bin+1);
					Bin_Content_HF_Odd_327327[0][bin]  = Get_HF__Odd_327327 ->GetBinContent(bin+1); Bin_Content_RPD_Odd_327327[0][bin] = Get_RPD_Odd_327327 ->GetBinContent(bin+1);
					Bin_Content_HF_Odd_327464[0][bin]  = Get_HF__Odd_327464 ->GetBinContent(bin+1); Bin_Content_RPD_Odd_327464[0][bin] = Get_RPD_Odd_327464 ->GetBinContent(bin+1);
					Bin_Content_HF_Odd_327524[0][bin]  = Get_HF__Odd_327524 ->GetBinContent(bin+1); Bin_Content_RPD_Odd_327524[0][bin] = Get_RPD_Odd_327524 ->GetBinContent(bin+1);
					Bin_Content_HF_Odd_327560[0][bin]  = Get_HF__Odd_327560 ->GetBinContent(bin+1); Bin_Content_RPD_Odd_327560[0][bin] = Get_RPD_Odd_327560 ->GetBinContent(bin+1);

					Bin_Content_HF_Odd_326776[1][bin]  = Get_HF__Odd_326776 ->GetBinError(bin+1); Bin_Content_RPD_Odd_326776[1][bin] = Get_RPD_Odd_326776 ->GetBinError(bin+1);
					Bin_Content_HF_Odd_326822[1][bin]  = Get_HF__Odd_326822 ->GetBinError(bin+1); Bin_Content_RPD_Odd_326822[1][bin] = Get_RPD_Odd_326822 ->GetBinError(bin+1);
					Bin_Content_HF_Odd_326883[1][bin]  = Get_HF__Odd_326883 ->GetBinError(bin+1); Bin_Content_RPD_Odd_326883[1][bin] = Get_RPD_Odd_326883 ->GetBinError(bin+1);
					Bin_Content_HF_Odd_326943[1][bin]  = Get_HF__Odd_326943 ->GetBinError(bin+1); Bin_Content_RPD_Odd_326943[1][bin] = Get_RPD_Odd_326943 ->GetBinError(bin+1);
					Bin_Content_HF_Odd_327078[1][bin]  = Get_HF__Odd_327078 ->GetBinError(bin+1); Bin_Content_RPD_Odd_327078[1][bin] = Get_RPD_Odd_327078 ->GetBinError(bin+1);
					Bin_Content_HF_Odd_327211[1][bin]  = Get_HF__Odd_327211 ->GetBinError(bin+1); Bin_Content_RPD_Odd_327211[1][bin] = Get_RPD_Odd_327211 ->GetBinError(bin+1);
					Bin_Content_HF_Odd_327327[1][bin]  = Get_HF__Odd_327327 ->GetBinError(bin+1); Bin_Content_RPD_Odd_327327[1][bin] = Get_RPD_Odd_327327 ->GetBinError(bin+1);
					Bin_Content_HF_Odd_327464[1][bin]  = Get_HF__Odd_327464 ->GetBinError(bin+1); Bin_Content_RPD_Odd_327464[1][bin] = Get_RPD_Odd_327464 ->GetBinError(bin+1);
					Bin_Content_HF_Odd_327524[1][bin]  = Get_HF__Odd_327524 ->GetBinError(bin+1); Bin_Content_RPD_Odd_327524[1][bin] = Get_RPD_Odd_327524 ->GetBinError(bin+1);
					Bin_Content_HF_Odd_327560[1][bin]  = Get_HF__Odd_327560 ->GetBinError(bin+1); Bin_Content_RPD_Odd_327560[1][bin] = Get_RPD_Odd_327560 ->GetBinError(bin+1);

					WA_HF_Odd_value = ((Bin_Content_HF_Odd_326776[0][bin]*num_events[0] //taking the weighted average to fill the final plots                                  
											 + Bin_Content_HF_Odd_326822[0][bin]*num_events[1]                                  
											 + Bin_Content_HF_Odd_326883[0][bin]*num_events[2]                                  
											 + Bin_Content_HF_Odd_326943[0][bin]*num_events[3]                                  
											 + Bin_Content_HF_Odd_327078[0][bin]*num_events[4]                                  
											 + Bin_Content_HF_Odd_327211[0][bin]*num_events[5]                                  
											 + Bin_Content_HF_Odd_327327[0][bin]*num_events[6]                                  
											 + Bin_Content_HF_Odd_327464[0][bin]*num_events[7]                                  
											 + Bin_Content_HF_Odd_327524[0][bin]*num_events[8]                                  
											 + Bin_Content_HF_Odd_327560[0][bin]*num_events[9])                                  
											 /                                  
											 (num_events[0]                                  
											  +num_events[1]                                  
											  +num_events[2]                                  
											  +num_events[3]                                  
											  +num_events[4]                                  
											  +num_events[5]                                  
											  +num_events[6]                                  
											  +num_events[7]                                  
											  +num_events[8]                                  
											  +num_events[9]                                  
											 ));                                  
                              
					WA_RPD_Odd_value =  ((Bin_Content_RPD_Odd_326776[0][bin]*num_events[0] //taking the weighted average to fill the final plots                                  
											   + Bin_Content_RPD_Odd_326822[0][bin]*num_events[1]                                  
											   + Bin_Content_RPD_Odd_326883[0][bin]*num_events[2]                                  
											   + Bin_Content_RPD_Odd_326943[0][bin]*num_events[3]                                  
											   + Bin_Content_RPD_Odd_327078[0][bin]*num_events[4]                                  
											   + Bin_Content_RPD_Odd_327211[0][bin]*num_events[5]                                  
											   + Bin_Content_RPD_Odd_327327[0][bin]*num_events[6]                                  
											   + Bin_Content_RPD_Odd_327464[0][bin]*num_events[7]                                  
											   + Bin_Content_RPD_Odd_327524[0][bin]*num_events[8]                                  
											   + Bin_Content_RPD_Odd_327560[0][bin]*num_events[9])                                  
											   /                                  
											   (num_events[0]                                  
											    +num_events[1]                                  
											    +num_events[2]                                  
											    +num_events[3]                                  
											    +num_events[4]                                  
											    +num_events[5]                                  
											    +num_events[6]                                  
											    +num_events[7]                                  
											    +num_events[8]                                  
											    +num_events[9]                                  
											   ));

					WA_HF_Odd_error = ((Bin_Content_HF_Odd_326776[1][bin]*num_events[0] //taking the weighted average to fill the final plots
											 + Bin_Content_HF_Odd_326822[1][bin]*num_events[1]
											 + Bin_Content_HF_Odd_326883[1][bin]*num_events[2]
											 + Bin_Content_HF_Odd_326943[1][bin]*num_events[3]
											 + Bin_Content_HF_Odd_327078[1][bin]*num_events[4]
											 + Bin_Content_HF_Odd_327211[1][bin]*num_events[5]
											 + Bin_Content_HF_Odd_327327[1][bin]*num_events[6]
											 + Bin_Content_HF_Odd_327464[1][bin]*num_events[7]
											 + Bin_Content_HF_Odd_327524[1][bin]*num_events[8]
											 + Bin_Content_HF_Odd_327560[1][bin]*num_events[9])
											 /
											 (num_events[0]
											  +num_events[1]
											  +num_events[2]
											  +num_events[3]
											  +num_events[4]
											  +num_events[5]
											  +num_events[6]
											  +num_events[7]
											  +num_events[8]
											  +num_events[9]
											 ));
					WA_RPD_Odd_error =  ((Bin_Content_RPD_Odd_326776[1][bin]*num_events[0] //taking the weighted average to fill the final plots
											   + Bin_Content_RPD_Odd_326822[1][bin]*num_events[1]
											   + Bin_Content_RPD_Odd_326883[1][bin]*num_events[2]
											   + Bin_Content_RPD_Odd_326943[1][bin]*num_events[3]
											   + Bin_Content_RPD_Odd_327078[1][bin]*num_events[4]
											   + Bin_Content_RPD_Odd_327211[1][bin]*num_events[5]
											   + Bin_Content_RPD_Odd_327327[1][bin]*num_events[6]
											   + Bin_Content_RPD_Odd_327464[1][bin]*num_events[7]
											   + Bin_Content_RPD_Odd_327524[1][bin]*num_events[8]
											   + Bin_Content_RPD_Odd_327560[1][bin]*num_events[9])
											   /
											   (num_events[0]
											    +num_events[1]
											    +num_events[2]
											    +num_events[3]
											    +num_events[4]
											    +num_events[5]
											    +num_events[6]
											    +num_events[7]
											    +num_events[8]
											    +num_events[9]
											   ));                                  
					if(Special_Cuts == false){
						HF_Odd_Storage[c][s][v]->SetBinContent((bin+1), WA_HF_Odd_value);
						RPD_Odd_Storage[c][s][v]->SetBinContent((bin+1), WA_RPD_Odd_value);
						HF_Odd_Storage[c][s][v]->SetBinError((bin+1), WA_HF_Odd_error);
						RPD_Odd_Storage[c][s][v]->SetBinError((bin+1), WA_RPD_Odd_error);
					}

					if(Special_Cuts == true){
						HF_Odd_Storage_Special_Cuts[c][s][v]->SetBinContent((bin+1), WA_HF_Odd_value);
						RPD_Odd_Storage_Special_Cuts[c][s][v]->SetBinContent((bin+1), WA_RPD_Odd_value);
						HF_Odd_Storage_Special_Cuts[c][s][v]->SetBinError((bin+1), WA_HF_Odd_error);
						RPD_Odd_Storage_Special_Cuts[c][s][v]->SetBinError((bin+1), WA_RPD_Odd_error);
					}
				}
				for (int bin = 0; bin < 16 ; bin++){

					Bin_Content_HF_Even_326776[0][bin] = Get_HF_Even_326776 ->GetBinContent(bin+1);  Bin_Content_RPD_Even_326776[0][bin] = Get_RPD_Even_326776 ->GetBinContent(bin+1); 
					Bin_Content_HF_Even_326822[0][bin] = Get_HF_Even_326822 ->GetBinContent(bin+1);  Bin_Content_RPD_Even_326822[0][bin] = Get_RPD_Even_326822 ->GetBinContent(bin+1); 
					Bin_Content_HF_Even_326883[0][bin] = Get_HF_Even_326883 ->GetBinContent(bin+1);  Bin_Content_RPD_Even_326883[0][bin] = Get_RPD_Even_326883 ->GetBinContent(bin+1); 
					Bin_Content_HF_Even_326943[0][bin] = Get_HF_Even_326943 ->GetBinContent(bin+1);  Bin_Content_RPD_Even_326943[0][bin] = Get_RPD_Even_326943 ->GetBinContent(bin+1); 
					Bin_Content_HF_Even_327078[0][bin] = Get_HF_Even_327078 ->GetBinContent(bin+1);  Bin_Content_RPD_Even_327078[0][bin] = Get_RPD_Even_327078 ->GetBinContent(bin+1); 
					Bin_Content_HF_Even_327211[0][bin] = Get_HF_Even_327211 ->GetBinContent(bin+1);  Bin_Content_RPD_Even_327211[0][bin] = Get_RPD_Even_327211 ->GetBinContent(bin+1); 
					Bin_Content_HF_Even_327327[0][bin] = Get_HF_Even_327327 ->GetBinContent(bin+1);  Bin_Content_RPD_Even_327327[0][bin] = Get_RPD_Even_327327 ->GetBinContent(bin+1); 
					Bin_Content_HF_Even_327464[0][bin] = Get_HF_Even_327464 ->GetBinContent(bin+1);  Bin_Content_RPD_Even_327464[0][bin] = Get_RPD_Even_327464 ->GetBinContent(bin+1); 
					Bin_Content_HF_Even_327524[0][bin] = Get_HF_Even_327524 ->GetBinContent(bin+1);  Bin_Content_RPD_Even_327524[0][bin] = Get_RPD_Even_327524 ->GetBinContent(bin+1); 
					Bin_Content_HF_Even_327560[0][bin] = Get_HF_Even_327560 ->GetBinContent(bin+1);  Bin_Content_RPD_Even_327560[0][bin] = Get_RPD_Even_327560 ->GetBinContent(bin+1); 

					Bin_Content_HF_Even_326776[1][bin] = Get_HF_Even_326776 ->GetBinError(bin+1);  Bin_Content_RPD_Even_326776[1][bin] = Get_RPD_Even_326776 ->GetBinError(bin+1); 
					Bin_Content_HF_Even_326822[1][bin] = Get_HF_Even_326822 ->GetBinError(bin+1);  Bin_Content_RPD_Even_326822[1][bin] = Get_RPD_Even_326822 ->GetBinError(bin+1); 
					Bin_Content_HF_Even_326883[1][bin] = Get_HF_Even_326883 ->GetBinError(bin+1);  Bin_Content_RPD_Even_326883[1][bin] = Get_RPD_Even_326883 ->GetBinError(bin+1); 
					Bin_Content_HF_Even_326943[1][bin] = Get_HF_Even_326943 ->GetBinError(bin+1);  Bin_Content_RPD_Even_326943[1][bin] = Get_RPD_Even_326943 ->GetBinError(bin+1); 
					Bin_Content_HF_Even_327078[1][bin] = Get_HF_Even_327078 ->GetBinError(bin+1);  Bin_Content_RPD_Even_327078[1][bin] = Get_RPD_Even_327078 ->GetBinError(bin+1); 
					Bin_Content_HF_Even_327211[1][bin] = Get_HF_Even_327211 ->GetBinError(bin+1);  Bin_Content_RPD_Even_327211[1][bin] = Get_RPD_Even_327211 ->GetBinError(bin+1); 
					Bin_Content_HF_Even_327327[1][bin] = Get_HF_Even_327327 ->GetBinError(bin+1);  Bin_Content_RPD_Even_327327[1][bin] = Get_RPD_Even_327327 ->GetBinError(bin+1); 
					Bin_Content_HF_Even_327464[1][bin] = Get_HF_Even_327464 ->GetBinError(bin+1);  Bin_Content_RPD_Even_327464[1][bin] = Get_RPD_Even_327464 ->GetBinError(bin+1); 
					Bin_Content_HF_Even_327524[1][bin] = Get_HF_Even_327524 ->GetBinError(bin+1);  Bin_Content_RPD_Even_327524[1][bin] = Get_RPD_Even_327524 ->GetBinError(bin+1); 
					Bin_Content_HF_Even_327560[1][bin] = Get_HF_Even_327560 ->GetBinError(bin+1);  Bin_Content_RPD_Even_327560[1][bin] = Get_RPD_Even_327560 ->GetBinError(bin+1); 

					WA_HF_Even_value = 		((Bin_Content_HF_Even_326776[0][bin]*num_events[0] //taking the weighted average to fill the final plots                                  
											+ Bin_Content_HF_Even_326822[0][bin]*num_events[1]                                  
											+ Bin_Content_HF_Even_326883[0][bin]*num_events[2]                                  
											+ Bin_Content_HF_Even_326943[0][bin]*num_events[3]                                  
											+ Bin_Content_HF_Even_327078[0][bin]*num_events[4]                                  
											+ Bin_Content_HF_Even_327211[0][bin]*num_events[5]                                  
											+ Bin_Content_HF_Even_327327[0][bin]*num_events[6]                                  
											+ Bin_Content_HF_Even_327464[0][bin]*num_events[7]                                  
											+ Bin_Content_HF_Even_327524[0][bin]*num_events[8]                                  
											+ Bin_Content_HF_Even_327560[0][bin]*num_events[9])                                  
											/                                  
											(num_events[0]                                  
											 +num_events[1]                                  
											 +num_events[2]                                  
											 +num_events[3]                                  
											 +num_events[4]                                  
											 +num_events[5]                                  
											 +num_events[6]                                  
											 +num_events[7]                                  
											 +num_events[8]                                  
											 +num_events[9]                                  
											));                                  
                              
					WA_RPD_Even_value =  	((Bin_Content_RPD_Even_326776[0][bin]*num_events[0] //taking the weighted average to fill the final plots                                  
											+ Bin_Content_RPD_Even_326822[0][bin]*num_events[1]                                  
											+ Bin_Content_RPD_Even_326883[0][bin]*num_events[2]                                  
											+ Bin_Content_RPD_Even_326943[0][bin]*num_events[3]                                  
											+ Bin_Content_RPD_Even_327078[0][bin]*num_events[4]                                  
											+ Bin_Content_RPD_Even_327211[0][bin]*num_events[5]                                  
											+ Bin_Content_RPD_Even_327327[0][bin]*num_events[6]                                  
											+ Bin_Content_RPD_Even_327464[0][bin]*num_events[7]                                  
											+ Bin_Content_RPD_Even_327524[0][bin]*num_events[8]                                  
											+ Bin_Content_RPD_Even_327560[0][bin]*num_events[9])                                  
											/                                  
											(num_events[0]                                  
											 +num_events[1]                                  
											 +num_events[2]                                  
											 +num_events[3]                                  
											 +num_events[4]                                  
											 +num_events[5]                                  
											 +num_events[6]                                  
											 +num_events[7]                                  
											 +num_events[8]                                  
											 +num_events[9]                                  
											));

					WA_HF_Even_error = 		((Bin_Content_HF_Even_326776[1][bin]*num_events[0] //taking the weighted average to fill the final plots
											+ Bin_Content_HF_Even_326822[1][bin]*num_events[1]
											+ Bin_Content_HF_Even_326883[1][bin]*num_events[2]
											+ Bin_Content_HF_Even_326943[1][bin]*num_events[3]
											+ Bin_Content_HF_Even_327078[1][bin]*num_events[4]
											+ Bin_Content_HF_Even_327211[1][bin]*num_events[5]
											+ Bin_Content_HF_Even_327327[1][bin]*num_events[6]
											+ Bin_Content_HF_Even_327464[1][bin]*num_events[7]
											+ Bin_Content_HF_Even_327524[1][bin]*num_events[8]
											+ Bin_Content_HF_Even_327560[1][bin]*num_events[9])
											/
											(num_events[0]
											 +num_events[1]
											 +num_events[2]
											 +num_events[3]
											 +num_events[4]
											 +num_events[5]
											 +num_events[6]
											 +num_events[7]
											 +num_events[8]
											 +num_events[9]
											));

					WA_RPD_Even_error =  	((Bin_Content_RPD_Even_326776[1][bin]*num_events[0] //taking the weighted average to fill the final plots
											+ Bin_Content_RPD_Even_326822[1][bin]*num_events[1]
											+ Bin_Content_RPD_Even_326883[1][bin]*num_events[2]
											+ Bin_Content_RPD_Even_326943[1][bin]*num_events[3]
											+ Bin_Content_RPD_Even_327078[1][bin]*num_events[4]
											+ Bin_Content_RPD_Even_327211[1][bin]*num_events[5]
											+ Bin_Content_RPD_Even_327327[1][bin]*num_events[6]
											+ Bin_Content_RPD_Even_327464[1][bin]*num_events[7]
											+ Bin_Content_RPD_Even_327524[1][bin]*num_events[8]
											+ Bin_Content_RPD_Even_327560[1][bin]*num_events[9])
											/
											(num_events[0]
											 +num_events[1]
											 +num_events[2]
											 +num_events[3]
											 +num_events[4]
											 +num_events[5]
											 +num_events[6]
											 +num_events[7]
											 +num_events[8]
											 +num_events[9]
											));                                  
					if (Special_Cuts == false){
						HF_Even_Storage[c][s][v]->SetBinContent((bin+1), WA_HF_Even_value);
						RPD_Even_Storage[c][s][v]->SetBinContent((bin+1), WA_RPD_Even_value);
						HF_Even_Storage[c][s][v]->SetBinError((bin+1), WA_HF_Even_error);
						RPD_Even_Storage[c][s][v]->SetBinError((bin+1), WA_RPD_Even_error);
					}
					else if (Special_Cuts == true){
						HF_Even_Storage_Special_Cuts[c][s][v]->SetBinContent((bin+1), WA_HF_Even_value);
						RPD_Even_Storage_Special_Cuts[c][s][v]->SetBinContent((bin+1), WA_RPD_Even_value);
						HF_Even_Storage_Special_Cuts[c][s][v]->SetBinError((bin+1), WA_HF_Even_error);
						RPD_Even_Storage_Special_Cuts[c][s][v]->SetBinError((bin+1), WA_RPD_Even_error);
					}

					//write code to save all the images may want to do inside loop? qand the n i need to make sure everything is present but i should be good then i can do more thesis edits!

				}



				HF_Odd_Storage[c][s][v]->Draw("HIST E");
				D->SaveAs(From("/home/ebadams/Final_Thesis_Code/Run_Summed_Data/%s/%s/HF_Odd_Storage_Cent_%s_Sys_%s_Vert_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str(), Systematics[s].c_str(), Verticies[v].c_str()));
				RPD_Odd_Storage[c][s][v]->Draw("HIST E");
				D->SaveAs(From("/home/ebadams/Final_Thesis_Code/Run_Summed_Data/%s/%s/RPD_Odd_Storage_Cent_%s_Sys_%s_Vert_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str(), Systematics[s].c_str(), Verticies[v].c_str()));
				HF_Odd_Storage[c][s][v]->Draw("HIST E");
				D->SaveAs(From("/home/ebadams/Final_Thesis_Code/Run_Summed_Data/%s/%s/HF_Odd_Storage_Cent_%s_Sys_%s_Vert_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str(), Systematics[s].c_str(), Verticies[v].c_str()));
				RPD_Odd_Storage[c][s][v]->Draw("HIST E");
				D->SaveAs(From("/home/ebadams/Final_Thesis_Code/Run_Summed_Data/%s/%s/RPD_Odd_Storage_Cent_%s_Sys_%s_Vert_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str(), Systematics[s].c_str(), Verticies[v].c_str()));

				HF_Even_Storage[c][s][v]->Draw("HIST E");
				D->SaveAs(From("/home/ebadams/Final_Thesis_Code/Run_Summed_Data/%s/%s/HF_Even_Storage_Cent_%s_Sys_%s_Vert_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str(), Systematics[s].c_str(), Verticies[v].c_str()));
				RPD_Even_Storage[c][s][v]->Draw("HIST E");;
				D->SaveAs(From("/home/ebadams/Final_Thesis_Code/Run_Summed_Data/%s/%s/RPD_Even_Storage_Cent_%s_Sys_%s_Vert_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str(), Systematics[s].c_str(), Verticies[v].c_str()));
				HF_Even_Storage[c][s][v]->Draw("HIST E");;
				D->SaveAs(From("/home/ebadams/Final_Thesis_Code/Run_Summed_Data/%s/%s/HF_Even_Storage_Cent_%s_Sys_%s_Vert_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str(), Systematics[s].c_str(), Verticies[v].c_str()));
				RPD_Even_Storage[c][s][v]->Draw("HIST E");;
				D->SaveAs(From("/home/ebadams/Final_Thesis_Code/Run_Summed_Data/%s/%s/RPD_Even_Storage_Cent_%s_Sys_%s_Vert_%s.root", Systematics[s].c_str(), Verticies[v].c_str(), centralitystring[c].c_str(), Systematics[s].c_str(), Verticies[v].c_str()));

			}
		}
	}	
}