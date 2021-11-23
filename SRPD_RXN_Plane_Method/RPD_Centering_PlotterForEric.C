 //Beginning of Macro
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TH2.h"
#include "TF1.h"
#include "TProfile.h"
#include "TProfile2D.h"
#include "TCanvas.h"
#include "TLeaf.h"
#include "TROOT.h"
#include "TChain.h"
#include <fstream> //for file output
#include <cstring>
#include <iostream>
#include <stdexcept>
#include "TStyle.h"
#include <vector>
#include <numeric>
#include <complex>
#include<iterator>
#include <math.h>
#include <TMath.h>
#include <iomanip>
#include <sstream>
#include <string>
#include <random>
#include <unordered_map>
#include "TGraph.h"
#include "TMultiGraph.h"
#include "TLegend.h"


#include "/home/ebadams/Final_Thesis_Code/Header/MASTER_HEADER_FINAL.h"

//this software is long and complicated but very useful. It analyses the flow vectors nd is used to find the centering values!

/////////////////////////////////
///MY FUNCTIONS IN THIS MACRO///
////////////////////////////////
double pi = 3.14159265359;
bool boolean = true;
void DummyFunction(int runnumber, string Systematics);
void PrettyPlots(int runnumber, string Systematics);

double FunctionShapeTester(double stdev, double mean, double x){
  double returnthis = ((1/stdev) * sqrt(2*pi) ) *  exp( pow((x-mean), 2) / (2*pow(stdev,2)) );
  return returnthis;
}

int runnumber = 0;
//double RPD_Weights[2][16] = {{1.303, 1.342, 1.314, 1.167, 1, 0.9304, 0.8977, 0.9637, 1.631, 1.726, 2.583, 2.189, 1.062, 1.102, 1.093, 1.018}, {0.726 ,1.164 ,1.000 ,0.924 ,1.310 ,1.237 ,1.245 ,1.158 ,0.970 ,0.850 ,0.990 ,1.008 ,0.790 ,0.878 ,0.919 ,0.738}};
//100 ns only

//for function shape tester
  double X_Odd_stddev[10] = {0};
  double X_Odd_mean[10] = {0};
  double X_Even_stddev[10] = {0};
  double X_Even_mean[10] = {0};
  double Y_Odd_stddev[10] = {0};
  double Y_Odd_mean[10] = {0};
  double Y_Even_stddev[10] = {0};
  double Y_Even_mean[10] = {0};
  
  double NEntries_X_Even[10] = {0};
  double NEntries_X_Odd[10] = {0};
  double NEntries_Y_Even[10] = {0};
  double NEntries_Y_Odd[10] = {0};

  std::string EvenStrings[10] = {"OneDTP_Even_X_Y_0_10", "OneDTP_Even_X_Y_10_20", "OneDTP_Even_X_Y_20_30", "OneDTP_Even_X_Y_30_40", "OneDTP_Even_X_Y_40_50", "OneDTP_Even_X_Y_50_60", "OneDTP_Even_X_Y_60_70", "OneDTP_Even_X_Y_70_80", "OneDTP_Even_X_Y_80_90", "OneDTP_Even_X_Y_90_100"};
  std::string OddStrings[10]  = {"OneDTP_Odd_X_Y_0_10", "OneDTP_Odd_X_Y_10_20", "OneDTP_Odd_X_Y_20_30", "OneDTP_Odd_X_Y_30_40", "OneDTP_Odd_X_Y_40_50", "OneDTP_Odd_X_Y_50_60", "OneDTP_Odd_X_Y_60_70", "OneDTP_Odd_X_Y_70_80", "OneDTP_Odd_X_Y_80_90", "OneDTP_Odd_X_Y_90_100"};
//


void MeananSTDDEVRecorder(TProfile* tprofileEven, TProfile* tprofileOdd, int centralityforfunction){ //this func gets the mean and std dev from the deisred plots then uses the values to be fed into a normal distribution plotter

      X_Even_stddev[centralityforfunction] = tprofileEven->GetBinError(1,1);
      X_Even_mean[centralityforfunction] =  tprofileEven->GetBinContent(1,1);

      Y_Even_stddev[centralityforfunction] = tprofileEven->GetBinError(2,1);
      Y_Even_mean[centralityforfunction] = tprofileEven->GetBinContent(2,1);

      X_Odd_stddev[centralityforfunction] = tprofileOdd->GetBinError(1,1);
      X_Odd_mean[centralityforfunction] = tprofileOdd->GetBinContent(1,1);

      Y_Odd_stddev[centralityforfunction] = tprofileOdd->GetBinError(2,1);
      Y_Odd_mean[centralityforfunction] = tprofileOdd->GetBinContent(2,1);

}

//for consumes tprofile
  double PRE_Centering_X_Odd_stddev[10]  = {0};
  double PRE_Centering_X_Odd_mean[10]    = {0};
  double PRE_Centering_X_Even_stddev[10] = {0};
  double PRE_Centering_X_Even_mean[10]   = {0};
  double PRE_Centering_Y_Odd_stddev[10]  = {0};
  double PRE_Centering_Y_Odd_mean[10]    = {0};
  double PRE_Centering_Y_Even_stddev[10] = {0};
  double PRE_Centering_Y_Even_mean[10]   = {0};

//



void Consumes_TProfile(int runnumber, string Systematics){ //thi function consumes the tprofile 2ds made previously and ouls out the mean and stddev to then be used for centering!
  //common storage for file names and just use string for changing to diiferent file names
  //lodd in histograms https://root.cern.ch/input-and-output

  TFile* X_Odd  =  new TFile(Form("/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/%d/Odd/%s/%d_PRE_CENTERING_TP_Odd_X.root", runnumber, Systematics.c_str(), runnumber)); //allocating mem for the TP2D
  TFile* X_Even = new TFile(Form("/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/%d/Even/%s/%d_PRE_CENTERING_TP_Even_X.root", runnumber, Systematics.c_str(),runnumber));
  TFile* Y_Odd  =  new TFile(Form("/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/%d/Odd/%s/%d_PRE_CENTERING_TP_Odd_Y.root", runnumber, Systematics.c_str(), runnumber));
  TFile* Y_Even = new TFile(Form("/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/%d/Even/%s/%d_PRE_CENTERING_TP_Even_Y.root", runnumber, Systematics.c_str(), runnumber));

  
/*  Y_Even->ls();
  Y_Even->cd();
  Y_Even->Draw();
  cin.ignore();*/
  
  TProfile2D* Odd_X  ;
  TProfile2D* Even_X ;
  TProfile2D* Odd_Y  ;
  TProfile2D* Even_Y ;

  Odd_X  =  (TProfile2D*)X_Odd->Get("PRE_X_FlowVectorsOdd_SRPD");//getting the TP2Ds 
  Even_X = (TProfile2D*)X_Even->Get("PRE_X_FlowVectorsEven_SRPD"); 
  Odd_Y  =  (TProfile2D*)Y_Odd->Get("PRE_Y_FlowVectorsOdd_SRPD"); 
  Even_Y = (TProfile2D*)Y_Even->Get("PRE_Y_FlowVectorsEven_SRPD");
  

  Odd_X->Draw(); //drawing the TP2Ds so they can be read
  Even_X->Draw();
  Odd_Y->Draw();
  Even_Y->Draw(); //nothing is drawn unlike the others
/*  cout << "Odd\n";
  Odd_Y->Print("all");
  cin.ignore();

  cout << "Even\n";
  Even_Y->Print("all");
  cin.ignore();*/

  for (int i = 0; i < 10; i++){

     PRE_Centering_X_Odd_stddev[i] = Odd_X->GetBinContent((i+1),2);
       PRE_Centering_X_Odd_mean[i] = Odd_X->GetBinContent((i+1),1);

    PRE_Centering_X_Even_stddev[i] = Even_X->GetBinContent((i+1),2);
      PRE_Centering_X_Even_mean[i] = Even_X->GetBinContent((i+1),1);

     PRE_Centering_Y_Odd_stddev[i] = Odd_Y->GetBinContent((i+1),2);
       PRE_Centering_Y_Odd_mean[i] = Odd_Y->GetBinContent((i+1),1);

    PRE_Centering_Y_Even_stddev[i] = Even_Y->GetBinContent((i+1),2);
      PRE_Centering_Y_Even_mean[i] = Even_Y->GetBinContent((i+1),1);
  }  

}

