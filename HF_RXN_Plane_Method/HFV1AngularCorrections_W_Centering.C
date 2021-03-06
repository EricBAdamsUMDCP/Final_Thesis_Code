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

void Initialize(int runnumber, string Systematics);
void FourierCorrections(int runnumber, string Systematics);
//Files and chains


/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
////////////           GLOBAL VARIABLES            //////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
Float_t pi=TMath::Pi();
Int_t vterm=1;//Set which order harmonic that this code is meant to measure
Int_t jMax=10;////Set out to which order correction we would like to apply
Int_t NumberOfEvents=0;

Int_t Centrality=0;
Int_t NumberOfHits=0;//This will be for both tracks and Hits
Float_t pT=0.;
Float_t phi=0.;
Float_t eta=0.;
Float_t Energy=0.;

//Create the output ROOT file
TFile *myFile;


const Int_t nCent=9;//Number of Centrality bins
float centlo[nCent] = {0, 10, 20, 30, 40, 50, 60, 70, 80};
float centhi[nCent] = {10, 20, 30, 40, 50, 60, 70, 80, 90};
//The following are lower and upper bounds for the centrality bins
/*centlo[0]=0;  centhi[0]=10;
centlo[1]=10;  centhi[1]=20;
centlo[2]=20;  centhi[2]=30;
centlo[3]=30;  centhi[3]=40;
centlo[4]=40;  centhi[4]=50;
*/