TH1D *hEP_odd;   
TH1D *hEP_even;  
TH1D *numEvents;
string centralitystring[10] = {"0-10%", "10-20%", "20-30%", "30-40%", "40-50%", "50-60%", "60-70%", "70-80%", "80-90%", "90-100%"};

//1D
auto  OneDTP_Even_X_Y_0_10 = new  TProfile("X_Y_FlowVectorsEvenWholeTracker_0_10 ",Form("Even_X_Y_FlowVectorInformationWholeTracker_%s",centralitystring[0].c_str()),2,0,2,"s(j)");
auto   OneDTP_Odd_X_Y_0_10  = new  TProfile("X_Y_FlowVectorsOddWholeTracker_0_10 ",Form("Odd_X_Y_FlowVectorInformationWholeTracker_%s",centralitystring[0].c_str()),2,0,2,"s(j)");
auto OneDTP_Even_X_Y_10_20 = new TProfile("X_Y_FlowVectorsEvenWholeTracker_10_20 ",Form("Even_X_Y_FlowVectorInformationWholeTracker_%s",centralitystring[1].c_str()),2,0,2,"s(j)");
auto  OneDTP_Odd_X_Y_10_20  = new TProfile("X_Y_FlowVectorsOddWholeTracker_10_20 ",Form("Odd_X_Y_FlowVectorInformationWholeTracker_%s",centralitystring[1].c_str()),2,0,2,"s(j)");
auto OneDTP_Even_X_Y_20_30 = new TProfile("X_Y_FlowVectorsEvenWholeTracker_20_30 ",Form("Even_X_Y_FlowVectorInformationWholeTracker_%s",centralitystring[2].c_str()),2,0,2,"s(j)");
auto  OneDTP_Odd_X_Y_20_30  = new TProfile("X_Y_FlowVectorsOddWholeTracker_20_30 ",Form("Odd_X_Y_FlowVectorInformationWholeTracker_%s",centralitystring[2].c_str()),2,0,2,"s(j)");
auto OneDTP_Even_X_Y_30_40 = new TProfile("X_Y_FlowVectorsEvenWholeTracker_30_40 ",Form("Even_X_Y_FlowVectorInformationWholeTracker_%s",centralitystring[3].c_str()),2,0,2,"s(j)");
auto  OneDTP_Odd_X_Y_30_40  = new TProfile("X_Y_FlowVectorsOddWholeTracker_30_40 ",Form("Odd_X_Y_FlowVectorInformationWholeTracker_%s",centralitystring[3].c_str()),2,0,2,"s(j)");
auto OneDTP_Even_X_Y_40_50 = new TProfile("X_Y_FlowVectorsEvenWholeTracker_40_50 ",Form("Even_X_Y_FlowVectorInformationWholeTracker_%s",centralitystring[4].c_str()),2,0,2,"s(j)");
auto  OneDTP_Odd_X_Y_40_50  = new TProfile("X_Y_FlowVectorsOddWholeTracker_40_50 ",Form("Odd_X_Y_FlowVectorInformationWholeTracker_%s",centralitystring[4].c_str()),2,0,2,"s(j)");
auto OneDTP_Even_X_Y_50_60 = new TProfile("X_Y_FlowVectorsEvenWholeTracker_50_60 ",Form("Even_X_Y_FlowVectorInformationWholeTracker_%s",centralitystring[5].c_str()),2,0,2,"s(j)");
auto  OneDTP_Odd_X_Y_50_60  = new TProfile("X_Y_FlowVectorsOddWholeTracker_50_60 ",Form("Odd_X_Y_FlowVectorInformationWholeTracker_%s",centralitystring[5].c_str()),2,0,2,"s(j)");
auto OneDTP_Even_X_Y_60_70 = new TProfile("X_Y_FlowVectorsEvenWholeTracker_60_70 ",Form("Even_X_Y_FlowVectorInformationWholeTracker_%s",centralitystring[6].c_str()),2,0,2,"s(j)");
auto  OneDTP_Odd_X_Y_60_70  = new TProfile("X_Y_FlowVectorsOddWholeTracker_60_70 ",Form("Odd_X_Y_FlowVectorInformationWholeTracker_%s",centralitystring[6].c_str()),2,0,2,"s(j)");
auto OneDTP_Even_X_Y_70_80 = new TProfile("X_Y_FlowVectorsEvenWholeTracker_70_80 ",Form("Even_X_Y_FlowVectorInformationWholeTracker_%s",centralitystring[7].c_str()),2,0,2,"s(j)");
auto  OneDTP_Odd_X_Y_70_80  = new TProfile("X_Y_FlowVectorsOddWholeTracker_70_80 ",Form("Odd_X_Y_FlowVectorInformationWholeTracker_%s",centralitystring[7].c_str()),2,0,2,"s(j)");
auto OneDTP_Even_X_Y_80_90 = new TProfile("X_Y_FlowVectorsEvenWholeTracker_80_90 ",Form("Even_X_Y_FlowVectorInformationWholeTracker_%s",centralitystring[8].c_str()),2,0,2,"s(j)");
auto  OneDTP_Odd_X_Y_80_90  = new TProfile("X_Y_FlowVectorsOddWholeTracker_80_90 ",Form("Odd_X_Y_FlowVectorInformationWholeTracker_%s",centralitystring[8].c_str()),2,0,2,"s(j)");
auto OneDTP_Even_X_Y_90_100= new TProfile("X_Y_FlowVectorsEvenWholeTracker_90_100",Form("Even_X_Y_FlowVectorInformationWholeTracker_%s",centralitystring[9].c_str()),2,0,2,"s(j)");
auto  OneDTP_Odd_X_Y_90_100 = new TProfile("X_Y_FlowVectorsOddWholeTracker_90_100",Form("Odd_X_Y_FlowVectorInformationWholeTracker_%s",centralitystring[9].c_str()),2,0,2,"s(j)");

std::unordered_map<std::string, TProfile*> TProfileMapEven({ //this allows us to map the plot names with a for loop so I can use a for loop with the function
  {"OneDTP_Even_X_Y_0_10",   OneDTP_Even_X_Y_0_10},
  {"OneDTP_Even_X_Y_10_20",  OneDTP_Even_X_Y_10_20},
  {"OneDTP_Even_X_Y_20_30",  OneDTP_Even_X_Y_20_30},
  {"OneDTP_Even_X_Y_30_40",  OneDTP_Even_X_Y_30_40},
  {"OneDTP_Even_X_Y_40_50",  OneDTP_Even_X_Y_40_50},
  {"OneDTP_Even_X_Y_50_60",  OneDTP_Even_X_Y_50_60},
  {"OneDTP_Even_X_Y_60_70",  OneDTP_Even_X_Y_60_70},
  {"OneDTP_Even_X_Y_70_80",  OneDTP_Even_X_Y_70_80},
  {"OneDTP_Even_X_Y_80_90",  OneDTP_Even_X_Y_80_90},
  {"OneDTP_Even_X_Y_90_100", OneDTP_Even_X_Y_90_100}
});

std::unordered_map<std::string, TProfile*> TProfileMapOdd({
  {"OneDTP_Odd_X_Y_0_10",    OneDTP_Odd_X_Y_0_10},
  {"OneDTP_Odd_X_Y_10_20",   OneDTP_Odd_X_Y_10_20},
  {"OneDTP_Odd_X_Y_20_30",   OneDTP_Odd_X_Y_20_30},
  {"OneDTP_Odd_X_Y_30_40",   OneDTP_Odd_X_Y_30_40},
  {"OneDTP_Odd_X_Y_40_50",   OneDTP_Odd_X_Y_40_50},
  {"OneDTP_Odd_X_Y_50_60",   OneDTP_Odd_X_Y_50_60},
  {"OneDTP_Odd_X_Y_60_70",   OneDTP_Odd_X_Y_60_70},
  {"OneDTP_Odd_X_Y_70_80",   OneDTP_Odd_X_Y_70_80},
  {"OneDTP_Odd_X_Y_80_90",   OneDTP_Odd_X_Y_80_90},
  {"OneDTP_Odd_X_Y_90_100",  OneDTP_Odd_X_Y_90_100}
});

//to access a TProfile
//TProfileMap.at(profile_name)

//MeananSTDDEVRecorder(TProfileMap.at(profile_name))

//2D
auto TP_Even_X = new TProfile2D("X_FlowVectorsEvenWholeTracker","Even_X_FlowVectorInformationWholeTracker_w_CENT; Centrality Bins (10); Top ROW Errors (STDDEV), Bottom Row Means",10,0,10,2,0,2,"s(j)");//new TProfile("Even_X_v_Y","Profile of EVEN X versus Y vectors; X vector; Y vector",1,-2,2,-2,2);
auto TP_Odd_X  = new TProfile2D("X_FlowVectorsOddWholeTracker","Odd_X_FlowVectorInformationWholeTracker_w_CENT; Centrality Bins (10); Top ROW Errors (STDDEV), Bottom Row Means",10,0,10,2,0,2,"s(j)");//new TProfile("Odd_X_v_Y ","Profile of ODD X versus Y vectors; X vector; Y vector" ,1,-2,2,-2,2);
auto TP_Even_Y = new TProfile2D("Y_FlowVectorsEvenWholeTracker","Even_Y_FlowVectorInformationWholeTracker_w_CENT; Centrality Bins (10); Top ROW Errors (STDDEV), Bottom Row Means",10,0,10,2,0,2,"s(j)");//new TProfile("Even_X_v_Y","Profile of EVEN X versus Y vectors; X vector; Y vector",1,-2,2,-2,2);
auto TP_Odd_Y  = new TProfile2D("Y_FlowVectorsOddWholeTracker","Odd_Y_FlowVectorInformationWholeTracker_w_CENT; Centrality Bins (10); Top ROW Errors (STDDEV), Bottom Row Means",10,0,10,2,0,2,"s(j)");//new TProfile("Odd_X_v_Y ","Profile of ODD X versus Y vectors; X vector; Y vector" ,1,-2,2,-2,2);



TH1D *FlowVector_X_Even[10];
TH1D *FlowVector_X_Odd[10];
TH1D *FlowVector_Y_Even[10];
TH1D *FlowVector_Y_Odd[10];

TH1D *ND_X_Even[10]; //ND short for normal distribtuion
TH1D *ND_X_Odd[10];
TH1D *ND_Y_Even[10];
TH1D *ND_Y_Odd[10];

TLegend* X_Odd_legend[10];
TLegend* X_Even_legend[10];
TLegend* Y_Odd_legend[10];
TLegend* Y_Even_legend[10];




double BinError_even_1 = 0;
double BinError_even_2 = 0;
double BinError_odd_1 = 0;
double BinError_odd_2 = 0;

int countEvents = 0;

Int_t RPD_Centering_PlotterForEric(int runnumber, int numSystematics){

  string Systematics;
  if (numSystematics == 0){
    Systematics = "Normal";
  }
  else if (numSystematics == 1){
    Systematics = "LOOSE";
  }
  else if (numSystematics == 2){
    Systematics = "NOMINAL";
  }
  else if (numSystematics == 3){
    Systematics = "TIGHT";
  }
  hEP_odd   = new TH1D(Form("Event_Plane_Angle_Odd_%d", runnumber),Form("EPAngleOdd_%d; Event Plane Angle (Raidans); Count", runnumber),100,-TMath::Pi()-.392699,TMath::Pi()+.392699);
  hEP_even  = new TH1D(Form("Event_Plane_Angle_Even_%d", runnumber),Form("EPAngleEven_%d; Event Plane Angle (Raidans); Count", runnumber),100,-TMath::Pi()-.392699,TMath::Pi()+.392699);  
  numEvents = new TH1D(Form("numevents_%d", runnumber), Form("Number_of_Events_%d", runnumber), 100, 1, 0);
  
  for( int i = 0; i < 10; i++){
  FlowVector_X_Even[i] = new TH1D(Form("FlowVector_X_Even_%d", i),Form("FlowVector_X_Even_%s; Flow Vector Value (A.U.); Count", centralitystring[i].c_str()),100,1,-1);
  FlowVector_X_Odd[i] = new TH1D(Form(" FlowVector_X_Odd_%d", i),Form(" FlowVector_X_Odd_%s; Flow Vector Value (A.U.); Count", centralitystring[i].c_str()),100,1,-1);
  FlowVector_Y_Even[i] = new TH1D(Form("FlowVector_Y_Even_%d", i),Form("FlowVector_Y_Even_%s; Flow Vector Value (A.U.); Count", centralitystring[i].c_str()),100,1,-1);
  FlowVector_Y_Odd[i] = new TH1D(Form(" FlowVector_Y_Odd_%d", i),Form(" FlowVector_Y_Odd_%s; Flow Vector Value (A.U.); Count", centralitystring[i].c_str()),100,1,-1);
  ND_X_Even[i] = new TH1D(Form("ND_X_Even_%d", i),Form("NormalDist_X_Even_%s; Flow Vector Value (A.U.); Count", centralitystring[i].c_str()),100,1,-1);
  ND_X_Odd[i] = new TH1D(Form(" ND_X_Odd_%d", i),Form(" NormalDist_X_Odd_%s; Flow Vector Value (A.U.); Count", centralitystring[i].c_str()),100,1,-1);
  ND_Y_Even[i] = new TH1D(Form("ND_Y_Even_%d", i),Form("NormalDist_Y_Even_%s; Flow Vector Value (A.U.); Count", centralitystring[i].c_str()),100,1,-1);
  ND_Y_Odd[i] = new TH1D(Form(" ND_Y_Odd_%d", i),Form(" NormalDist_Y_Odd_%s; Flow Vector Value (A.U.); Count", centralitystring[i].c_str()),100,1,-1);
  X_Odd_legend[i]  = new TLegend(0.39,0.9,0.61,1.0,"","brNDC");
  X_Even_legend[i] = new TLegend(0.39,0.9,0.61,1.0,"","brNDC");
  Y_Odd_legend[i]  = new TLegend(0.39,0.9,0.61,1.0,"","brNDC");
  Y_Even_legend[i] = new TLegend(0.39,0.9,0.61,1.0,"","brNDC");
  
  }
  //auto hprof  = new TProfile("hprof","Profile of pz versus px",10,-4,4);
  DummyFunction(runnumber, Systematics);
  PrettyPlots(runnumber, Systematics);
  return 0;
}