//Make Subdirectories for what will follow
TDirectory *myPlots;//the top level
////////////////////////////////////////////////
TDirectory *midtrackeroddcorrs;
//Angular Correction Folders
TDirectory *angularcorrectionplots;
//Psi1 Odd Corrections
TDirectory *angcorr1odd;
TDirectory *hfoddcorrs;
TDirectory *hfpoddcorrs;
TDirectory *hfnoddcorrs;
/////////////////////////////////
//Psi1 Even Corrections
TDirectory *angcorr1even;
TDirectory *hfevencorrs;
TDirectory *hfpevencorrs;
TDirectory *hfnevencorrs;
/////////////////////////////////
//Looping Variables
//v1 even
Float_t X_hfeven=0.,Y_hfeven=0.;
//v1 odd
Float_t X_hfodd=0.,Y_hfodd=0.;
///Looping Variables
//v1 even
Float_t EPhfeven=0.;
Float_t AngularCorrectionHFEven=0.,EPfinalhfeven=0.;
//v1 odd
Float_t EPhfodd=0.;
Float_t AngularCorrectionHFOdd=0.,EPfinalhfodd=0.;
//PosHFEven
Float_t X_poseven=0.,Y_poseven=0.;
Float_t EP_poseven=0.,EP_finalposeven=0.;
Float_t AngularCorrectionHFPEven=0.;
//PosHFOdd
Float_t X_posodd=0.,Y_posodd=0.;
Float_t EP_posodd=0.,EP_finalposodd=0.;
Float_t AngularCorrectionHFPOdd=0.;
//NegHFEven
Float_t X_negeven=0.,Y_negeven=0.;
Float_t EP_negeven=0.,EP_finalnegeven=0.;
Float_t AngularCorrectionHFNEven=0.;
//NegHFOdd
Float_t X_negodd=0.,Y_negodd=0.;
Float_t EP_negodd=0.,EP_finalnegodd=0.;
Float_t AngularCorrectionHFNOdd=0.;
////////////////////////////////////////////////////
//These Will store the angular correction factors
//v1 even
TProfile *Coshfeven[nCent];
TProfile *Sinhfeven[nCent];
//PosHF
TProfile *Coshfpeven[nCent];
TProfile *Sinhfpeven[nCent];
//NegHF
TProfile *Coshfneven[nCent];
TProfile *Sinhfneven[nCent];
//v1 odd
TProfile *Coshfodd[nCent];
TProfile *Sinhfodd[nCent];
//PosHF
TProfile *Coshfpodd[nCent];
TProfile *Sinhfpodd[nCent];
//NegHF
TProfile *Coshfnodd[nCent];
TProfile *Sinhfnodd[nCent];
//////////////////////////////////////////////////////
Int_t HFV1AngularCorrections_W_Centering(int runnumber, int numSystematics){

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

  Initialize(runnumber, Systematics);
  FourierCorrections(runnumber, Systematics);
  return 0;
}
void Initialize(int runnumber, string Systematics){
  //  std::cout<<"Made it into initialize"<<std::endl;
  Float_t eta_bin_small[13]={-0.6,-0.5,-0.4,-0.3,-0.2,-0.1,0.0,0.1,0.2,0.3,0.4,0.5,0.6};
  Double_t pt_bin[17]={0.4,0.6,0.8,1.0,1.2,1.4,1.6,1.8,2.0,2.4,2.8,3.2,3.6,4.5,6.5,9.5,12};

  //Create the output ROOT file
  myFile = new TFile(Form("/data2/users/ebadams/Eric_HF_RXN_Data_Sets/%d/Sys_%s_RN_%d_JaimeFourierCode.root",runnumber, Systematics.c_str(), runnumber),"recreate");
  //Make Subdirectories for what will follow
  myPlots = myFile->mkdir("Plots");
  myPlots->cd();
  //////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////
  //Angular Correction Folders
  angularcorrectionplots = myPlots->mkdir("FourierCorrectionPlots");
  //Psi1 Corrections
  //Psi1 Even Corrections
  angcorr1even = angularcorrectionplots->mkdir("FirstOrderEPEvenCorrs");
  hfevencorrs = angcorr1even->mkdir("WholeHF");
  hfpevencorrs = angcorr1even->mkdir("PositiveHF");
  hfnevencorrs = angcorr1even->mkdir("NegativeHF");
  ///////////////////////////////////////////////////////////////////////////
  //Psi1 Corrections
  angcorr1odd = angularcorrectionplots->mkdir("FirstOrderEPOddCorrs");
  hfoddcorrs = angcorr1odd->mkdir("WholeHF");
  hfpoddcorrs = angcorr1odd->mkdir("PositiveHF");
  hfnoddcorrs = angcorr1odd->mkdir("NegativeHF");
  midtrackeroddcorrs = angcorr1odd->mkdir("Tracker");
  //<Cos>,<Sin> Psi1(even)
  //Whole HF
  char coshfevenname[128],coshfeventitle[128];
  char sinhfevenname[128],sinhfeventitle[128];
  //Pos HF
  char coshfpevenname[128],coshfpeventitle[128];
  char sinhfpevenname[128],sinhfpeventitle[128];
  //Neg HF
  char coshfnevenname[128],coshfneventitle[128];
  char sinhfnevenname[128],sinhfneventitle[128];
  /////////////////////////////////////////////////
  //<Cos>,<Sin> Psi1(odd)
  char coshfoddname[128],coshfoddtitle[128];
  char sinhfoddname[128],sinhfoddtitle[128];
  //Pos HF
  char coshfpoddname[128],coshfpoddtitle[128];
  char sinhfpoddname[128],sinhfpoddtitle[128];
  //Neg HF
  char coshfnoddname[128],coshfnoddtitle[128];
  char sinhfnoddname[128],sinhfnoddtitle[128];
  ////////////////////////////////////////////////////////////
  //Psi1 Raw, Psi1 Final
  //Psi1(even)
  //Whole HF
  char epevenrawname[128],epevenrawtitle[128];
  char epevenfinalname[128],epevenfinaltitle[128];
  //Pos HF
  char poshfevenrawname[128],poshfevenrawtitle[128];
  char poshfevenfinalname[128],poshfevenfinaltitle[128];
  //Neg HF
  char neghfevenrawname[128],neghfevenrawtitle[128];
  char neghfevenfinalname[128],neghfevenfinaltitle[128];
  //////////////////////////////////////////////////////////////
  //Psi1(odd)
  //Whole HF
  char epoddrawname[128],epoddrawtitle[128];
  char epoddfinalname[128],epoddfinaltitle[128];
  //Pos HF
  char poshfoddrawname[128],poshfoddrawtitle[128];
  char poshfoddfinalname[128],poshfoddfinaltitle[128];
  //Neg HF
  char neghfoddrawname[128],neghfoddrawtitle[128];
  char neghfoddfinalname[128],neghfoddfinaltitle[128];
  //////////////////////////////////////////////////////////////
  //Visualization of Correction Factors
  //Psi1(even)
  char psi1evencorrsname[128],psi1evencorrstitle[128];
  //Psi1(odd)
  char psi1oddcorrsname[128],psi1oddcorrstitle[128];
  /////////////////////////////////////////////////////////////
  //
  //V1 Plots
  //v1(even)
  char v1etaevenname[128],v1etaeventitle[128];
  char v1ptevenname[128],v1pteventitle[128];
  //v1(odd)
  char v1etaoddname[128],v1etaoddtitle[128];
  char v1ptoddname[128],v1ptoddtitle[128];
  for (Int_t i=0;i<nCent;i++)
    {
      ///////////////////////////////
      ////////<cos>,<sin> plots//////
      ///////////////////////////////
      //v1 even
      //Whole HF
      hfevencorrs->cd();
      sprintf(coshfevenname,"CosValues_HFEven_%1.0lfto%1.0lf",centlo[i],centhi[i]);
      sprintf(coshfeventitle,"CosValues_HFEven_%1.0lfto%1.0lf",centlo[i],centhi[i]);
      Coshfeven[i]= new TProfile(coshfevenname,coshfeventitle,jMax,0,jMax);
      Coshfeven[i]->GetYaxis()->SetTitle("<cos(Xbin*#Psi)>");
      sprintf(sinhfevenname,"SinValues_HFEven_%1.0lfto%1.0lf",centlo[i],centhi[i]);
      sprintf(sinhfeventitle,"SinValues_HFEven_%1.0lfto%1.0lf",centlo[i],centhi[i]);
      Sinhfeven[i]= new TProfile(sinhfevenname,sinhfeventitle,jMax,0,jMax);
      Sinhfeven[i]->GetYaxis()->SetTitle("<sin(Xbin*#Psi)>");
      //Pos HF
      hfpevencorrs->cd();
      sprintf(coshfpevenname,"CosValues_HFPEven_%1.0lfto%1.0lf",centlo[i],centhi[i]);
      sprintf(coshfpeventitle,"CosValues_HFPEven_%1.0lfto%1.0lf",centlo[i],centhi[i]);
      Coshfpeven[i]= new TProfile(coshfpevenname,coshfpeventitle,jMax,0,jMax);
      Coshfpeven[i]->GetYaxis()->SetTitle("<cos(Xbin*#Psi)>");
      sprintf(sinhfpevenname,"SinValues_HFPEven_%1.0lfto%1.0lf",centlo[i],centhi[i]);
      sprintf(sinhfpeventitle,"SinValues_HFPEven_%1.0lfto%1.0lf",centlo[i],centhi[i]);
      Sinhfpeven[i]= new TProfile(sinhfpevenname,sinhfpeventitle,jMax,0,jMax);
      Sinhfpeven[i]->GetYaxis()->SetTitle("<sin(Xbin*#Psi)>");
      //Neg HF
      hfnevencorrs->cd();
      sprintf(coshfnevenname,"CosValues_HFNEven_%1.0lfto%1.0lf",centlo[i],centhi[i]);
      sprintf(coshfneventitle,"CosValues_HFNEven_%1.0lfto%1.0lf",centlo[i],centhi[i]);
      Coshfneven[i]= new TProfile(coshfnevenname,coshfneventitle,jMax,0,jMax);
      Coshfneven[i]->GetYaxis()->SetTitle("<cos(Xbin*#Psi)>");
      sprintf(sinhfnevenname,"SinValues_HFNEven_%1.0lfto%1.0lf",centlo[i],centhi[i]);
      sprintf(sinhfneventitle,"SinValues_HFNEven_%1.0lfto%1.0lf",centlo[i],centhi[i]);
      Sinhfneven[i]= new TProfile(sinhfnevenname,sinhfneventitle,jMax,0,jMax);
      Sinhfneven[i]->GetYaxis()->SetTitle("<sin(Xbin*#Psi)>");
      ////////////////////////////////////////////////////////////////////////////////////////////////
      //Psi1 Odd
      hfoddcorrs->cd();
      sprintf(coshfoddname,"CosValues_HFOdd_%1.0lfto%1.0lf",centlo[i],centhi[i]);
      sprintf(coshfoddtitle,"CosValues_HFOdd_%1.0lfto%1.0lf",centlo[i],centhi[i]);
      Coshfodd[i]= new TProfile(coshfoddname,coshfoddtitle,jMax,0,jMax);
      Coshfodd[i]->GetYaxis()->SetTitle("<cos(Xbin*#Psi)>");
      sprintf(sinhfoddname,"SinValues_HFOdd_%1.0lfto%1.0lf",centlo[i],centhi[i]);
      sprintf(sinhfoddtitle,"SinValues_HFOdd_%1.0lfto%1.0lf",centlo[i],centhi[i]);
      Sinhfodd[i]= new TProfile(sinhfoddname,sinhfoddtitle,jMax,0,jMax);
      Sinhfodd[i]->GetYaxis()->SetTitle("<sin(Xbin*#Psi)>");
      //Pos HF
      hfpoddcorrs->cd();
      sprintf(coshfpoddname,"CosValues_HFPOdd_%1.0lfto%1.0lf",centlo[i],centhi[i]);
      sprintf(coshfpoddtitle,"CosValues_HFPOdd_%1.0lfto%1.0lf",centlo[i],centhi[i]);
      Coshfpodd[i]= new TProfile(coshfpoddname,coshfpoddtitle,jMax,0,jMax);
      Coshfpodd[i]->GetYaxis()->SetTitle("<cos(Xbin*#Psi)>");
      sprintf(sinhfpoddname,"SinValues_HFPOdd_%1.0lfto%1.0lf",centlo[i],centhi[i]);
      sprintf(sinhfpoddtitle,"SinValues_HFPOdd_%1.0lfto%1.0lf",centlo[i],centhi[i]);
      Sinhfpodd[i]= new TProfile(sinhfpoddname,sinhfpoddtitle,jMax,0,jMax);
      Sinhfpodd[i]->GetYaxis()->SetTitle("<sin(Xbin*#Psi)>");
      //Neg HF
      hfnoddcorrs->cd();
      sprintf(coshfnoddname,"CosValues_HFNOdd_%1.0lfto%1.0lf",centlo[i],centhi[i]);
      sprintf(coshfnoddtitle,"CosValues_HFNOdd_%1.0lfto%1.0lf",centlo[i],centhi[i]);
      Coshfnodd[i]= new TProfile(coshfnoddname,coshfnoddtitle,jMax,0,jMax);
      Coshfnodd[i]->GetYaxis()->SetTitle("<cos(Xbin*#Psi)>");
      sprintf(sinhfnoddname,"SinValues_HFNOdd_%1.0lfto%1.0lf",centlo[i],centhi[i]);
      sprintf(sinhfnoddtitle,"SinValues_HFNOdd_%1.0lfto%1.0lf",centlo[i],centhi[i]);
      Sinhfnodd[i]= new TProfile(sinhfnoddname,sinhfnoddtitle,jMax,0,jMax);
      Sinhfnodd[i]->GetYaxis()->SetTitle("<sin(Xbin*#Psi)>");
    }//end of loop over centralities
}//end of initialize function