double RPD_Weights[2][10][16] = {0};
void DummyFunction(int runnumber, string Systematics){

  bool TF = true;

  int NumberOfEvents = 0;
  ////////////////////////////////////////////////////////////////////////////////////
  TFile *f;
  if (runnumber == 3){
    //JOSH PMC
  }
  else {
    if (Systematics == "Normal"){
    f = new TFile(Form("/data2/users/ebadams/Erics_Data_Sets/Complete_%d/Full_Erics_rereco_PbPb2018_AOD_MinBias2_%d_RPDZDC.root",runnumber,runnumber));//(Form("%s",FileandPath.c_str()));
    }
    if (Systematics == "LOOSE"){
      f = new TFile(Form("/data2/users/ebadams/Erics_Data_Sets/Complete_%d/LOOSE_Full_Erics_rereco_PbPb2018_AOD_MinBias2_%d_RPDZDC.root",runnumber,runnumber));//(Form("%s",FileandPath.c_str()));
    }
    if (Systematics == "NOMINAL"){
      f = new TFile(Form("/data2/users/ebadams/Erics_Data_Sets/Complete_%d/NOMINAL_Full_Erics_rereco_PbPb2018_AOD_MinBias2_%d_RPDZDC.root",runnumber,runnumber));//(Form("%s",FileandPath.c_str()));
    }
    if (Systematics == "TIGHT"){
      f = new TFile(Form("/data2/users/ebadams/Erics_Data_Sets/Complete_%d/TIGHT_Full_Erics_rereco_PbPb2018_AOD_MinBias2_%d_RPDZDC.root",runnumber,runnumber));//(Form("%s",FileandPath.c_str()));
    }
  }
  
  TTree* ZDCDigiTree = (TTree*)f->Get("analyzer/zdcdigi");

  ZDCDigiTree->SetBranchStatus("*",0); //turns off all branches
  ZDCDigiTree->SetBranchStatus("nTrack",1); //turnns on branch
  ZDCDigiTree->SetBranchStatus("nAcceptedTracks",1); //turnns on branch
  ZDCDigiTree->SetBranchStatus("Cent",1); //turnns on branch
  ZDCDigiTree->SetBranchStatus("event",1); //turnns on branch
  ZDCDigiTree->SetBranchStatus("run",1); //turnns on branch
  ZDCDigiTree->SetBranchStatus("phi",1); //turnns on branch
  ZDCDigiTree->SetBranchStatus("eta",1); //turnns on branch
  ZDCDigiTree->SetBranchStatus("Pt",1); //turnns on branch

  ZDCDigiTree->SetBranchStatus("zside",1); //turnns on branch
  ZDCDigiTree->SetBranchStatus("section",1); //turnns on branch
  ZDCDigiTree->SetBranchStatus("channel",1); //turnns on branch
  ZDCDigiTree->SetBranchStatus("chi2",1); //turnns on branch
  ZDCDigiTree->SetBranchStatus("nfC0", 1);// turns branch on
  ZDCDigiTree->SetBranchStatus("nfC1", 1);// turns branch on
  ZDCDigiTree->SetBranchStatus("nfC2", 1);// turns branch on
  ZDCDigiTree->SetBranchStatus("nfC3", 1);// turns branch on
  ZDCDigiTree->SetBranchStatus("nfC4", 1);// turns branch on
  ZDCDigiTree->SetBranchStatus("nfC5", 1);// turns branch on
  ZDCDigiTree->SetBranchStatus("nfC6", 1);// turns branch on
  ZDCDigiTree->SetBranchStatus("nfC7", 1);// turns branch on
  ZDCDigiTree->SetBranchStatus("nfC8", 1);// turns branch on
  ZDCDigiTree->SetBranchStatus("nfC9", 1);// turns branch on
  
  TLeaf* eventLeaf = (TLeaf*)ZDCDigiTree->GetLeaf("event");
  TLeaf* zsideLeaf = (TLeaf*)ZDCDigiTree->GetLeaf("zside");
  TLeaf* sectionLeaf = (TLeaf*)ZDCDigiTree->GetLeaf("section");
  TLeaf* channelLeaf = (TLeaf*)ZDCDigiTree->GetLeaf("channel");
  TLeaf* runLeaf = (TLeaf*)ZDCDigiTree->GetLeaf("run");
  TLeaf* AccpTrackLeaf = (TLeaf*)ZDCDigiTree->GetLeaf("nAcceptedTracks");
  TLeaf* CentralityLeaf =(TLeaf*)ZDCDigiTree->GetLeaf("cent");
  TLeaf* fCleaf[NTS];

  for (int iTS = 0; iTS < NTS; iTS++) {
      fCleaf[iTS] = (TLeaf*)ZDCDigiTree->GetLeaf(Form("nfC%d", iTS));
      //fCPureleaf[iTS] = (TLeaf*)ZDCDigiTree->GetLeaf(Form("nfC%d", iTS));
  }

  NumberOfEvents = ZDCDigiTree->GetEntries();

  ZDCDigiTree->GetEntry(0);
  int RunNumber = runLeaf->GetValue(0);

  std::cout<<NumberOfEvents<<std::endl;

  Double_t pi = TMath::Pi();

   int Order = 1;

   //root tree for saving and making the event plane trees
   double X_vector_evn = 0;
   double X_vector_odd = 0;
   double Y_vector_evn = 0;
   double Y_vector_odd = 0;
   int CentralityBinNum, EventNum, run;
   TFile X(Form("/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/%d/Sys_%s_RN_%d_RXN_Plane_VECTORS_Tree_HF_JAIME.root",runnumber, Systematics.c_str(), runnumber), "recreate");
   TTree *newTree = new TTree("RXN_Plane","RXN_Plane_Tree");
   newTree->Branch("X_vector_evn", &X_vector_evn, "X_vector_evn/D");
   newTree->Branch("X_vector_odd", &X_vector_odd, "X_vector_odd/D");
   newTree->Branch("Y_vector_evn", &Y_vector_evn, "Y_vector_evn/D");
   newTree->Branch("Y_vector_odd", &Y_vector_odd, "Y_vector_odd/D");
   newTree->Branch("CentralityBinNum", &CentralityBinNum,"CentralityBinNum/I");
   newTree->Branch("EventNum", &EventNum, "EventNum/I");
   newTree->Branch("run", &run, "run/I");
   // end root tree stuff

  for (Int_t i=0; i< NumberOfEvents; i++){

    if (false){
      BadValue:
      continue;
    }



    ZDCDigiTree->GetEntry(i);

    if (TF){
      runnumber = runLeaf->GetValue(0);

      RPD_Cuts_Generator(runnumber, RPD_Cuts);
      Returns_SRPD_Calibrations(RunNumber,RPD_Weights);

      cout << runnumber << endl;
      Consumes_TProfile(runnumber, Systematics); //gets the values made by the previous program
      TF = false;
    }
    if ( !(i%10000) ) std::cout<<"Events # " << i <<std::endl;

    CentralityValue = CentralityLeaf->GetValue();
    centBin = getHiBinFromhiHF(CentralityValue);
    centIter_Array_Value = Centrality_Array_Chooser(centBin);
    EventNumber = eventLeaf->GetValue();
    EventNum = EventNumber;
    run = runnumber;
    //cout << EventNumber << " EVENT" << endl;
    CentralityBinNum = centBin; //cent value stored for root tree

    ////
    for (int n = 0; n < NChannels; n++) { //iterates through all channels of both ZDC + and -
  
      int side = (int)((zsideLeaf->GetValue(n) + 1) / 2.0);
      int type = (int)(sectionLeaf->GetValue(n)) - 1;
      int channel = (int)(channelLeaf->GetValue(n)) - 1;
    
      double TS_Zero  = (fCleaf[0]->GetValue(n) <= 0) ? 0 : (fCleaf[0]->GetValue(n));
      double TS_One   = (fCleaf[1]->GetValue(n) <= 0) ? 0 : (fCleaf[1]->GetValue(n));
      double TS_Two   = (fCleaf[2]->GetValue(n) <= 0) ? 0 : (fCleaf[2]->GetValue(n));
      double TS_Three = (fCleaf[3]->GetValue(n) <= 0) ? 0 : (fCleaf[3]->GetValue(n));
      double TS_Four  = (fCleaf[4]->GetValue(n) <= 0) ? 0 : (fCleaf[4]->GetValue(n));
      double TS_Five  = (fCleaf[5]->GetValue(n) <= 0) ? 0 : (fCleaf[5]->GetValue(n));
      double TS_Six   = (fCleaf[6]->GetValue(n) <= 0) ? 0 : (fCleaf[6]->GetValue(n));
      double TS_Seven = (fCleaf[7]->GetValue(n) <= 0) ? 0 : (fCleaf[7]->GetValue(n));
      double TS_Eight = (fCleaf[8]->GetValue(n) <= 0) ? 0 : (fCleaf[8]->GetValue(n));
      double TS_Nine  = (fCleaf[9]->GetValue(n) <= 0) ? 0 : (fCleaf[9]->GetValue(n));
    
    
    
      double TS_ARRAY[NTS] = { TS_Zero, TS_One, TS_Two, TS_Three, TS_Four, TS_Five, TS_Six, TS_Seven, TS_Eight, TS_Nine};
    
      if (type == RPD){ // make sure to set cuttoff to 40 fC for RPD
        for (int TS = 0; TS < NTS; TS++){
          RawDataRPD[side][channel][TS] = TS_ARRAY[TS];
        }
      }
    }
    double OC_RPD_Data[2][16] = {0};
  
    Modern_RPD_Data_Organizer_and_Cleaner(RawDataRPD, RPD_Cuts, OC_RPD_Data, runnumber); // cleans and organizes RPD data into a sensible order start top left , read left to right, finish bottom right Quartz blocks

/*    if (OC_RPD_Data[0][0] == -343 or OC_RPD_Data[1][0] == -343){
      continue;
    }*/

   // cout << "yes passed " << endl;
    
  
    double OCC_RPD_Data[2][16] = {0};
  
    for (int s = 0; s < 2; s ++){
      if (OC_RPD_Data[s][0] == -343){ //NOTE this can produce arrays that are defualt filled with zeros need logic to not fill if entire array is zeros
        for ( int c = 0; c < 16; c++){
            OCC_RPD_Data[s][c] = (OC_RPD_Data[s][c]);
        }
        //goto badvalue; // if a bad rpd event is detected it jumps the loop and counts up a bad rpd
      }
      else{
      for ( int c = 0; c < 16; c++){
       //if (c != 1 or c != 10 or c != 11/* or c!= 3 or c!= 7*/){ //3 is tentive but it does make the correlation better
          OCC_RPD_Data[s][c] = (OC_RPD_Data[s][c] * RPD_Weights[s][centIter_Array_Value][c]/** RPD_Frac[s][c]*/);

       // }
       // if (c == 1 or c == 10 or c == 11 /*or c == 3 or c == 7*/){ //3 is tentive but it does make the correlation better
        // OCC_RPD_Data[s][c] = (OC_RPD_Data[0][c] * RPD_Weights[0][c]/** RPD_Frac[0][c]*/);
       // }
      }
     }
    }

    if( OCC_RPD_Data[1][7] > 500 and OCC_RPD_Data[1][15]> 500){
      //do nothing
    }
    else{
      goto BadValue;
    }



       X_vector_odd=0.;
       Y_vector_odd=0.;
       X_vector_evn=0.;
       Y_vector_evn=0.;

    for (int c = 0; c < 16; c++){

      //pos
      //X_vector_odd += (OCC_RPD_Data[1][c] * cos(Order*RPD_Block_In_Phi[c]));
      //Y_vector_odd += (OCC_RPD_Data[1][c] * sin(Order*RPD_Block_In_Phi[c]));
      X_vector_evn += (OCC_RPD_Data[1][c] * cos(Order*RPD_Block_In_Phi[c]));
      Y_vector_evn += (OCC_RPD_Data[1][c] * sin(Order*RPD_Block_In_Phi[c]));

      //neg
      X_vector_odd += ((-1*OCC_RPD_Data[0][c]) * cos(Order*RPD_Block_In_Phi[c]));
      Y_vector_odd += ((-1*OCC_RPD_Data[0][c]) * sin(Order*RPD_Block_In_Phi[c]));
     // X_vector_evn += (OCC_RPD_Data[0][c] * cos(Order*RPD_Block_In_Phi[c]));
     // Y_vector_evn += (OCC_RPD_Data[0][c] * sin(Order*RPD_Block_In_Phi[c]));
    }

    //cout << PRE_Centering_X_Even_mean[centIter_Array_Value] << endl;

    // CENTERING

    X_vector_evn = ((X_vector_evn - PRE_Centering_X_Even_mean[centIter_Array_Value])/PRE_Centering_X_Even_stddev[centIter_Array_Value]);

    X_vector_odd = ((X_vector_odd - PRE_Centering_X_Odd_mean[centIter_Array_Value])/PRE_Centering_X_Odd_stddev[centIter_Array_Value]);

    Y_vector_evn = ((Y_vector_evn - PRE_Centering_Y_Even_mean[centIter_Array_Value])/PRE_Centering_Y_Even_stddev[centIter_Array_Value]);

    Y_vector_odd = ((Y_vector_odd - PRE_Centering_Y_Odd_mean[centIter_Array_Value])/PRE_Centering_Y_Odd_stddev[centIter_Array_Value]);

    //cout << "yes passed " << endl;

    newTree->Fill();
    //

    FlowVector_X_Even[centIter_Array_Value]->Fill(X_vector_evn);
    if (isnan(X_vector_evn)){
      cout << "NAN!!!\n";
      return;
    }
     FlowVector_X_Odd[centIter_Array_Value]->Fill(X_vector_odd);
     FlowVector_Y_Even[centIter_Array_Value]->Fill(Y_vector_evn);
     FlowVector_Y_Odd[centIter_Array_Value]->Fill(Y_vector_odd);

       TP_Odd_X->Fill(centIter_Array_Value+0.2, 0.2,X_vector_odd);
      TP_Even_X->Fill(centIter_Array_Value+0.2, 0.2,X_vector_evn); 
       TP_Odd_Y->Fill(centIter_Array_Value+0.2, 0.2,Y_vector_odd);
      TP_Even_Y->Fill(centIter_Array_Value+0.2, 0.2,Y_vector_evn);

      if (centIter_Array_Value == 0){ //filling one d Tprofile plots based on centrality
       OneDTP_Even_X_Y_0_10->Fill(0.5,X_vector_evn);
        OneDTP_Odd_X_Y_0_10->Fill(0.5,X_vector_odd);
         OneDTP_Even_X_Y_0_10->Fill(1,Y_vector_evn);
          OneDTP_Odd_X_Y_0_10->Fill(1,Y_vector_odd);
      }
      else if (centIter_Array_Value == 1){
      OneDTP_Even_X_Y_10_20->Fill(0.5,X_vector_evn);
       OneDTP_Odd_X_Y_10_20->Fill(0.5,X_vector_odd);
        OneDTP_Even_X_Y_10_20->Fill(1,Y_vector_evn);
         OneDTP_Odd_X_Y_10_20->Fill(1,Y_vector_odd);
      }
      else if (centIter_Array_Value == 2){
      OneDTP_Even_X_Y_20_30->Fill(0.5,X_vector_evn);
       OneDTP_Odd_X_Y_20_30->Fill(0.5,X_vector_odd);
        OneDTP_Even_X_Y_20_30->Fill(1,Y_vector_evn);
         OneDTP_Odd_X_Y_20_30->Fill(1,Y_vector_odd);
      }
      else if (centIter_Array_Value == 3){
      OneDTP_Even_X_Y_30_40->Fill(0.5,X_vector_evn);
       OneDTP_Odd_X_Y_30_40->Fill(0.5,X_vector_odd);
        OneDTP_Even_X_Y_30_40->Fill(1,Y_vector_evn);
         OneDTP_Odd_X_Y_30_40->Fill(1,Y_vector_odd);
      }
      else if (centIter_Array_Value == 4){
      OneDTP_Even_X_Y_40_50->Fill(0.5,X_vector_evn);
       OneDTP_Odd_X_Y_40_50->Fill(0.5,X_vector_odd);
        OneDTP_Even_X_Y_40_50->Fill(1,Y_vector_evn);
         OneDTP_Odd_X_Y_40_50->Fill(1,Y_vector_odd);
      }
      else if (centIter_Array_Value == 5){
      OneDTP_Even_X_Y_50_60->Fill(0.5,X_vector_evn);
       OneDTP_Odd_X_Y_50_60->Fill(0.5,X_vector_odd);
        OneDTP_Even_X_Y_50_60->Fill(1,Y_vector_evn);
         OneDTP_Odd_X_Y_50_60->Fill(1,Y_vector_odd);
      }
      else if (centIter_Array_Value == 6){
      OneDTP_Even_X_Y_60_70->Fill(0.5,X_vector_evn);
       OneDTP_Odd_X_Y_60_70->Fill(0.5,X_vector_odd);
        OneDTP_Even_X_Y_60_70->Fill(1,Y_vector_evn);
         OneDTP_Odd_X_Y_60_70->Fill(1,Y_vector_odd);
      }
      else if (centIter_Array_Value == 7){
      OneDTP_Even_X_Y_70_80->Fill(0.5,X_vector_evn);
       OneDTP_Odd_X_Y_70_80->Fill(0.5,X_vector_odd);
        OneDTP_Even_X_Y_70_80->Fill(1,Y_vector_evn);
         OneDTP_Odd_X_Y_70_80->Fill(1,Y_vector_odd);
      }
      else if(centIter_Array_Value == 8){
      OneDTP_Even_X_Y_80_90->Fill(0.5,X_vector_evn);
       OneDTP_Odd_X_Y_80_90->Fill(0.5,X_vector_odd);
        OneDTP_Even_X_Y_80_90->Fill(1,Y_vector_evn);
         OneDTP_Odd_X_Y_80_90->Fill(1,Y_vector_odd);
      }
      else if(centIter_Array_Value == 9){
      OneDTP_Even_X_Y_90_100->Fill(0.5,X_vector_evn);
       OneDTP_Odd_X_Y_90_100->Fill(0.5,X_vector_odd);
        OneDTP_Even_X_Y_90_100->Fill(1,Y_vector_evn);
         OneDTP_Odd_X_Y_90_100->Fill(1,Y_vector_odd);
      }

       Double_t EPhfodd = -999.;
       EPhfodd=(1./1.)*atan2(Y_vector_odd,X_vector_odd);
       if (EPhfodd>(pi)) EPhfodd=(EPhfodd-(TMath::TwoPi()));
       if (EPhfodd<(-1.0*pi)) EPhfodd=(EPhfodd+(TMath::TwoPi()));
       hEP_odd->Fill(EPhfodd); //rxn plane

       //Odd_RXN_Plane = EPhfodd;

       Double_t EPhfeven = -999.;
       EPhfeven=(1./1.)*atan2(Y_vector_evn,X_vector_evn);
       if (EPhfeven>(pi)) EPhfeven=(EPhfeven-(TMath::TwoPi()));
       if (EPhfeven<(-1.0*pi)) EPhfeven=(EPhfeven+(TMath::TwoPi()));
       hEP_even->Fill(EPhfeven);

      //Even_RXN_Plane = EPhfeven; ///////////////////////////////////////////////////////////////eventplane values
       
      countEvents += 1;

/*      newTree->Fill();*/
      }//End of loop over events

    X.cd();
    newTree->Write(); //finishing root file tree
      
    numEvents->Fill(countEvents);

}//End of the Dummy function 



void PrettyPlots(int runnumber, string Systematics){

  for (int i = 0; i < 10; i++){
    MeananSTDDEVRecorder(TProfileMapEven.at(EvenStrings[i]), TProfileMapOdd.at(OddStrings[i]), i);
  }

 // new TCanvas;
  auto D = new TCanvas("D", "D", 3000, 2000);
  hEP_odd->SetLineColor(kRed-7);
  hEP_odd->Draw("hist e");
  //gPad->SetLogx();
  //gPad->SetLogy();
  //gPad->SetLogz();
  hEP_odd->SetTitle(Form("EP_odd_%d", runnumber));
  gPad->SetTickx();
  gPad->SetTicky();
  gStyle->SetOptStat(0);
  gStyle->SetLineWidth(2);
  //hEP_odd->GetXaxis()->SetRangeUser(.8,100);
  //hEP_odd->GetYaxis()->SetRangeUser(0.08,100);
  hEP_odd->GetXaxis()->SetTitle("#Psi_{1}^{odd} (radians)");
  hEP_odd->GetXaxis()->CenterTitle(1);
  hEP_odd->GetXaxis()->SetTitleFont(42);
  hEP_odd->GetXaxis()->SetTitleSize(0.05);
  hEP_odd->GetXaxis()->SetLabelSize(0.045);
  hEP_odd->GetZaxis()->SetLabelSize(0.045);
  hEP_odd->GetYaxis()->SetTitle("counts/radian");
  hEP_odd->GetYaxis()->CenterTitle(1);
  hEP_odd->GetYaxis()->SetTitleFont(42);
  hEP_odd->GetYaxis()->SetTitleSize(0.05);
  hEP_odd->GetYaxis()->SetLabelSize(0.045);
  D->SaveAs(Form("/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/Sys_%s_EP_odd_%d.png", Systematics.c_str(), runnumber));

  hEP_even->SetLineColor(kRed-7);
  hEP_even->Draw("hist e");
  //gPad->SetLogx();
  //gPad->SetLogy();
  //gPad->SetLogz();
  hEP_even->SetTitle(Form("EP_even_%d", runnumber));
  gPad->SetTickx();
  gPad->SetTicky();
  //gStyle->SetOptStat(0);
  gStyle->SetLineWidth(2);
  //hEP_even->GetXaxis()->SetRangeUser(.8,100);
  //hEP_even->GetYaxis()->SetRangeUser(0.08,100);
  hEP_even->GetXaxis()->SetTitle("#Psi_{1}^{even} (radians)");
  hEP_even->GetXaxis()->CenterTitle(1);
  hEP_even->GetXaxis()->SetTitleFont(42);
  hEP_even->GetXaxis()->SetTitleSize(0.05);
  hEP_even->GetXaxis()->SetLabelSize(0.045);
  hEP_even->GetZaxis()->SetLabelSize(0.045);
  hEP_even->GetYaxis()->SetTitle("counts/radian");
  hEP_even->GetYaxis()->CenterTitle(1);
  hEP_even->GetYaxis()->SetTitleFont(42);
  hEP_even->GetYaxis()->SetTitleSize(0.05);
  hEP_even->GetYaxis()->SetLabelSize(0.045);
  D->SaveAs(Form("/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/Sys_%s_EP_even_%d.png", Systematics.c_str(), runnumber));

  numEvents->Draw();
  D->SaveAs(Form("/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/Sys_%s_NumberOfEvents_%d.png", Systematics.c_str(), runnumber));

  //2D
  
   for (int i = 0; i < 10; i++){
    BinError_even_1 =TP_Even_X->GetBinError(i+1,1);
    TP_Even_X->Fill(i, 1.2,BinError_even_1); //https://root.cern.ch/root/roottalk/roottalk04/0509.html
  }
  TP_Even_X->Draw("text");
  D->SaveAs(Form("/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/%d/Even/%s/POST_TP_Even_X.png", runnumber, Systematics.c_str()));
  TP_Even_X->SaveAs(Form("/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/%d/Even/%s/POST_TP_Even_X.root", runnumber, Systematics.c_str()));

  
  for (int i = 0; i < 10; i++){
    BinError_odd_1 =TP_Odd_X->GetBinError(i+1,1);
    TP_Odd_X->Fill(i, 1.2,BinError_odd_1);
  }
  TP_Odd_X->Draw("text");
  D->SaveAs(Form("/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/%d/Odd/%s/POST_TP_Odd_X.png", runnumber, Systematics.c_str()));
  TP_Odd_X->SaveAs(Form("/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/%d/Odd/%s/POST_TP_Odd_X.root", runnumber, Systematics.c_str()));

  
  for (int i = 0; i < 10; i++){
    BinError_even_1 =TP_Even_Y->GetBinError(i+1,1);
    TP_Even_Y->Fill(i, 1.2,BinError_even_1);
  }
  TP_Even_Y->Draw("text");
  D->SaveAs(Form("/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/%d/Even/%s/POST_TP_Even_Y.png", runnumber, Systematics.c_str()));
  TP_Even_Y->SaveAs(Form("/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/%d/Even/%s/POST_TP_Even_Y.root", runnumber, Systematics.c_str()));
 
  for (int i = 0; i < 10; i++){
    BinError_odd_1 =TP_Odd_Y->GetBinError(i+1,1);
    TP_Odd_Y->Fill(i,1.2,BinError_odd_1);
  }
  TP_Odd_Y->Draw("text");
  D->SaveAs(Form("/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/%d/Odd/%s/POST_TP_Odd_Y.png", runnumber, Systematics.c_str()));
  TP_Odd_Y->SaveAs(Form("/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/%d/Odd/%s/POST_TP_Odd_Y.root", runnumber, Systematics.c_str())); //in essence the fruit of this software is these root files as the values are accessed by a seperate program which uses the histogram to read the values. pretty cleer if i do say so myself.

  //1D
  OneDTP_Even_X_Y_0_10->Draw("TEXT");
  for (int i = 1; i < 3; i++){
    BinError_odd_1 =OneDTP_Even_X_Y_0_10->GetBinError(i,1);
    OneDTP_Even_X_Y_0_10->GetXaxis()->SetBinLabel(i,Form("Error_%f",BinError_odd_1));
  }
  D->SaveAs(Form("/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/%d/Even/%s/Cent_0-10/POST_OneDTP_Even_X_Y_0_10.png",runnumber, Systematics.c_str()));
   OneDTP_Odd_X_Y_0_10->Draw("TEXT");
  for (int i = 1; i < 3; i++){
    BinError_odd_1 =OneDTP_Odd_X_Y_0_10->GetBinError(i,1);
    OneDTP_Odd_X_Y_0_10->GetXaxis()->SetBinLabel(i,Form("Error_%f",BinError_odd_1));
  }
   D->SaveAs(Form("/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/%d/Odd/%s/Cent_0-10/POST_OneDTP_Odd_X_Y_0_10.png",runnumber, Systematics.c_str()));
 OneDTP_Even_X_Y_10_20->Draw("TEXT");
  for (int i = 1; i < 3; i++){
    BinError_odd_1 =OneDTP_Even_X_Y_10_20->GetBinError(i,1);
    OneDTP_Even_X_Y_10_20->GetXaxis()->SetBinLabel(i,Form("Error_%f",BinError_odd_1));
  }
 D->SaveAs(Form("/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/%d/Even/%s/Cent_10-20/POST_OneDTP_Even_X_Y_10_20.png",runnumber, Systematics.c_str()));
  OneDTP_Odd_X_Y_10_20->Draw("TEXT");
  for (int i = 1; i < 3; i++){
    BinError_odd_1 =OneDTP_Odd_X_Y_10_20->GetBinError(i,1);
    OneDTP_Odd_X_Y_10_20->GetXaxis()->SetBinLabel(i,Form("Error_%f",BinError_odd_1));
  }
  D->SaveAs(Form("/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/%d/Odd/%s/Cent_10-20/POST_OneDTP_Odd_X_Y_10_20.png",runnumber, Systematics.c_str()));
 OneDTP_Even_X_Y_20_30->Draw("TEXT");
  for (int i = 1; i < 3; i++){
    BinError_odd_1 =OneDTP_Even_X_Y_20_30->GetBinError(i,1);
    OneDTP_Even_X_Y_20_30->GetXaxis()->SetBinLabel(i,Form("Error_%f",BinError_odd_1));
  }
 D->SaveAs(Form("/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/%d/Even/%s/Cent_20-30/POST_OneDTP_Even_X_Y_20_30.png",runnumber, Systematics.c_str()));
  OneDTP_Odd_X_Y_20_30->Draw("TEXT");
  for (int i = 1; i < 3; i++){
    BinError_odd_1 =OneDTP_Odd_X_Y_20_30->GetBinError(i,1);
    OneDTP_Odd_X_Y_20_30->GetXaxis()->SetBinLabel(i,Form("Error_%f",BinError_odd_1));
  }
  D->SaveAs(Form("/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/%d/Odd/%s/Cent_20-30/POST_OneDTP_Odd_X_Y_20_30.png",runnumber, Systematics.c_str()));
 OneDTP_Even_X_Y_30_40->Draw("TEXT");
  for (int i = 1; i < 3; i++){
    BinError_odd_1 = OneDTP_Even_X_Y_30_40->GetBinError(i,1);
     OneDTP_Even_X_Y_30_40->GetXaxis()->SetBinLabel(i,Form("Error_%f",BinError_odd_1));
  }
 D->SaveAs(Form("/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/%d/Even/%s/Cent_30-40/POST_OneDTP_Even_X_Y_30_40.png",runnumber, Systematics.c_str()));
  OneDTP_Odd_X_Y_30_40->Draw("TEXT");
  for (int i = 1; i < 3; i++){
    BinError_odd_1 =OneDTP_Odd_X_Y_30_40->GetBinError(i,1);
    OneDTP_Odd_X_Y_30_40->GetXaxis()->SetBinLabel(i,Form("Error_%f",BinError_odd_1));
  }
  D->SaveAs(Form("/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/%d/Odd/%s/Cent_30-40/POST_OneDTP_Odd_X_Y_30_40.png",runnumber, Systematics.c_str()));
 OneDTP_Even_X_Y_40_50->Draw("TEXT");
  for (int i = 1; i < 3; i++){
    BinError_odd_1 =OneDTP_Even_X_Y_40_50->GetBinError(i,1);
    OneDTP_Even_X_Y_40_50->GetXaxis()->SetBinLabel(i,Form("Error_%f",BinError_odd_1));
  }
 D->SaveAs(Form("/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/%d/Even/%s/Cent_40-50/POST_OneDTP_Even_X_Y_40_50.png",runnumber, Systematics.c_str()));
  OneDTP_Odd_X_Y_40_50->Draw("TEXT");
  for (int i = 1; i < 3; i++){
    BinError_odd_1 =OneDTP_Odd_X_Y_40_50->GetBinError(i,1);
    OneDTP_Odd_X_Y_40_50->GetXaxis()->SetBinLabel(i,Form("Error_%f",BinError_odd_1));
  }
  D->SaveAs(Form("/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/%d/Odd/%s/Cent_40-50/POST_OneDTP_Odd_X_Y_40_50.png",runnumber, Systematics.c_str()));
 OneDTP_Even_X_Y_50_60->Draw("TEXT");
  for (int i = 1; i < 3; i++){
    BinError_odd_1 =OneDTP_Even_X_Y_50_60->GetBinError(i,1);
    OneDTP_Even_X_Y_50_60->GetXaxis()->SetBinLabel(i,Form("Error_%f",BinError_odd_1));
  }
 D->SaveAs(Form("/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/%d/Even/%s/Cent_50-60/POST_OneDTP_Even_X_Y_50_60.png",runnumber, Systematics.c_str()));
  OneDTP_Odd_X_Y_50_60->Draw("TEXT");
  for (int i = 1; i < 3; i++){
    BinError_odd_1 =OneDTP_Odd_X_Y_50_60->GetBinError(i,1);
    OneDTP_Odd_X_Y_50_60->GetXaxis()->SetBinLabel(i,Form("Error_%f",BinError_odd_1));
  }
  D->SaveAs(Form("/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/%d/Odd/%s/Cent_50-60/OneDTP_Odd_X_Y_50_60.png",runnumber, Systematics.c_str()));
 OneDTP_Even_X_Y_60_70->Draw("TEXT");
  for (int i = 1; i < 3; i++){
    BinError_odd_1 =OneDTP_Even_X_Y_60_70->GetBinError(i,1);
    OneDTP_Even_X_Y_60_70->GetXaxis()->SetBinLabel(i,Form("Error_%f",BinError_odd_1));
  }
 D->SaveAs(Form("/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/%d/Even/%s/Cent_60-70/POST_OneDTP_Even_X_Y_60_70.png",runnumber, Systematics.c_str()));
  OneDTP_Odd_X_Y_60_70->Draw("TEXT");
  for (int i = 1; i < 3; i++){
    BinError_odd_1 =OneDTP_Odd_X_Y_60_70->GetBinError(i,1);
    OneDTP_Odd_X_Y_60_70->GetXaxis()->SetBinLabel(i,Form("Error_%f",BinError_odd_1));
  }
  D->SaveAs(Form("/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/%d/Odd/%s/Cent_60-70/POST_OneDTP_Odd_X_Y_60_70.png",runnumber, Systematics.c_str()));
 OneDTP_Even_X_Y_70_80->Draw("TEXT");
  for (int i = 1; i < 3; i++){
    BinError_odd_1 =OneDTP_Even_X_Y_70_80->GetBinError(i,1);
    OneDTP_Even_X_Y_70_80->GetXaxis()->SetBinLabel(i,Form("Error_%f",BinError_odd_1));
  }
 D->SaveAs(Form("/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/%d/Even/%s/Cent_70-80/POST_OneDTP_Even_X_Y_70_80.png",runnumber, Systematics.c_str()));
  OneDTP_Odd_X_Y_70_80->Draw("TEXT");
  for (int i = 1; i < 3; i++){
    BinError_odd_1 =OneDTP_Odd_X_Y_70_80->GetBinError(i,1);
    OneDTP_Odd_X_Y_70_80->GetXaxis()->SetBinLabel(i,Form("Error_%f",BinError_odd_1));
  }
  D->SaveAs(Form("/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/%d/Odd/%s/Cent_70-80/POST_OneDTP_Odd_X_Y_70_80.png",runnumber, Systematics.c_str()));
 OneDTP_Even_X_Y_80_90->Draw("TEXT");
  for (int i = 1; i < 3; i++){
    BinError_odd_1 = OneDTP_Even_X_Y_80_90->GetBinError(i,1);
     OneDTP_Even_X_Y_80_90->GetXaxis()->SetBinLabel(i,Form("Error_%f",BinError_odd_1));
  }
 D->SaveAs(Form("/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/%d/Even/%s/Cent_80-90/POST_OneDTP_Even_X_Y_80_90.png",runnumber, Systematics.c_str()));
  OneDTP_Odd_X_Y_80_90->Draw("TEXT");
  for (int i = 1; i < 3; i++){
    BinError_odd_1 =OneDTP_Odd_X_Y_80_90->GetBinError(i,1);
    OneDTP_Odd_X_Y_80_90->GetXaxis()->SetBinLabel(i,Form("Error_%f",BinError_odd_1));
  }
  D->SaveAs(Form("/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/%d/Odd/%s/Cent_80-90/POST_OneDTP_Odd_X_Y_80_90.png",runnumber, Systematics.c_str()));
OneDTP_Even_X_Y_90_100->Draw("TEXT");
  for (int i = 1; i < 3; i++){
    BinError_odd_1 =OneDTP_Even_X_Y_90_100->GetBinError(i,1);
    OneDTP_Even_X_Y_90_100->GetXaxis()->SetBinLabel(i,Form("Error_%f",BinError_odd_1));
  }
D->SaveAs(Form("/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/%d/Even/%s/Cent_90-100/POST_OneDTP_Even_X_Y_90_10.png",runnumber, Systematics.c_str()));
 OneDTP_Odd_X_Y_90_100->Draw("TEXT");
  for (int i = 1; i < 3; i++){
    BinError_odd_1 =OneDTP_Odd_X_Y_90_100->GetBinError(i,1);
    OneDTP_Odd_X_Y_90_100->GetXaxis()->SetBinLabel(i,Form("Error_%f",BinError_odd_1));
  }
 D->SaveAs(Form("/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/%d/Odd/%s/Cent_90-100/POST_OneDTP_Odd_X_Y_90_10.png",runnumber, Systematics.c_str()));

  /*ofstream myfile;
  myfile.open ("/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/MeansandSTDDEV.txt");
*/


  for (int i = 0; i <10; i ++){ //loop for ND and flow vector dists

   /* myfile << OddStrings[i] << " stddevX: " << X_Odd_stddev[i] << " meanX: " << X_Odd_mean[i]<< endl;
    myfile << EvenStrings[i] << " stddevX: " << X_Even_stddev[i] << " meanX: " << X_Even_mean[i] << endl;
    myfile << OddStrings[i] << " stddevY: " << Y_Odd_stddev[i] << " meanY: " << Y_Odd_mean[i] << endl;
    myfile << EvenStrings[i] << " stddevY: " << Y_Even_stddev[i] << " meanY: " << Y_Even_mean[i] << endl;
*/
    std::default_random_engine X_Odd_generator; //random number generator which feeds into the normal dist equation which is fed a mean and std dev
    std::default_random_engine X_Even_generator;
    std::default_random_engine Y_Odd_generator;
    std::default_random_engine Y_Even_generator;
    std::normal_distribution<double> X_Odd_distribution(X_Odd_mean[i], X_Odd_stddev[i]);
    std::normal_distribution<double> X_Even_distribution(X_Even_mean[i], X_Even_stddev[i]);
    std::normal_distribution<double> Y_Odd_distribution(Y_Odd_mean[i], Y_Odd_stddev[i]);
    std::normal_distribution<double> Y_Even_distribution(Y_Even_mean[i], Y_Even_stddev[i]);

    X_Odd_legend[i]->SetHeader(Form("Legend %s",centralitystring[i].c_str()),"C");
   X_Even_legend[i]->SetHeader(Form("Legend %s",centralitystring[i].c_str()),"C");
    Y_Odd_legend[i]->SetHeader(Form("Legend %s",centralitystring[i].c_str()),"C");
   Y_Even_legend[i]->SetHeader(Form("Legend %s",centralitystring[i].c_str()),"C");

    D->Divide(2);
    for ( int NE = 0; NE < 100001; NE++){ //this controls the number of events in the normal dist producing function this is the heart of the comparison code
      double X_Odd_value  = X_Odd_distribution(X_Odd_generator);
      double X_Even_value = X_Even_distribution(X_Even_generator);
      double Y_Odd_value  = Y_Odd_distribution(Y_Odd_generator);
      double Y_Even_value = Y_Even_distribution(Y_Even_generator);

      ND_X_Even[i]->Fill(X_Even_value);
      ND_X_Odd[i]->Fill(X_Odd_value);
      ND_Y_Even[i]->Fill(Y_Even_value);
      ND_Y_Odd[i]->Fill(Y_Odd_value);
    }

    /*TP_Even_Y->Draw("colz");*/
    /*D->SaveAs("TP_Even_Y.png");*/
    /* TP_Odd_Y->Draw("colz");*/
    /* D->SaveAs(" TP_Odd_Y.png");*/

    D->Clear();
    D->Divide(2);
    D->cd(1);
    FlowVector_X_Even[i]->SetFillColor(4);
    FlowVector_X_Even[i]->SetFillStyle(3002);
    FlowVector_X_Even[i]->Draw("hist e");
    X_Even_legend[i]->AddEntry(FlowVector_X_Even[i],"Flow Vector Dist.","f");
    D->cd(2);
    ND_X_Even[i]->SetFillColor(kRed-7);
    ND_X_Even[i]->SetFillStyle(3002);
    ND_X_Even[i]->Draw("hist e");
    X_Even_legend[i]->AddEntry(ND_X_Even[i],"Flow Vector Normal Dist.","f");
    D->Update();
    D->cd();
    X_Even_legend[i]->Draw();
    D->SaveAs(Form("/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/%d/Even/%s/POST_FlowVector_X_Even_%s.png",runnumber, Systematics.c_str(), centralitystring[i].c_str()));
    D->Clear();
    D->Divide(2);
    D->cd(1);
    FlowVector_X_Odd[i]->SetFillColor(4);
    FlowVector_X_Odd[i]->SetFillStyle(3002);
    FlowVector_X_Odd[i]->Draw("hist e");
    X_Odd_legend[i]->AddEntry(FlowVector_X_Odd[i],"Flow Vector Dist.","f");
    D->cd(2);
    ND_X_Odd[i]->SetFillColor(kRed-7);
    ND_X_Odd[i]->SetFillStyle(3002);
    ND_X_Odd[i]->Draw("hist e");
    X_Odd_legend[i]->AddEntry(ND_X_Odd[i],"Flow Vector Normal Dist.","f");
    D->Update();
    D->cd();
    X_Odd_legend[i]->Draw();
    D->SaveAs(Form("/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/%d/Odd/%s/POST_FlowVector_X_Odd_%s.png",runnumber, Systematics.c_str(), centralitystring[i].c_str()));
    D->Clear();
    D->Divide(2);
    D->cd(1);
    FlowVector_Y_Even[i]->SetFillColor(4);
    FlowVector_Y_Even[i]->SetFillStyle(3002);
    FlowVector_Y_Even[i]->Draw("hist e");
    Y_Even_legend[i]->AddEntry(FlowVector_Y_Even[i],"Flow Vector Dist.","f");
    D->cd(2);
    ND_Y_Even[i]->SetFillColor(kRed-7);
    ND_Y_Even[i]->SetFillStyle(3002);
    ND_Y_Even[i]->Draw("hist e");
    Y_Even_legend[i]->AddEntry(ND_Y_Even[i],"Flow Vector Normal Dist.","f");
    D->Update();
    D->cd();
    Y_Even_legend[i]->Draw();
    D->SaveAs(Form("/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/%d/Even/%s/POST_FlowVector_Y_Even_%s.png",runnumber, Systematics.c_str(), centralitystring[i].c_str()));
    D->Clear();
    D->Divide(2);
    D->cd(1);
    FlowVector_Y_Odd[i]->SetFillColor(4);
    FlowVector_Y_Odd[i]->SetFillStyle(3002);
    FlowVector_Y_Odd[i]->Draw("hist e");
    Y_Odd_legend[i]->AddEntry(FlowVector_Y_Odd[i],"Flow Vector Dist.","f");
    D->cd(2);
    ND_Y_Odd[i]->SetFillColor(kRed-7);
    ND_Y_Odd[i]->SetFillStyle(3002);
    ND_Y_Odd[i]->Draw("hist e");
    Y_Odd_legend[i]->AddEntry(ND_Y_Odd[i],"Flow Vector Normal Dist.","f");
    D->Update();
    D->cd();
    Y_Odd_legend[i]->Draw();
    D->SaveAs(Form("/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/%d/Odd/%s/POST_FlowVector_Y_Odd_%s.png",runnumber, Systematics.c_str(), centralitystring[i].c_str()));
  }

  //myfile.close();

}//End of the Pretty Plots function