void FourierCorrections(int runnumber, string Systematics){
    TFile *f = new TFile(Form("/data2/users/ebadams/Eric_HF_RXN_Data_Sets/%d/Sys_%s_RN_%d_RXN_Plane_VECTORS_Tree_HF_JAIME.root", runnumber, Systematics.c_str(), runnumber));
   // /data2/users/ebadams/Erics_Data_Sets/Complete_326776/200406_201756/0000/FULLRUN_Erics_rereco_PbPb2018_AOD_MinBias2_326776_RPDZDC.root
    //std::cout<<f<<std::endl;
  
    TTree *t = (TTree*)f->Get("RXN_Plane");

    cout << "test\n";
      //"analyzer/zdcdigi");

/*    vector<double>* hfp_phi_pos = 0;
    vector<double>* hfp_eta_pos = 0;
    vector<double>* hfp_eng_pos = 0;
    vector<double>* hfp_phi_neg = 0;
    vector<double>* hfp_eta_neg = 0;
    vector<double>* hfp_eng_neg = 0;*/
/*
    TLeaf* NHFPos;
    NHFPos = (TLeaf*) t->GetLeaf("nHF_pos");
    //std::cout<<NHFPos<<std::endl;
    TLeaf* NHFNeg;
    NHFNeg = (TLeaf*) t->GetLeaf("nHF_neg");*/
  
    TLeaf* CentralityLeaf =(TLeaf*)t->GetLeaf("CentralityBinNum");
    TLeaf* runLeaf;
    runLeaf = (TLeaf*)t->GetLeaf("run");
  
    TLeaf* eventLeaf = (TLeaf*)t->GetLeaf("EventNum");

    TLeaf* X_vector_evn;
      X_vector_evn = (TLeaf*)t->GetLeaf("X_vector_evn");
    TLeaf* X_vector_odd;
      X_vector_odd = (TLeaf*)t->GetLeaf("X_vector_odd");
    TLeaf* Y_vector_evn;
      Y_vector_evn = (TLeaf*)t->GetLeaf("Y_vector_evn");
    TLeaf* Y_vector_odd;
      Y_vector_odd = (TLeaf*)t->GetLeaf("Y_vector_odd");

      double X_vector_EVEN = 0;
      double X_vector_ODD = 0;
      double Y_vector_EVEN = 0;
      double Y_vector_ODD = 0;
  
/*    t->SetBranchAddress("HFPos_Phi", &hfp_phi_pos);
    t->SetBranchAddress("HFPos_Eta", &hfp_eta_pos);//Kinda useless if u already split pos and neg
    t->SetBranchAddress("HFPos_Energy",  &hfp_eng_pos);//Dont weight with Et, use energy instead
    t->SetBranchAddress("HFNeg_Phi", &hfp_phi_neg);
    t->SetBranchAddress("HFNeg_Eta", &hfp_eta_neg);//Kinda useless if u already split pos and neg
    t->SetBranchAddress("HFNeg_Energy",  &hfp_eng_neg);//Dont weight with Et, use energy instead*/

    Int_t NumberOfEvents = t->GetEntries();

    std::cout<<NumberOfEvents<<std::endl;

    std::ofstream myout;
    myout.open(Form("/data2/users/ebadams/Eric_HF_RXN_Data_Sets/%d/Sys_%s_RN_%d_CosAndSinValues.txt", runnumber, Systematics.c_str(), runnumber)); 


  for (Int_t i=0;i<NumberOfEvents;i++){
      if ( !(i%10000) ) cout << " 2nd round, event # " << i << " / " << NumberOfEvents << endl;
      t->GetEntry(i);

      //CentralityValue = CentralityLeaf->GetValue();
      centBin = CentralityLeaf->GetValue(); //getHiBinFromhiHF(CentralityValue);
      centIter_Array_Value = ALICE_Centrality_Array_Chooser(centBin);

      X_vector_EVEN = X_vector_evn->GetValue();
      X_vector_ODD = X_vector_odd->GetValue();
      Y_vector_EVEN = Y_vector_evn->GetValue();
      Y_vector_ODD = Y_vector_odd->GetValue();


/*      Int_t NumberOfHits_Pos = NHFPos->GetValue();
      Int_t NumberOfHits_Neg = NHFNeg->GetValue();*/

      if (centIter_Array_Value>18) continue;//DONT WANT SUPER PERIPHERAL EVENTS
      //Calo Tower Tree
/*      CaloHits= (TLeaf*) chain2->GetLeaf("Calo_NumberOfHits");
      CaloEN= (TLeaf*) chain2->GetLeaf("Et");
      CaloPhi= (TLeaf*) chain2->GetLeaf("Phi");
      CaloEta= (TLeaf*) chain2->GetLeaf("Eta");*/
      //v1 Even
      //Whole HF
      X_hfeven= X_vector_EVEN;
      Y_hfeven= Y_vector_EVEN;
      //Pos HF
      X_poseven=0.;
      Y_poseven=0.;
      //Neg HF
      X_negeven=0.;
      Y_negeven=0.;
      //v1 Odd
      //Whole HF
      X_hfodd= X_vector_ODD;
      Y_hfodd= Y_vector_ODD;

      //Pos HF
      X_posodd=0.;
      Y_posodd=0.;
      //Neg HF
      X_negodd=0.;
      Y_negodd=0.;
      

/*           for (Int_t ii = 0; ii<NumberOfHits_Pos; ii++){
              Double_t phi    = hfp_phi_pos->at(ii);
              Double_t Energy = hfp_eng_pos->at(ii);
                //          cout << "here1\n";
              //Whole HF Odd
              X_hfodd+=cos(phi)*(Energy);
              Y_hfodd+=sin(phi)*(Energy);
              //Pos HF Odd
              X_posodd+=cos(phi)*(Energy);
              Y_posodd+=sin(phi)*(Energy);
              //Whole HF Even
              X_hfeven+=cos(phi)*(Energy);
              Y_hfeven+=sin(phi)*(Energy);
              //Pos HF Even
              X_poseven+=cos(phi)*(Energy);
              Y_poseven+=sin(phi)*(Energy);
            }
             for (Int_t ii = 0; ii<NumberOfHits_Neg; ii++){
                     //     cout << "here2\n";
              Double_t phi    = hfp_phi_neg->at(ii);
              Double_t Energy = hfp_eng_neg->at(ii);
              //Whole HF Odd
              X_hfodd+=cos(phi)*(-1.0*Energy);
              Y_hfodd+=sin(phi)*(-1.0*Energy);
              //Neg HF Odd
              X_negodd+=cos(phi)*(-1.0*Energy);
              Y_negodd+=sin(phi)*(-1.0*Energy);
              //Whole HF   Even
              X_hfeven+=cos(phi)*(Energy);
              Y_hfeven+=sin(phi)*(Energy);
              // Neg HF Even
              X_negeven+=cos(phi)*(Energy);
              Y_negeven+=sin(phi)*(Energy);
            }
            */

      for (Int_t c=0;c<nCent;c++)
        {
          if ( (centIter_Array_Value*6.3) > centhi[c] ) continue; 
          if ( (centIter_Array_Value*6.3) < centlo[c] ) continue; //3.5
/*
          cout << (centIter_Array_Value*6.3) << endl;
          cout << "centhi[c] " << centhi[c] << " " << " centlo[c] " << centlo[c] << endl;*/
          //Time to fill the appropriate histograms, this will be <cos> <sin>
          //V1 Even
          //Whole HF
          EPhfeven=-999;
          EPhfeven=(1./1.)*atan2(Y_hfeven,X_hfeven);
          if (EPhfeven>(pi)) EPhfeven=(EPhfeven-(TMath::TwoPi()));
          if (EPhfeven<(-1.0*(pi))) EPhfeven=(EPhfeven+(TMath::TwoPi()));
          //Pos HF
/*          EP_poseven=-999;
          EP_poseven=(1./1.)*atan2(Y_poseven,X_poseven);
          if (EP_poseven>(pi)) EP_poseven=(EP_poseven-(TMath::TwoPi()));
          if (EP_poseven<(-1.0*(pi))) EP_poseven=(EP_poseven+(TMath::TwoPi()));*/
          //Neg HF
/*          EP_negeven=-999;
          EP_negeven=(1./1.)*atan2(Y_negeven,X_negeven);
          if (EP_negeven>(pi)) EP_negeven=(EP_negeven-(TMath::TwoPi()));
          if (EP_negeven<(-1.0*(pi))) EP_negeven=(EP_negeven+(TMath::TwoPi()));*/
          //V1 odd
          //Whole HF
          EPhfodd=-999;
          EPhfodd=(1./1.)*atan2(Y_hfodd,X_hfodd);
          if (EPhfodd>(pi)) EPhfodd=(EPhfodd-(TMath::TwoPi()));
          if (EPhfodd<(-1.0*(pi))) EPhfodd=(EPhfodd+(TMath::TwoPi()));
          //Pos HF
/*          EP_posodd=-999;
          EP_posodd=(1./1.)*atan2(Y_posodd,X_posodd);
          if (EP_posodd>(pi)) EP_posodd=(EP_posodd-(TMath::TwoPi()));
          if (EP_posodd<(-1.0*(pi))) EP_posodd=(EP_posodd+(TMath::TwoPi()));*/
          //Neg HF
/*          EP_negodd=-999;
          EP_negodd=(1./1.)*atan2(Y_negodd,X_negodd);
          if (EP_negodd>(pi)) EP_negodd=(EP_negodd-(TMath::TwoPi()));
          if (EP_negodd<(-1.0*(pi))) EP_negodd=(EP_negodd+(TMath::TwoPi()));*/


          if((EPhfeven>-500) && (EPhfodd>-500))
            {
              for (int k=1;k<(jMax+1);k++)
                {
                  //v1 odd
                  //Whole HF
                 // cout << "c = " << c << endl;
                  Coshfodd[c]->Fill(k-1,cos(k*EPhfodd)); //
                  Sinhfodd[c]->Fill(k-1,sin(k*EPhfodd)); //
                  //Pos HF
                  Coshfpodd[c]->Fill(k-1,cos(k*EP_posodd));
                  Sinhfpodd[c]->Fill(k-1,sin(k*EP_posodd));
                  //Neg HF
                  Coshfnodd[c]->Fill(k-1,cos(k*EP_negodd));
                  Sinhfnodd[c]->Fill(k-1,sin(k*EP_negodd));
                  /////////////////////////////////////////////////////////
                  //v1 even
                  //Whole HF
                  Coshfeven[c]->Fill(k-1,cos(k*EPhfeven)); //
                  Sinhfeven[c]->Fill(k-1,sin(k*EPhfeven));//
                  //Pos HF
                  Coshfpeven[c]->Fill(k-1,cos(k*EP_poseven));
                  Sinhfpeven[c]->Fill(k-1,sin(k*EP_poseven));
                  //Neg HF
                  Coshfneven[c]->Fill(k-1,cos(k*EP_negeven));
                  Sinhfneven[c]->Fill(k-1,sin(k*EP_negeven));
                }//end of loop over K
            }//preventing empty EP
        }//End of loop over centralities
    }//end of loop over events
      myout<<"//V1 Even"<<endl;
      myout<<"//Whole HF"<<endl;
      for (Int_t k=1;k<=10;k++)
    {
      myout<<"Coshfeven[0]["<<k-1<<"]="<<Coshfeven[0]->GetBinContent(k)<<";"<<endl;
      myout<<"Coshfeven[1]["<<k-1<<"]="<<Coshfeven[1]->GetBinContent(k)<<";"<<endl;
      myout<<"Coshfeven[2]["<<k-1<<"]="<<Coshfeven[2]->GetBinContent(k)<<";"<<endl;
      myout<<"Coshfeven[3]["<<k-1<<"]="<<Coshfeven[3]->GetBinContent(k)<<";"<<endl;
      myout<<"Coshfeven[4]["<<k-1<<"]="<<Coshfeven[4]->GetBinContent(k)<<";"<<endl;
      myout<<"Coshfeven[5]["<<k-1<<"]="<<Coshfeven[5]->GetBinContent(k)<<";"<<endl;
      myout<<"Coshfeven[6]["<<k-1<<"]="<<Coshfeven[6]->GetBinContent(k)<<";"<<endl;
      myout<<"Coshfeven[7]["<<k-1<<"]="<<Coshfeven[7]->GetBinContent(k)<<";"<<endl;
      myout<<"Coshfeven[8]["<<k-1<<"]="<<Coshfeven[8]->GetBinContent(k)<<";"<<endl;
      myout<<" "<<endl;
      myout<<"Sinhfeven[0]["<<k-1<<"]="<<Sinhfeven[0]->GetBinContent(k)<<";"<<endl;
      myout<<"Sinhfeven[1]["<<k-1<<"]="<<Sinhfeven[1]->GetBinContent(k)<<";"<<endl;
      myout<<"Sinhfeven[2]["<<k-1<<"]="<<Sinhfeven[2]->GetBinContent(k)<<";"<<endl;
      myout<<"Sinhfeven[3]["<<k-1<<"]="<<Sinhfeven[3]->GetBinContent(k)<<";"<<endl;
      myout<<"Sinhfeven[4]["<<k-1<<"]="<<Sinhfeven[4]->GetBinContent(k)<<";"<<endl;
      myout<<"Sinhfeven[5]["<<k-1<<"]="<<Sinhfeven[5]->GetBinContent(k)<<";"<<endl;
      myout<<"Sinhfeven[6]["<<k-1<<"]="<<Sinhfeven[6]->GetBinContent(k)<<";"<<endl;
      myout<<"Sinhfeven[7]["<<k-1<<"]="<<Sinhfeven[7]->GetBinContent(k)<<";"<<endl;
      myout<<"Sinhfeven[8]["<<k-1<<"]="<<Sinhfeven[8]->GetBinContent(k)<<";"<<endl;
      myout<<" "<<endl;      
    }//end of loop over k

  myout<<" "<<endl;
  myout<<"//Pos HF"<<endl;

  for (Int_t k=1;k<=10;k++)
    {
      myout<<"Coshfpeven[0]["<<k-1<<"]="<<Coshfpeven[0]->GetBinContent(k)<<";"<<endl;
      myout<<"Coshfpeven[1]["<<k-1<<"]="<<Coshfpeven[1]->GetBinContent(k)<<";"<<endl;
      myout<<"Coshfpeven[2]["<<k-1<<"]="<<Coshfpeven[2]->GetBinContent(k)<<";"<<endl;
      myout<<"Coshfpeven[3]["<<k-1<<"]="<<Coshfpeven[3]->GetBinContent(k)<<";"<<endl;
      myout<<"Coshfpeven[4]["<<k-1<<"]="<<Coshfpeven[4]->GetBinContent(k)<<";"<<endl;
      myout<<" "<<endl;
      myout<<"Sinhfpeven[0]["<<k-1<<"]="<<Sinhfpeven[0]->GetBinContent(k)<<";"<<endl;
      myout<<"Sinhfpeven[1]["<<k-1<<"]="<<Sinhfpeven[1]->GetBinContent(k)<<";"<<endl;
      myout<<"Sinhfpeven[2]["<<k-1<<"]="<<Sinhfpeven[2]->GetBinContent(k)<<";"<<endl;
      myout<<"Sinhfpeven[3]["<<k-1<<"]="<<Sinhfpeven[3]->GetBinContent(k)<<";"<<endl;
      myout<<"Sinhfpeven[4]["<<k-1<<"]="<<Sinhfpeven[4]->GetBinContent(k)<<";"<<endl;
      myout<<" "<<endl;
    }//end of loop over k  

     myout<<" "<<endl;
  myout<<"//Neg HF"<<endl;

  for (Int_t k=1;k<=10;k++)
    {
      myout<<"Coshfneven[0]["<<k-1<<"]="<<Coshfneven[0]->GetBinContent(k)<<";"<<endl;
      myout<<"Coshfneven[1]["<<k-1<<"]="<<Coshfneven[1]->GetBinContent(k)<<";"<<endl;
      myout<<"Coshfneven[2]["<<k-1<<"]="<<Coshfneven[2]->GetBinContent(k)<<";"<<endl;
      myout<<"Coshfneven[3]["<<k-1<<"]="<<Coshfneven[3]->GetBinContent(k)<<";"<<endl;
      myout<<"Coshfneven[4]["<<k-1<<"]="<<Coshfneven[4]->GetBinContent(k)<<";"<<endl;
      myout<<" "<<endl;
      myout<<"Sinhfneven[0]["<<k-1<<"]="<<Sinhfneven[0]->GetBinContent(k)<<";"<<endl;
      myout<<"Sinhfneven[1]["<<k-1<<"]="<<Sinhfneven[1]->GetBinContent(k)<<";"<<endl;
      myout<<"Sinhfneven[2]["<<k-1<<"]="<<Sinhfneven[2]->GetBinContent(k)<<";"<<endl;
      myout<<"Sinhfneven[3]["<<k-1<<"]="<<Sinhfneven[3]->GetBinContent(k)<<";"<<endl;
      myout<<"Sinhfneven[4]["<<k-1<<"]="<<Sinhfneven[4]->GetBinContent(k)<<";"<<endl;
      myout<<" "<<endl;
    }//end of loop over k   

    myout<<"//V1 Odd"<<endl;
    myout<<"//Whole HF"<<endl;

  
  for (Int_t k=1;k<=10;k++)
    {
      myout<<"Coshfodd[0]["<<k-1<<"]="<<Coshfodd[0]->GetBinContent(k)<<";"<<endl;
      myout<<"Coshfodd[1]["<<k-1<<"]="<<Coshfodd[1]->GetBinContent(k)<<";"<<endl;
      myout<<"Coshfodd[2]["<<k-1<<"]="<<Coshfodd[2]->GetBinContent(k)<<";"<<endl;
      myout<<"Coshfodd[3]["<<k-1<<"]="<<Coshfodd[3]->GetBinContent(k)<<";"<<endl;
      myout<<"Coshfodd[4]["<<k-1<<"]="<<Coshfodd[4]->GetBinContent(k)<<";"<<endl;
      myout<<"Coshfodd[5]["<<k-1<<"]="<<Coshfodd[5]->GetBinContent(k)<<";"<<endl;
      myout<<"Coshfodd[6]["<<k-1<<"]="<<Coshfodd[6]->GetBinContent(k)<<";"<<endl;
      myout<<"Coshfodd[7]["<<k-1<<"]="<<Coshfodd[7]->GetBinContent(k)<<";"<<endl;
      myout<<"Coshfodd[8]["<<k-1<<"]="<<Coshfodd[8]->GetBinContent(k)<<";"<<endl;
      myout<<" "<<endl;
      myout<<"Sinhfodd[0]["<<k-1<<"]="<<Sinhfodd[0]->GetBinContent(k)<<";"<<endl;
      myout<<"Sinhfodd[1]["<<k-1<<"]="<<Sinhfodd[1]->GetBinContent(k)<<";"<<endl;
      myout<<"Sinhfodd[2]["<<k-1<<"]="<<Sinhfodd[2]->GetBinContent(k)<<";"<<endl;
      myout<<"Sinhfodd[3]["<<k-1<<"]="<<Sinhfodd[3]->GetBinContent(k)<<";"<<endl;
      myout<<"Sinhfodd[4]["<<k-1<<"]="<<Sinhfodd[4]->GetBinContent(k)<<";"<<endl;
      myout<<"Sinhfodd[5]["<<k-1<<"]="<<Sinhfodd[5]->GetBinContent(k)<<";"<<endl;
      myout<<"Sinhfodd[6]["<<k-1<<"]="<<Sinhfodd[6]->GetBinContent(k)<<";"<<endl;
      myout<<"Sinhfodd[7]["<<k-1<<"]="<<Sinhfodd[7]->GetBinContent(k)<<";"<<endl;
      myout<<"Sinhfodd[8]["<<k-1<<"]="<<Sinhfodd[8]->GetBinContent(k)<<";"<<endl;
      myout<<" "<<endl;      
    }//end of loop over k

      myout<<" "<<endl;
  myout<<"//Pos HF"<<endl;


  for (Int_t k=1;k<=10;k++)
    {
      myout<<"Coshfpodd[0]["<<k-1<<"]="<<Coshfpodd[0]->GetBinContent(k)<<";"<<endl;
      myout<<"Coshfpodd[1]["<<k-1<<"]="<<Coshfpodd[1]->GetBinContent(k)<<";"<<endl;
      myout<<"Coshfpodd[2]["<<k-1<<"]="<<Coshfpodd[2]->GetBinContent(k)<<";"<<endl;
      myout<<"Coshfpodd[3]["<<k-1<<"]="<<Coshfpodd[3]->GetBinContent(k)<<";"<<endl;
      myout<<"Coshfpodd[4]["<<k-1<<"]="<<Coshfpodd[4]->GetBinContent(k)<<";"<<endl;
      myout<<" "<<endl;
      myout<<"Sinhfpodd[0]["<<k-1<<"]="<<Sinhfpodd[0]->GetBinContent(k)<<";"<<endl;
      myout<<"Sinhfpodd[1]["<<k-1<<"]="<<Sinhfpodd[1]->GetBinContent(k)<<";"<<endl;
      myout<<"Sinhfpodd[2]["<<k-1<<"]="<<Sinhfpodd[2]->GetBinContent(k)<<";"<<endl;
      myout<<"Sinhfpodd[3]["<<k-1<<"]="<<Sinhfpodd[3]->GetBinContent(k)<<";"<<endl;
      myout<<"Sinhfpodd[4]["<<k-1<<"]="<<Sinhfpodd[4]->GetBinContent(k)<<";"<<endl;
      myout<<" "<<endl;
    }//end of loop over k  


      myout<<" "<<endl;
  myout<<"//Neg HF"<<endl;


  for (Int_t k=1;k<=10;k++)
    {
      myout<<"Coshfnodd[0]["<<k-1<<"]="<<Coshfnodd[0]->GetBinContent(k)<<";"<<endl;
      myout<<"Coshfnodd[1]["<<k-1<<"]="<<Coshfnodd[1]->GetBinContent(k)<<";"<<endl;
      myout<<"Coshfnodd[2]["<<k-1<<"]="<<Coshfnodd[2]->GetBinContent(k)<<";"<<endl;
      myout<<"Coshfnodd[3]["<<k-1<<"]="<<Coshfnodd[3]->GetBinContent(k)<<";"<<endl;
      myout<<"Coshfnodd[4]["<<k-1<<"]="<<Coshfnodd[4]->GetBinContent(k)<<";"<<endl;
      myout<<" "<<endl;
      myout<<"Sinhfnodd[0]["<<k-1<<"]="<<Sinhfnodd[0]->GetBinContent(k)<<";"<<endl;
      myout<<"Sinhfnodd[1]["<<k-1<<"]="<<Sinhfnodd[1]->GetBinContent(k)<<";"<<endl;
      myout<<"Sinhfnodd[2]["<<k-1<<"]="<<Sinhfnodd[2]->GetBinContent(k)<<";"<<endl;
      myout<<"Sinhfnodd[3]["<<k-1<<"]="<<Sinhfnodd[3]->GetBinContent(k)<<";"<<endl;
      myout<<"Sinhfnodd[4]["<<k-1<<"]="<<Sinhfnodd[4]->GetBinContent(k)<<";"<<endl;
      myout<<" "<<endl;
    }//end of loop over k     
  

  myFile->Write();

  myout.close();
}//End of Angular Corrections Function
