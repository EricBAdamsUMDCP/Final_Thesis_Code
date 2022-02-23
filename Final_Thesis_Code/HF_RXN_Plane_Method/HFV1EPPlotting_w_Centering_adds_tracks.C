#include "TH1F.h"
#include "TProfile.h"
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include "TFile.h"
#include "TTree.h"
#include "TLeaf.h"

#include "TMath.h"
 
#include "/home/ebadams/TEST_FOR_ANALYSIS_SOFTWARE/HEADER/MASTER_HEADER_fixed_fourier.h"

void Initialize(int runnumber, string Systematics);
void FillAngularCorrections(int runnumber, string Systematics);
void EPPlotting(int runnumber, string Systematics);


using namespace std;

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
////////////           GLOBAL VARIABLES            //////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
Float_t pi=TMath::Pi();
Int_t vterm=1;//Set which order harmonic that this code is meant to measure
const Int_t jMax=10;////Set out to which order correction we would like to apply
Int_t NumberOfEvents=0;
Int_t Centrality=0;
//  NumberOfEvents = chain->GetEntries();

///Looping Variables
Int_t NumberOfHits=0;//This will be for both tracks and Hits
/*Float_t pT=0.;
Float_t phi=0.;
Float_t eta=0.;
Float_t Energy=0.;*/


//Create the output ROOT file
TFile *myFile;

const Int_t nCent=9;//Number of Centrality classes

float centlo[nCent] = {0, 10, 20, 30, 40, 50, 60, 70, 80};
float centhi[nCent] = {10, 20, 30, 40, 50, 60, 70, 80, 90};
/*centlo[0]=0;  centhi[0]=10;
centlo[1]=10;  centhi[1]=20;
centlo[2]=20;  centhi[2]=30;
centlo[3]=30;  centhi[3]=40;
centlo[4]=40;  centhi[4]=50;
*/


double Even_Flattened_RXNP = 0;
double Odd_Flattened_RXNP  = 0;
int numberoftracks = 0;
int numberofAcceptracks = 0;
int CentralityBinNum, EventNum, run;
double vtx_ = 0;


//Make Subdirectories for what will follow
TDirectory *myPlots;//the top level

TDirectory *epangles;//where i will store the ep angles
TDirectory *wholehfepangles;
////////////////////////////////////////////////

/////////////////////////////////


     Float_t X_poseven=0.;
     Float_t Y_poseven=0.;
     Float_t X_negeven=0.;
     Float_t Y_negeven=0.;
     Float_t X_posodd=0.;
     Float_t Y_posodd=0.;
     Float_t X_negodd=0.;
     Float_t Y_negodd=0.;

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


//<Cos> <Sin>
                  //v1 even
                  //Whole HF
                  Float_t Sinhfeven[nCent][jMax],Coshfeven[nCent][jMax];
                  //Pos HF
                  Float_t Sinhfpeven[nCent][jMax],Coshfpeven[nCent][jMax];
                  //Neg HF
                  Float_t Sinhfneven[nCent][jMax],Coshfneven[nCent][jMax];
                  //Tracker
                 // Float_t Sintreven[nCent][jMax],Costreven[nCent][jMax];
                  
                  //v1 odd
                  //Whole HF
                  Float_t Sinhfodd[nCent][jMax],Coshfodd[nCent][jMax];
                  //Pos HF
                  Float_t Sinhfpodd[nCent][jMax],Coshfpodd[nCent][jMax];
                  //Neg HF
                  Float_t Sinhfnodd[nCent][jMax],Coshfnodd[nCent][jMax];
                  //Tracker
                 // Float_t Sintrodd[nCent][jMax],Costrodd[nCent][jMax];

//////////////////////////////////////////////////////

////////////////////////////////////////////
//Final EP Plots
//Psi1Even
//Whole HF
TH1F *PsiEvenRaw[nCent];
TH1F *PsiEvenFinal[nCent];

//////////////////////////////////////////////
//Psi1 Odd
//Whole HF
TH1F *PsiOddRaw[nCent];
TH1F *PsiOddFinal[nCent];
///////////////////////////////////////////////

//Average Corrections
TProfile *PsiOddCorrs[nCent];
TProfile *PsiEvenCorrs[nCent];

//////////////////////////////////
//addable histos

TH1D* Flattened_Odd;
TH1D* Flattened_Even;
//for adding tracks to root file

std::vector<double>* phi_storage_vector;
//std::vector<double>* phiError_storage_vector;
std::vector<double>* eta_storage_vector;
//std::vector<double>* etaError_storage_vector;
std::vector<double>* Pt_storage_vector;
//std::vector<double>* ptError_storage_vector;
//std::vector<double>* chi2_storage_vector;

std::vector<double>* phi = 0;
//std::vector<double>* phiError = 0;
std::vector<double>* eta = 0;
//std::vector<double>* etaError = 0;
std::vector<double>* Pt = 0;
//std::vector<double>* ptError = 0;
//std::vector<double>* chi2 = 0;



//////////////////////////////////

Int_t HFV1EPPlotting_w_Centering_adds_tracks(int runnumber, int numSystematics){

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
  FillAngularCorrections(runnumber, Systematics);
  EPPlotting(runnumber, Systematics);
  return 0;
}


void Initialize(int runnumber, string Systematics){

  Flattened_Odd  = new TH1D(Form("Histo_Odd_Flat_RXN_Plane_%d", runnumber),Form("RXN_Planes_%d_Odd; Reaction Plane Angle Odd Flattened (#Phi, Radians); Count", runnumber),100,-3.5,3.5);
  Flattened_Even = new TH1D(Form("Histo_Even_Flat_RXN_Plane_%d", runnumber),Form("RXN_Planes_%d_Even; Reaction Plane Angle Even Flattened (#Phi, Radians); Count", runnumber),100,-3.5,3.5);

  Flattened_Odd->GetXaxis()->SetTitle("#Psi_{1}^{odd} (radians)");
  Flattened_Odd->GetXaxis()->CenterTitle(1);
  Flattened_Odd->GetXaxis()->SetTitleFont(42);
  Flattened_Odd->GetXaxis()->SetTitleSize(0.05);
  Flattened_Odd->GetXaxis()->SetLabelSize(0.045);
  Flattened_Odd->GetZaxis()->SetLabelSize(0.045);
  Flattened_Odd->GetYaxis()->SetTitle("counts/radian");
  Flattened_Odd->GetYaxis()->CenterTitle(1);
  Flattened_Odd->GetYaxis()->SetTitleFont(42);
  Flattened_Odd->GetYaxis()->SetTitleSize(0.05);
  Flattened_Odd->GetYaxis()->SetLabelSize(0.045);

  Flattened_Even->GetXaxis()->SetTitle("#Psi_{1}^{even} (radians)");
  Flattened_Even->GetXaxis()->CenterTitle(1);
  Flattened_Even->GetXaxis()->SetTitleFont(42);
  Flattened_Even->GetXaxis()->SetTitleSize(0.05);
  Flattened_Even->GetXaxis()->SetLabelSize(0.045);
  Flattened_Even->GetZaxis()->SetLabelSize(0.045);
  Flattened_Even->GetYaxis()->SetTitle("counts/radian");
  Flattened_Even->GetYaxis()->CenterTitle(1);
  Flattened_Even->GetYaxis()->SetTitleFont(42);
  Flattened_Even->GetYaxis()->SetTitleSize(0.05);
  Flattened_Even->GetYaxis()->SetLabelSize(0.045);

  phi_storage_vector = nullptr;
 // phiError_storage_vector = nullptr;
  eta_storage_vector = nullptr;
 // etaError_storage_vector = nullptr;
  Pt_storage_vector = nullptr;
  //ptError_storage_vector = nullptr;
  //chi2_storage_vector = nullptr;


  
  Float_t eta_bin_small[7]={-1.5,-1.0,-0.5,0.0,0.5,1.0,1.5};
  Double_t pt_bin[17]={0.4,0.6,0.8,1.0,1.2,1.4,1.6,1.8,2.0,2.4,2.8,3.2,3.6,4.5,6.5,9.5,12};

  //Create the output ROOT >>file<<
  myFile = new TFile(Form("/data2/users/ebadams/Eric_HF_RXN_Data_Sets/%d/Sys_%s_EP_RN_%d_fourierflattened.root",runnumber, Systematics.c_str(), runnumber),"recreate");

  //create output root >>tree<<

 // newTree->Branch("ptError_storage_vector",  "std::vector<double>", &ptError_storage_vector);
  //newTree->Branch("chi2_storage_vector", "std::vector<double>", &chi2_storage_vector);

  //Make Subdirectories for what will follow
  myPlots = myFile->mkdir("Plots");
  myPlots->cd();
  //////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////
  //Directory for the EP angles
  epangles = myPlots->mkdir("EventPlanes");
  wholehfepangles = epangles->mkdir("CombinedHF");
  ////////////////////////////////////////////////////////////
  //Psi1 Raw, Psi1 Final
  //Psi1(even)
  //Whole HF
  char epevenrawname[128],epevenrawtitle[128];
  char epevenfinalname[128],epevenfinaltitle[128];
  //////////////////////////////////////////////////////////////
  //Psi1(odd)
  //Whole HF
  char epoddrawname[128],epoddrawtitle[128];
  char epoddfinalname[128],epoddfinaltitle[128];
  //////////////////////////////////////////////////////////////

  //Visualization of Correction Factors
  //Psi1(even)
  char psi1evencorrsname[128],psi1evencorrstitle[128];
  //Psi1(odd)
  char psi1oddcorrsname[128],psi1oddcorrstitle[128];




  for (Int_t i=0;i<nCent;i++)
    {
      ///////////////////////////////////////////////////////////////////////////////////////////
      //Event Plane Plots
      wholehfepangles->cd();
      //Psi1Even
      //Whole HF
      //Raw
      sprintf(epevenrawname,"Psi1EvenRaw_%1.0lfto%1.0lf",centlo[i],centhi[i]);
      sprintf(epevenrawtitle,"#Psi_{1}^{even} Raw %1.0lfto%1.0lf",centlo[i],centhi[i]);
      PsiEvenRaw[i] = new TH1F(epevenrawname,epevenrawtitle,100,-TMath::Pi()-.392699,TMath::Pi()+.392699);
      PsiEvenRaw[i]->GetXaxis()->SetTitle("EP Angle (radians)");
      //Final
      sprintf(epevenfinalname,"Psi1EvenFinal_%1.0lfto%1.0lf",centlo[i],centhi[i]);
      sprintf(epevenfinaltitle,"#Psi_{1}^{even} Final %1.0lfto%1.0lf",centlo[i],centhi[i]);
      PsiEvenFinal[i] = new TH1F(epevenfinalname,epevenfinaltitle,100,-TMath::Pi()-.392699,TMath::Pi()+.392699);
      PsiEvenFinal[i]->GetXaxis()->SetTitle("EP Angle (radians)");
      ///////////////////////////////////////////////////////////////////////////////////

      //Psi1Odd
      wholehfepangles->cd();
      //Whole HF
      //Raw
      sprintf(epoddrawname,"Psi1OddRaw_%1.0lfto%1.0lf",centlo[i],centhi[i]);
      sprintf(epoddrawtitle,"#Psi_{1}^{odd} Raw %1.0lfto%1.0lf",centlo[i],centhi[i]);
      PsiOddRaw[i] = new TH1F(epoddrawname,epoddrawtitle,100,-TMath::Pi()-.392699,TMath::Pi()+.392699);
      PsiOddRaw[i]->GetXaxis()->SetTitle("EP Angle (radians)");
      //Final
      sprintf(epoddfinalname,"Psi1OddFinal_%1.0lfto%1.0lf",centlo[i],centhi[i]);
      sprintf(epoddfinaltitle,"#Psi_{1}^{odd} Final %1.0lfto%1.0lf",centlo[i],centhi[i]);
      PsiOddFinal[i] = new TH1F(epoddfinalname,epoddfinaltitle,100,-TMath::Pi()-.392699,TMath::Pi()+.392699);
      PsiOddFinal[i]->GetXaxis()->SetTitle("EP Angle (radians)");


      
    }//end of loop over centralities

}//end of initialize function

TFile* G;
void EPPlotting(int runnumber, string Systematics){
///////////////////////////////////
  TFile X(Form("/data2/users/ebadams/Eric_HF_RXN_Data_Sets/%d/Sys_%s_RN_%d_Fourier_Flattened_w_centering_RXN_Plane_Tree_and_Tracks_HF_JAIME.root",runnumber, Systematics.c_str(), runnumber), "recreate");
  TTree *newTree = new TTree("RXN_Plane","RXN_Plane_Tree");

  newTree->Branch("Even_Flattened_RXNP", &Even_Flattened_RXNP, "Even_Flattened_RXNP/D");
  newTree->Branch("Odd_Flattened_RXNP", &Odd_Flattened_RXNP, "Odd_Flattened_RXNP/D");
  newTree->Branch("CentralityBinNum", &CentralityBinNum,"CentralityBinNum/I");
  newTree->Branch("EventNum", &EventNum, "EventNum/I");
  newTree->Branch("run", &run, "run/I");
  newTree->Branch("numberoftracks", &numberoftracks, "numberoftracks/I");
  newTree->Branch("numberofAcceptracks", &numberofAcceptracks, "numberofAcceptracks/I");
  //


  newTree->Branch("vtx_", &vtx_, "vtx_/D");
  
  newTree->Branch("phi_storage_vector", "std::vector<double>", &phi_storage_vector);
  //newTree->Branch("phiError_storage_vector", "std::vector<double>", &phiError_storage_vector);
  newTree->Branch("eta_storage_vector", "std::vector<double>", &eta_storage_vector);
 // newTree->Branch("etaError_storage_vector", "std::vector<double>", &etaError_storage_vector);
  newTree->Branch("Pt_storage_vector",  "std::vector<double>", &Pt_storage_vector);
  ////////////////////

  TFile *f = new TFile(Form("/data2/users/ebadams/Eric_HF_RXN_Data_Sets/%d/Sys_%s_RN_%d_RXN_Plane_VECTORS_Tree_HF_JAIME.root", runnumber, Systematics.c_str(), runnumber));
    //std::cout<<f<<std::endl;
  
    TTree *t = (TTree*)f->Get("RXN_Plane");

    
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

    Int_t NumberOfEvents = t->GetEntries();


    //for making tree ith tracks

    if ( runnumber == 1){
      G = new TFile(Form("/data2/users/ebadams/Erics_Data_Sets/MonteCarlo_NO_FLOW/AMPT_Official/AMPT_Official_2018_PbPb.root"));
    }
    else if (Systematics == "Normal"){
      G = new TFile(Form("/data2/users/ebadams/Erics_Data_Sets/Complete_%d/Full_Erics_rereco_PbPb2018_AOD_MinBias2_%d_RPDZDC.root",runnumber,runnumber));//(Form("%s",FileandPath.c_str()));
    }
    if (Systematics == "LOOSE"){
      G = new TFile(Form("/data2/users/ebadams/Erics_Data_Sets/Complete_%d/LOOSE_Full_Erics_rereco_PbPb2018_AOD_MinBias2_%d_RPDZDC.root",runnumber,runnumber));//(Form("%s",FileandPath.c_str()));
    }
    if (Systematics == "NOMINAL"){
      G = new TFile(Form("/data2/users/ebadams/Erics_Data_Sets/Complete_%d/NOMINAL_Full_Erics_rereco_PbPb2018_AOD_MinBias2_%d_RPDZDC.root",runnumber,runnumber));//(Form("%s",FileandPath.c_str()));
    }
    if (Systematics == "TIGHT"){
      G = new TFile(Form("/data2/users/ebadams/Erics_Data_Sets/Complete_%d/TIGHT_Full_Erics_rereco_PbPb2018_AOD_MinBias2_%d_RPDZDC.root",runnumber,runnumber));//(Form("%s",FileandPath.c_str()));
    }

    ///data2/users/ebadams/ZDC_Only_326943/326943/
    ///home/ebadams/TEST_FOR_ANALYSIS_SOFTWARE/Modern_RXN_Plane

    TTree* ZDCDigiTree = (TTree*)G->Get("analyzer/zdcdigi"); //may need to be zdcdigi only

    ZDCDigiTree->SetBranchStatus("*",0); //turns off all branches
    ZDCDigiTree->SetBranchStatus("nTrack",1); //turnns on branch
    ZDCDigiTree->SetBranchStatus("nAcceptedTracks",1); //turnns on branch
    ZDCDigiTree->SetBranchStatus("Cent",1); //turnns on branch
    ZDCDigiTree->SetBranchStatus("event",1); //turnns on branch
    ZDCDigiTree->SetBranchStatus("run",1); //turnns on branch
    ZDCDigiTree->SetBranchStatus("phi",1); //turnns on branch
    ZDCDigiTree->SetBranchStatus("eta",1); //turnns on branch
    ZDCDigiTree->SetBranchStatus("Pt",1); //turnns on branch
    ZDCDigiTree->SetBranchStatus("vtx",1); //turnns on branch

  
    TLeaf* ZDCeventLeaf   = (TLeaf*)ZDCDigiTree->GetLeaf("event");
    TLeaf* AccpTrackLeaf  = (TLeaf*)ZDCDigiTree->GetLeaf("nAcceptedTracks");
    TLeaf* numberoftracksRAW = (TLeaf*)ZDCDigiTree->GetLeaf("nTrack");
    TLeaf* vtxLeaf = (TLeaf*)ZDCDigiTree->GetLeaf("vtx");
    //TLeaf* CentralityLeaf = (TLeaf*)ZDCDigiTree->GetLeaf("cent");
    
      //  cout << "b" << endl;
    ZDCDigiTree->SetBranchAddress("phi", &phi);
   // ZDCDigiTree->SetBranchAddress("phiError", &phiError);
    ZDCDigiTree->SetBranchAddress("eta", &eta);
   // ZDCDigiTree->SetBranchAddress("etaError", &etaError);
    ZDCDigiTree->SetBranchAddress("Pt", &Pt);
    //ZDCDigiTree->SetBranchAddress("ptError", &ptError);
  //  ZDCDigiTree->SetBranchAddress("chi2", &chi2);

  int N_entries_tracks = ZDCDigiTree->GetEntries();

    std::cout<<NumberOfEvents<<std::endl;

    if (NumberOfEvents == N_entries_tracks){
      cout << "matching number of events!! :)\n";
    }



  int EventNumZDC = 0;
  int addedvalue = 0;
  int numtracks = 0;
  for (Int_t i=0;i<NumberOfEvents;i++){

    //clears track vectors after each event

    delete phi_storage_vector;     phi_storage_vector = new std::vector<double>();
 //   delete phiError_storage_vector;    phiError_storage_vector = new std::vector<double>();
    delete eta_storage_vector;     eta_storage_vector = new std::vector<double>();
  //  delete etaError_storage_vector;    etaError_storage_vector = new std::vector<double>();
    delete Pt_storage_vector;    Pt_storage_vector = new std::vector<double>();
  //  delete ptError_storage_vector;     ptError_storage_vector = new std::vector<double>();
   // delete chi2_storage_vector;    chi2_storage_vector = new std::vector<double>();


    /////////////////////////////////////
      

      if ( !(i%10000) ) cout << " 3rd round, event # " << i << " / " << NumberOfEvents << endl;

      ZDCDigiTree->GetEntry(i);
      t->GetEntry(i);


      EventNum = eventLeaf->GetValue();
      run = runLeaf->GetValue();
      centBin = CentralityLeaf->GetValue(); //getHiBinFromhiHF(CentralityValue);
      centIter_Array_Value = ALICE_Centrality_Array_Chooser(centBin);
      CentralityBinNum = centIter_Array_Value;

      //
      EventNumZDC = ZDCeventLeaf->GetValue();

      vtx_ = vtxLeaf->GetValue();

      EventNumZDC = ZDCeventLeaf->GetValue();
/*
      if (EventNumZDC != EventNum){
        cout << "EventNumZDC = " << EventNumZDC << endl;
        cout << "EventNum = " << EventNum << endl;
        cout << "NON-MATCHED EVENTS Error stopping....\n";
        return;

      }*/

      numberofAcceptracks  = AccpTrackLeaf->GetValue();
      numtracks= numberoftracksRAW->GetValue();

      numberoftracks = numtracks;


      for (int track = 0; track < numberofAcceptracks; track++){

        double etavalue = eta->at(track);
        double ptvalue = Pt->at(track);
        double phivalue = phi->at(track);
/*        double etavalueError = etaError->at(track);
        double ptvalueError = ptError->at(track);
        double phivalueError = phiError->at(track);
        double chi2value = chi2->at(track);*/
        eta_storage_vector->push_back(etavalue);
        Pt_storage_vector->push_back(ptvalue);
        phi_storage_vector->push_back(phivalue);
        /*etaError_storage_vector->push_back(etavalueError);
        ptError_storage_vector->push_back(ptvalueError);
        phiError_storage_vector->push_back(phivalueError);
        chi2_storage_vector->push_back(chi2value);*/
      };

      X_vector_EVEN = X_vector_evn->GetValue();
      X_vector_ODD = X_vector_odd->GetValue();
      Y_vector_EVEN = Y_vector_evn->GetValue();
      Y_vector_ODD = Y_vector_odd->GetValue();

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
      

      /*for (Int_t ii = 0; ii<NumberOfHits_Pos; ii++){
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
      }*/
      


      for (Int_t c=0;c<nCent;c++)
        {
          if ( (centIter_Array_Value*6.3) > centhi[c] ) continue;
          if ( (centIter_Array_Value*6.3) < centlo[c] ) continue;


          //V1 Even
          //Whole HF
          EPhfeven=-999;
          EPhfeven=(1./1.)*atan2(Y_hfeven,X_hfeven);
          if (EPhfeven>(pi)) EPhfeven=(EPhfeven-(TMath::TwoPi()));
          if (EPhfeven<(-1.0*(pi))) EPhfeven=(EPhfeven+(TMath::TwoPi()));
          PsiEvenRaw[c]->Fill(EPhfeven);

  
          //////////////////////////////////////////////
          //V1 odd
          //Whole HF
          EPhfodd=-999;
          EPhfodd=(1./1.)*atan2(Y_hfodd,X_hfodd);
          if (EPhfodd>(pi)) EPhfodd=(EPhfodd-(TMath::TwoPi()));
          if (EPhfodd<(-1.0*(pi))) EPhfodd=(EPhfodd+(TMath::TwoPi()));
          PsiOddRaw[c]->Fill(EPhfodd);



          //v1 even stuff
          //Whole HF
          AngularCorrectionHFEven=0.;EPfinalhfeven=-999.;
          //v1 odd stuff
          //Whole HF
          AngularCorrectionHFOdd=0.;EPfinalhfodd=-999.;

          
          if((EPhfeven>-500) && (EPhfodd>-500))
            {


              //Compute Angular Corrections
              for (Int_t k=1;k<(jMax+1);k++)
                {
                  //v1 even
                  //Whole HF
                  AngularCorrectionHFEven+=((2./k)*(((-Sinhfeven[c][k-1])*(cos(k*EPhfeven)))+((Coshfeven[c][k-1])*(sin(k*EPhfeven)))));
                  //////////////////////////////////////////////////////
                  //v1 odd
                  //Whole HF
                  AngularCorrectionHFOdd+=((2./k)*(((-Sinhfodd[c][k-1])*(cos(k*EPhfodd)))+((Coshfodd[c][k-1])*(sin(k*EPhfodd)))));
                }//end of angular correction calculation
            }//prevent bad corrections

          //Add the final Corrections to the Event Plane
          //and store it and do the flow measurement with it


          //v1 even
          //Whole HF
          EPfinalhfeven=EPhfeven+AngularCorrectionHFEven;
          if (EPfinalhfeven>(pi)) EPfinalhfeven=(EPfinalhfeven-(TMath::TwoPi()));
          if (EPfinalhfeven<(-1.0*(pi))) EPfinalhfeven=(EPfinalhfeven+(TMath::TwoPi()));
          if(EPfinalhfeven>-500)
            {
              PsiEvenFinal[c]->Fill(EPfinalhfeven);
            }
          //////////////////////////////////////////////////
          //v1 odd
          //Whole HF
          EPfinalhfodd=EPhfodd+AngularCorrectionHFOdd;
          if (EPfinalhfodd>(pi)) EPfinalhfodd=(EPfinalhfodd-(TMath::TwoPi()));
          if (EPfinalhfodd<(-1.0*(pi))) EPfinalhfodd=(EPfinalhfodd+(TMath::TwoPi()));
          if(EPfinalhfodd>-500){
            PsiOddFinal[c]->Fill(EPfinalhfodd);
          }

          Even_Flattened_RXNP = EPfinalhfeven; 
          Odd_Flattened_RXNP = EPfinalhfodd;

          Flattened_Odd->Fill(EPfinalhfodd);
          Flattened_Even->Fill(EPfinalhfeven);
          
          newTree->Fill();


        }//End of loop over Centralities
    }//End of loop over events
  myFile->Write();

  X.cd();
  newTree->Write(); 
 // newTree->Close();
  // delete myFile;
  Flattened_Odd->GetXaxis()->SetTitle("#Psi_{1}^{odd} (radians)");
  Flattened_Odd->GetXaxis()->CenterTitle(1);
  Flattened_Odd->GetXaxis()->SetTitleFont(42);
  Flattened_Odd->GetXaxis()->SetTitleSize(0.05);
  Flattened_Odd->GetXaxis()->SetLabelSize(0.045);
  Flattened_Odd->GetZaxis()->SetLabelSize(0.045);
  Flattened_Odd->GetYaxis()->SetTitle("counts/radian");
  Flattened_Odd->GetYaxis()->CenterTitle(1);
  Flattened_Odd->GetYaxis()->SetTitleFont(42);
  Flattened_Odd->GetYaxis()->SetTitleSize(0.05);
  Flattened_Odd->GetYaxis()->SetLabelSize(0.045);
  Flattened_Odd->SaveAs(Form("/data2/users/ebadams/Eric_HF_RXN_Data_Sets/%d/Sys_%s_COMBINED_%d_HF_RXN_Plane_Dists_ODD.root", runnumber, Systematics.c_str(), runnumber));
  Flattened_Even->GetXaxis()->SetTitle("#Psi_{1}^{even} (radians)");
  Flattened_Even->GetXaxis()->CenterTitle(1);
  Flattened_Even->GetXaxis()->SetTitleFont(42);
  Flattened_Even->GetXaxis()->SetTitleSize(0.05);
  Flattened_Even->GetXaxis()->SetLabelSize(0.045);
  Flattened_Even->GetZaxis()->SetLabelSize(0.045);
  Flattened_Even->GetYaxis()->SetTitle("counts/radian");
  Flattened_Even->GetYaxis()->CenterTitle(1);
  Flattened_Even->GetYaxis()->SetTitleFont(42);
  Flattened_Even->GetYaxis()->SetTitleSize(0.05);
  Flattened_Even->GetYaxis()->SetLabelSize(0.045);
  Flattened_Even->SaveAs(Form("/data2/users/ebadams/Eric_HF_RXN_Data_Sets/%d/Sys_%s_COMBINED_%d_HF_RXN_Plane_Dists_EVEN.root", runnumber, Systematics.c_str(), runnumber));
}//end of ep plotting


void FillAngularCorrections(int runnumber, string Systematics){ //this is a cursed solution to legacy code

  switch (runnumber) {
    case 1:

      #include "/data2/users/ebadams/Eric_HF_RXN_Data_Sets/1/Sys_Normal_RN_1_CosAndSinValues.txt"



    case 326776:

      if (Systematics == "Normal"){
        #include "/data2/users/ebadams/Eric_HF_RXN_Data_Sets/326776/Sys_Normal_RN_326776_CosAndSinValues.txt"
      }
      else if (Systematics == "LOOSE"){
        #include "/data2/users/ebadams/Eric_HF_RXN_Data_Sets/326776/Sys_LOOSE_RN_326776_CosAndSinValues.txt"
      }
      else if (Systematics == "NOMINAL"){
        #include "/data2/users/ebadams/Eric_HF_RXN_Data_Sets/326776/Sys_NOMINAL_RN_326776_CosAndSinValues.txt"
      }
      else if (Systematics == "TIGHT"){
        #include "/data2/users/ebadams/Eric_HF_RXN_Data_Sets/326776/Sys_TIGHT_RN_326776_CosAndSinValues.txt"
      }
      break;
    case 327078:

      if (Systematics == "Normal"){
        #include "/data2/users/ebadams/Eric_HF_RXN_Data_Sets/327078/Sys_Normal_RN_327078_CosAndSinValues.txt"
      }
      else if (Systematics == "LOOSE"){
        #include "/data2/users/ebadams/Eric_HF_RXN_Data_Sets/327078/Sys_LOOSE_RN_327078_CosAndSinValues.txt"
      }
      else if (Systematics == "NOMINAL"){
        #include "/data2/users/ebadams/Eric_HF_RXN_Data_Sets/327078/Sys_NOMINAL_RN_327078_CosAndSinValues.txt"
      }
      else if (Systematics == "TIGHT"){
        #include "/data2/users/ebadams/Eric_HF_RXN_Data_Sets/327078/Sys_TIGHT_RN_327078_CosAndSinValues.txt"
      }
      break;
    case 327524:

      if (Systematics == "Normal"){
        #include "/data2/users/ebadams/Eric_HF_RXN_Data_Sets/327524/Sys_Normal_RN_327524_CosAndSinValues.txt"
      }
      else if (Systematics == "LOOSE"){
        #include "/data2/users/ebadams/Eric_HF_RXN_Data_Sets/327524/Sys_LOOSE_RN_327524_CosAndSinValues.txt"
      }
      else if (Systematics == "NOMINAL"){
        #include "/data2/users/ebadams/Eric_HF_RXN_Data_Sets/327524/Sys_NOMINAL_RN_327524_CosAndSinValues.txt"
      }
      else if (Systematics == "TIGHT"){
        #include "/data2/users/ebadams/Eric_HF_RXN_Data_Sets/327524/Sys_TIGHT_RN_327524_CosAndSinValues.txt"
      }
      break;
    case 326822:

      if (Systematics == "Normal"){
        #include "/data2/users/ebadams/Eric_HF_RXN_Data_Sets/326822/Sys_Normal_RN_326822_CosAndSinValues.txt"
      }
      else if (Systematics == "LOOSE"){
        #include "/data2/users/ebadams/Eric_HF_RXN_Data_Sets/326822/Sys_LOOSE_RN_326822_CosAndSinValues.txt"
      }
      else if (Systematics == "NOMINAL"){
        #include "/data2/users/ebadams/Eric_HF_RXN_Data_Sets/326822/Sys_NOMINAL_RN_326822_CosAndSinValues.txt"
      }
      else if (Systematics == "TIGHT"){
        #include "/data2/users/ebadams/Eric_HF_RXN_Data_Sets/326822/Sys_TIGHT_RN_326822_CosAndSinValues.txt"
      }
      break;
    case 327211:

      if (Systematics == "Normal"){
        #include "/data2/users/ebadams/Eric_HF_RXN_Data_Sets/327211/Sys_Normal_RN_327211_CosAndSinValues.txt"
      }
      else if (Systematics == "LOOSE"){
        #include "/data2/users/ebadams/Eric_HF_RXN_Data_Sets/327211/Sys_LOOSE_RN_327211_CosAndSinValues.txt"
      }
      else if (Systematics == "NOMINAL"){
        #include "/data2/users/ebadams/Eric_HF_RXN_Data_Sets/327211/Sys_NOMINAL_RN_327211_CosAndSinValues.txt"
      }
      else if (Systematics == "TIGHT"){
        #include "/data2/users/ebadams/Eric_HF_RXN_Data_Sets/327211/Sys_TIGHT_RN_327211_CosAndSinValues.txt"
      }
      break;
    case 327560:

      if (Systematics == "Normal"){
        #include "/data2/users/ebadams/Eric_HF_RXN_Data_Sets/327560/Sys_Normal_RN_327560_CosAndSinValues.txt"
      }
      else if (Systematics == "LOOSE"){
        #include "/data2/users/ebadams/Eric_HF_RXN_Data_Sets/327560/Sys_LOOSE_RN_327560_CosAndSinValues.txt"
      }
      else if (Systematics == "NOMINAL"){
        #include "/data2/users/ebadams/Eric_HF_RXN_Data_Sets/327560/Sys_NOMINAL_RN_327560_CosAndSinValues.txt"
      }
      else if (Systematics == "TIGHT"){
        #include "/data2/users/ebadams/Eric_HF_RXN_Data_Sets/327560/Sys_TIGHT_RN_327560_CosAndSinValues.txt"
      }
      break;
    case 326883:

      if (Systematics == "Normal"){
        #include "/data2/users/ebadams/Eric_HF_RXN_Data_Sets/326883/Sys_Normal_RN_326883_CosAndSinValues.txt"
      }
      else if (Systematics == "LOOSE"){
        #include "/data2/users/ebadams/Eric_HF_RXN_Data_Sets/326883/Sys_LOOSE_RN_326883_CosAndSinValues.txt"
      }
      else if (Systematics == "NOMINAL"){
        #include "/data2/users/ebadams/Eric_HF_RXN_Data_Sets/326883/Sys_NOMINAL_RN_326883_CosAndSinValues.txt"
      }
      else if (Systematics == "TIGHT"){
        #include "/data2/users/ebadams/Eric_HF_RXN_Data_Sets/326883/Sys_TIGHT_RN_326883_CosAndSinValues.txt"
      }
      break;
    case 327327:

      if (Systematics == "Normal"){
        //#include "/data2/users/ebadams/Eric_HF_RXN_Data_Sets/327327/Sys_Normal_RN_327327_CosAndSinValues.txt"
      }
      else if (Systematics == "LOOSE"){
        #include "/data2/users/ebadams/Eric_HF_RXN_Data_Sets/327327/Sys_LOOSE_RN_327327_CosAndSinValues.txt"
      }
      else if (Systematics == "NOMINAL"){
        //#include "/data2/users/ebadams/Eric_HF_RXN_Data_Sets/327327/Sys_NOMINAL_RN_327327_CosAndSinValues.txt"
      }
      else if (Systematics == "TIGHT"){
        #include "/data2/users/ebadams/Eric_HF_RXN_Data_Sets/327327/Sys_TIGHT_RN_327327_CosAndSinValues.txt"
      }
      break;
    case 326943:

      if (Systematics == "Normal"){
        #include "/data2/users/ebadams/Eric_HF_RXN_Data_Sets/326943/Sys_Normal_RN_326943_CosAndSinValues.txt"
      }
      else if (Systematics == "LOOSE"){
        #include "/data2/users/ebadams/Eric_HF_RXN_Data_Sets/326943/Sys_LOOSE_RN_326943_CosAndSinValues.txt"
      }
      else if (Systematics == "NOMINAL"){
        #include "/data2/users/ebadams/Eric_HF_RXN_Data_Sets/326943/Sys_NOMINAL_RN_326943_CosAndSinValues.txt"
      }
      else if (Systematics == "TIGHT"){
        #include "/data2/users/ebadams/Eric_HF_RXN_Data_Sets/326943/Sys_TIGHT_RN_326943_CosAndSinValues.txt"
      }
      break;
    case 327464:

      if (Systematics == "Normal"){
        #include "/data2/users/ebadams/Eric_HF_RXN_Data_Sets/327464/Sys_Normal_RN_327464_CosAndSinValues.txt"
      }
      else if (Systematics == "LOOSE"){
        #include "/data2/users/ebadams/Eric_HF_RXN_Data_Sets/327464/Sys_LOOSE_RN_327464_CosAndSinValues.txt"
      }
      else if (Systematics == "NOMINAL"){
        #include "/data2/users/ebadams/Eric_HF_RXN_Data_Sets/327464/Sys_NOMINAL_RN_327464_CosAndSinValues.txt"
      }
      else if (Systematics == "TIGHT"){
        #include "/data2/users/ebadams/Eric_HF_RXN_Data_Sets/327464/Sys_TIGHT_RN_327464_CosAndSinValues.txt"
      }
      break;
    default:
      std::cerr << "error: invalid run number " << runnumber << std::endl;
      std::exit(1);
  }
 


// //V1 Even
// //Whole HF
// Coshfeven[0][0]=0.00128987;
// Coshfeven[1][0]=0.000727468;
// Coshfeven[2][0]=-0.0178055;
// Coshfeven[3][0]=0.0021222;
// Coshfeven[4][0]=0.0182887;
// Coshfeven[5][0]=0.00130667;
// Coshfeven[6][0]=0.0011127;
// Coshfeven[7][0]=-0.0133233;
// Coshfeven[8][0]=0.000962819;
 
// Sinhfeven[0][0]=0.00115133;
// Sinhfeven[1][0]=0.000377357;
// Sinhfeven[2][0]=-0.00226828;
// Sinhfeven[3][0]=0.0010787;
// Sinhfeven[4][0]=0.00321933;
// Sinhfeven[5][0]=0.000912281;
// Sinhfeven[6][0]=-9.82587e-05;
// Sinhfeven[7][0]=0.000857407;
// Sinhfeven[8][0]=0.000483835;
 
// Coshfeven[0][1]=-0.00142193;
// Coshfeven[1][1]=0.00158182;
// Coshfeven[2][1]=-0.00108998;
// Coshfeven[3][1]=0.000984009;
// Coshfeven[4][1]=-0.000663319;
// Coshfeven[5][1]=0.00139644;
// Coshfeven[6][1]=-0.000197341;
// Coshfeven[7][1]=0.000204759;
// Coshfeven[8][1]=0.0017253;
 
// Sinhfeven[0][1]=0.00507067;
// Sinhfeven[1][1]=0.00478343;
// Sinhfeven[2][1]=0.00723897;
// Sinhfeven[3][1]=0.00491607;
// Sinhfeven[4][1]=0.00537534;
// Sinhfeven[5][1]=0.00737656;
// Sinhfeven[6][1]=0.00926059;
// Sinhfeven[7][1]=0.00903014;
// Sinhfeven[8][1]=0.00988193;
 
// Coshfeven[0][2]=-0.000662008;
// Coshfeven[1][2]=-0.000477767;
// Coshfeven[2][2]=0.000382717;
// Coshfeven[3][2]=0.000162438;
// Coshfeven[4][2]=0.000656083;
// Coshfeven[5][2]=-0.000219776;
// Coshfeven[6][2]=-0.00108971;
// Coshfeven[7][2]=0.000955728;
// Coshfeven[8][2]=-0.000572421;
 
// Sinhfeven[0][2]=-3.28378e-05;
// Sinhfeven[1][2]=-0.000659484;
// Sinhfeven[2][2]=0.00132002;
// Sinhfeven[3][2]=-0.000278704;
// Sinhfeven[4][2]=0.00097248;
// Sinhfeven[5][2]=-0.000448;
// Sinhfeven[6][2]=-0.00121424;
// Sinhfeven[7][2]=-0.000644747;
// Sinhfeven[8][2]=-0.000724135;
 
// Coshfeven[0][3]=0.000277244;
// Coshfeven[1][3]=0.000976615;
// Coshfeven[2][3]=-3.50986e-05;
// Coshfeven[3][3]=-7.02375e-05;
// Coshfeven[4][3]=0.00163661;
// Coshfeven[5][3]=0.000955828;
// Coshfeven[6][3]=-0.000563681;
// Coshfeven[7][3]=0.00126502;
// Coshfeven[8][3]=0.000365529;
 
// Sinhfeven[0][3]=-8.90599e-05;
// Sinhfeven[1][3]=-0.00125392;
// Sinhfeven[2][3]=0.000902471;
// Sinhfeven[3][3]=0.00110696;
// Sinhfeven[4][3]=-0.00124912;
// Sinhfeven[5][3]=0.000407946;
// Sinhfeven[6][3]=-8.28259e-05;
// Sinhfeven[7][3]=-0.00118815;
// Sinhfeven[8][3]=0.000789927;
 
// Coshfeven[0][4]=-0.000218886;
// Coshfeven[1][4]=-0.000346058;
// Coshfeven[2][4]=0.0035624;
// Coshfeven[3][4]=-0.0010507;
// Coshfeven[4][4]=-0.00109541;
// Coshfeven[5][4]=-0.000554799;
// Coshfeven[6][4]=-0.00158035;
// Coshfeven[7][4]=-0.00174696;
// Coshfeven[8][4]=0.000489692;
 
// Sinhfeven[0][4]=-0.00119396;
// Sinhfeven[1][4]=-0.000842561;
// Sinhfeven[2][4]=0.000553026;
// Sinhfeven[3][4]=-0.000704266;
// Sinhfeven[4][4]=-0.000782233;
// Sinhfeven[5][4]=0.00115292;
// Sinhfeven[6][4]=0.000634491;
// Sinhfeven[7][4]=-0.00129872;
// Sinhfeven[8][4]=-0.00112436;
 
// Coshfeven[0][5]=-0.000902201;
// Coshfeven[1][5]=0.000148742;
// Coshfeven[2][5]=0.000700805;
// Coshfeven[3][5]=0.00144189;
// Coshfeven[4][5]=-0.00136635;
// Coshfeven[5][5]=-0.0016052;
// Coshfeven[6][5]=-0.0019666;
// Coshfeven[7][5]=-0.000709907;
// Coshfeven[8][5]=0.000729223;
 
// Sinhfeven[0][5]=0.00161259;
// Sinhfeven[1][5]=-0.00106582;
// Sinhfeven[2][5]=0.0014456;
// Sinhfeven[3][5]=-9.7809e-05;
// Sinhfeven[4][5]=-0.000365667;
// Sinhfeven[5][5]=0.000965455;
// Sinhfeven[6][5]=0.000160838;
// Sinhfeven[7][5]=-0.000167211;
// Sinhfeven[8][5]=0.00108141;
 
// Coshfeven[0][6]=0.000728985;
// Coshfeven[1][6]=-0.000491767;
// Coshfeven[2][6]=-0.00149849;
// Coshfeven[3][6]=0.000124739;
// Coshfeven[4][6]=0.00143653;
// Coshfeven[5][6]=0.00130202;
// Coshfeven[6][6]=0.000695336;
// Coshfeven[7][6]=0.00241834;
// Coshfeven[8][6]=-0.00108196;
 
// Sinhfeven[0][6]=0.000936874;
// Sinhfeven[1][6]=0.00130245;
// Sinhfeven[2][6]=0.00253356;
// Sinhfeven[3][6]=0.000783793;
// Sinhfeven[4][6]=0.00117352;
// Sinhfeven[5][6]=-0.001269;
// Sinhfeven[6][6]=-0.00108418;
// Sinhfeven[7][6]=-0.000852993;
// Sinhfeven[8][6]=-0.00108957;
 
// Coshfeven[0][7]=-0.000390536;
// Coshfeven[1][7]=-0.000638461;
// Coshfeven[2][7]=0.000678745;
// Coshfeven[3][7]=0.00167201;
// Coshfeven[4][7]=-0.000217575;
// Coshfeven[5][7]=0.00092657;
// Coshfeven[6][7]=0.00109246;
// Coshfeven[7][7]=-0.000646913;
// Coshfeven[8][7]=-0.000232386;
 
// Sinhfeven[0][7]=-0.000676663;
// Sinhfeven[1][7]=-0.000693445;
// Sinhfeven[2][7]=-0.000891749;
// Sinhfeven[3][7]=0.00034436;
// Sinhfeven[4][7]=-0.000564234;
// Sinhfeven[5][7]=-0.000873444;
// Sinhfeven[6][7]=0.000308866;
// Sinhfeven[7][7]=-0.00144806;
// Sinhfeven[8][7]=-0.00125518;
 
// Coshfeven[0][8]=-0.000591589;
// Coshfeven[1][8]=0.000678445;
// Coshfeven[2][8]=-0.00170433;
// Coshfeven[3][8]=-0.000451935;
// Coshfeven[4][8]=-0.00142054;
// Coshfeven[5][8]=0.000484931;
// Coshfeven[6][8]=0.000949124;
// Coshfeven[7][8]=-0.000367559;
// Coshfeven[8][8]=-0.00128795;
 
// Sinhfeven[0][8]=-0.000409909;
// Sinhfeven[1][8]=0.000243558;
// Sinhfeven[2][8]=0.000316717;
// Sinhfeven[3][8]=-0.00128481;
// Sinhfeven[4][8]=0.000137959;
// Sinhfeven[5][8]=5.55071e-05;
// Sinhfeven[6][8]=-0.00109048;
// Sinhfeven[7][8]=0.00264682;
// Sinhfeven[8][8]=0.00265015;
 
// Coshfeven[0][9]=9.43716e-05;
// Coshfeven[1][9]=0.000719721;
// Coshfeven[2][9]=-0.000273147;
// Coshfeven[3][9]=0.000490936;
// Coshfeven[4][9]=0.00283639;
// Coshfeven[5][9]=-5.99888e-05;
// Coshfeven[6][9]=-0.000561706;
// Coshfeven[7][9]=0.000947417;
// Coshfeven[8][9]=-0.000473756;
 
// Sinhfeven[0][9]=0.00042203;
// Sinhfeven[1][9]=-0.00174052;
// Sinhfeven[2][9]=-9.82466e-05;
// Sinhfeven[3][9]=-6.57058e-05;
// Sinhfeven[4][9]=0.000697549;
// Sinhfeven[5][9]=-0.00111236;
// Sinhfeven[6][9]=0.000784131;
// Sinhfeven[7][9]=0.00111832;
// Sinhfeven[8][9]=0.000863995;
 
 
// //Pos HF
// Coshfpeven[0][0]=1;
// Coshfpeven[1][0]=1;
// Coshfpeven[2][0]=1;
// Coshfpeven[3][0]=1;
// Coshfpeven[4][0]=1;
 
// Sinhfpeven[0][0]=0;
// Sinhfpeven[1][0]=0;
// Sinhfpeven[2][0]=0;
// Sinhfpeven[3][0]=0;
// Sinhfpeven[4][0]=0;
 
// Coshfpeven[0][1]=1;
// Coshfpeven[1][1]=1;
// Coshfpeven[2][1]=1;
// Coshfpeven[3][1]=1;
// Coshfpeven[4][1]=1;
 
// Sinhfpeven[0][1]=0;
// Sinhfpeven[1][1]=0;
// Sinhfpeven[2][1]=0;
// Sinhfpeven[3][1]=0;
// Sinhfpeven[4][1]=0;
 
// Coshfpeven[0][2]=1;
// Coshfpeven[1][2]=1;
// Coshfpeven[2][2]=1;
// Coshfpeven[3][2]=1;
// Coshfpeven[4][2]=1;
 
// Sinhfpeven[0][2]=0;
// Sinhfpeven[1][2]=0;
// Sinhfpeven[2][2]=0;
// Sinhfpeven[3][2]=0;
// Sinhfpeven[4][2]=0;
 
// Coshfpeven[0][3]=1;
// Coshfpeven[1][3]=1;
// Coshfpeven[2][3]=1;
// Coshfpeven[3][3]=1;
// Coshfpeven[4][3]=1;
 
// Sinhfpeven[0][3]=0;
// Sinhfpeven[1][3]=0;
// Sinhfpeven[2][3]=0;
// Sinhfpeven[3][3]=0;
// Sinhfpeven[4][3]=0;
 
// Coshfpeven[0][4]=1;
// Coshfpeven[1][4]=1;
// Coshfpeven[2][4]=1;
// Coshfpeven[3][4]=1;
// Coshfpeven[4][4]=1;
 
// Sinhfpeven[0][4]=0;
// Sinhfpeven[1][4]=0;
// Sinhfpeven[2][4]=0;
// Sinhfpeven[3][4]=0;
// Sinhfpeven[4][4]=0;
 
// Coshfpeven[0][5]=1;
// Coshfpeven[1][5]=1;
// Coshfpeven[2][5]=1;
// Coshfpeven[3][5]=1;
// Coshfpeven[4][5]=1;
 
// Sinhfpeven[0][5]=0;
// Sinhfpeven[1][5]=0;
// Sinhfpeven[2][5]=0;
// Sinhfpeven[3][5]=0;
// Sinhfpeven[4][5]=0;
 
// Coshfpeven[0][6]=1;
// Coshfpeven[1][6]=1;
// Coshfpeven[2][6]=1;
// Coshfpeven[3][6]=1;
// Coshfpeven[4][6]=1;
 
// Sinhfpeven[0][6]=0;
// Sinhfpeven[1][6]=0;
// Sinhfpeven[2][6]=0;
// Sinhfpeven[3][6]=0;
// Sinhfpeven[4][6]=0;
 
// Coshfpeven[0][7]=1;
// Coshfpeven[1][7]=1;
// Coshfpeven[2][7]=1;
// Coshfpeven[3][7]=1;
// Coshfpeven[4][7]=1;
 
// Sinhfpeven[0][7]=0;
// Sinhfpeven[1][7]=0;
// Sinhfpeven[2][7]=0;
// Sinhfpeven[3][7]=0;
// Sinhfpeven[4][7]=0;
 
// Coshfpeven[0][8]=1;
// Coshfpeven[1][8]=1;
// Coshfpeven[2][8]=1;
// Coshfpeven[3][8]=1;
// Coshfpeven[4][8]=1;
 
// Sinhfpeven[0][8]=0;
// Sinhfpeven[1][8]=0;
// Sinhfpeven[2][8]=0;
// Sinhfpeven[3][8]=0;
// Sinhfpeven[4][8]=0;
 
// Coshfpeven[0][9]=1;
// Coshfpeven[1][9]=1;
// Coshfpeven[2][9]=1;
// Coshfpeven[3][9]=1;
// Coshfpeven[4][9]=1;
 
// Sinhfpeven[0][9]=0;
// Sinhfpeven[1][9]=0;
// Sinhfpeven[2][9]=0;
// Sinhfpeven[3][9]=0;
// Sinhfpeven[4][9]=0;
 
 
// //Neg HF
// Coshfneven[0][0]=1;
// Coshfneven[1][0]=1;
// Coshfneven[2][0]=1;
// Coshfneven[3][0]=1;
// Coshfneven[4][0]=1;
 
// Sinhfneven[0][0]=0;
// Sinhfneven[1][0]=0;
// Sinhfneven[2][0]=0;
// Sinhfneven[3][0]=0;
// Sinhfneven[4][0]=0;
 
// Coshfneven[0][1]=1;
// Coshfneven[1][1]=1;
// Coshfneven[2][1]=1;
// Coshfneven[3][1]=1;
// Coshfneven[4][1]=1;
 
// Sinhfneven[0][1]=0;
// Sinhfneven[1][1]=0;
// Sinhfneven[2][1]=0;
// Sinhfneven[3][1]=0;
// Sinhfneven[4][1]=0;
 
// Coshfneven[0][2]=1;
// Coshfneven[1][2]=1;
// Coshfneven[2][2]=1;
// Coshfneven[3][2]=1;
// Coshfneven[4][2]=1;
 
// Sinhfneven[0][2]=0;
// Sinhfneven[1][2]=0;
// Sinhfneven[2][2]=0;
// Sinhfneven[3][2]=0;
// Sinhfneven[4][2]=0;
 
// Coshfneven[0][3]=1;
// Coshfneven[1][3]=1;
// Coshfneven[2][3]=1;
// Coshfneven[3][3]=1;
// Coshfneven[4][3]=1;
 
// Sinhfneven[0][3]=0;
// Sinhfneven[1][3]=0;
// Sinhfneven[2][3]=0;
// Sinhfneven[3][3]=0;
// Sinhfneven[4][3]=0;
 
// Coshfneven[0][4]=1;
// Coshfneven[1][4]=1;
// Coshfneven[2][4]=1;
// Coshfneven[3][4]=1;
// Coshfneven[4][4]=1;
 
// Sinhfneven[0][4]=0;
// Sinhfneven[1][4]=0;
// Sinhfneven[2][4]=0;
// Sinhfneven[3][4]=0;
// Sinhfneven[4][4]=0;
 
// Coshfneven[0][5]=1;
// Coshfneven[1][5]=1;
// Coshfneven[2][5]=1;
// Coshfneven[3][5]=1;
// Coshfneven[4][5]=1;
 
// Sinhfneven[0][5]=0;
// Sinhfneven[1][5]=0;
// Sinhfneven[2][5]=0;
// Sinhfneven[3][5]=0;
// Sinhfneven[4][5]=0;
 
// Coshfneven[0][6]=1;
// Coshfneven[1][6]=1;
// Coshfneven[2][6]=1;
// Coshfneven[3][6]=1;
// Coshfneven[4][6]=1;
 
// Sinhfneven[0][6]=0;
// Sinhfneven[1][6]=0;
// Sinhfneven[2][6]=0;
// Sinhfneven[3][6]=0;
// Sinhfneven[4][6]=0;
 
// Coshfneven[0][7]=1;
// Coshfneven[1][7]=1;
// Coshfneven[2][7]=1;
// Coshfneven[3][7]=1;
// Coshfneven[4][7]=1;
 
// Sinhfneven[0][7]=0;
// Sinhfneven[1][7]=0;
// Sinhfneven[2][7]=0;
// Sinhfneven[3][7]=0;
// Sinhfneven[4][7]=0;
 
// Coshfneven[0][8]=1;
// Coshfneven[1][8]=1;
// Coshfneven[2][8]=1;
// Coshfneven[3][8]=1;
// Coshfneven[4][8]=1;
 
// Sinhfneven[0][8]=0;
// Sinhfneven[1][8]=0;
// Sinhfneven[2][8]=0;
// Sinhfneven[3][8]=0;
// Sinhfneven[4][8]=0;
 
// Coshfneven[0][9]=1;
// Coshfneven[1][9]=1;
// Coshfneven[2][9]=1;
// Coshfneven[3][9]=1;
// Coshfneven[4][9]=1;
 
// Sinhfneven[0][9]=0;
// Sinhfneven[1][9]=0;
// Sinhfneven[2][9]=0;
// Sinhfneven[3][9]=0;
// Sinhfneven[4][9]=0;
 
// //V1 Odd
// //Whole HF
// Coshfodd[0][0]=0.00116399;
// Coshfodd[1][0]=0.00109157;
// Coshfodd[2][0]=-0.0113161;
// Coshfodd[3][0]=0.00227058;
// Coshfodd[4][0]=0.0105913;
// Coshfodd[5][0]=0.000710667;
// Coshfodd[6][0]=0.00148837;
// Coshfodd[7][0]=-0.00366553;
// Coshfodd[8][0]=0.000543822;
 
// Sinhfodd[0][0]=0.000793245;
// Sinhfodd[1][0]=0.00135677;
// Sinhfodd[2][0]=-0.0117594;
// Sinhfodd[3][0]=0.00134263;
// Sinhfodd[4][0]=0.0124751;
// Sinhfodd[5][0]=0.000300848;
// Sinhfodd[6][0]=0.000923809;
// Sinhfodd[7][0]=-0.00898538;
// Sinhfodd[8][0]=0.000574528;
 
// Coshfodd[0][1]=-0.000457683;
// Coshfodd[1][1]=0.000122188;
// Coshfodd[2][1]=-0.000601073;
// Coshfodd[3][1]=-0.000828551;
// Coshfodd[4][1]=0.00213989;
// Coshfodd[5][1]=0.000386892;
// Coshfodd[6][1]=0.000743301;
// Coshfodd[7][1]=-0.00150629;
// Coshfodd[8][1]=0.000429432;
 
// Sinhfodd[0][1]=0.00154209;
// Sinhfodd[1][1]=0.00327824;
// Sinhfodd[2][1]=0.00138861;
// Sinhfodd[3][1]=0.00126094;
// Sinhfodd[4][1]=0.00185395;
// Sinhfodd[5][1]=-0.00232735;
// Sinhfodd[6][1]=-0.00187676;
// Sinhfodd[7][1]=-0.00151401;
// Sinhfodd[8][1]=-0.0029732;
 
// Coshfodd[0][2]=-0.000594858;
// Coshfodd[1][2]=6.50808e-05;
// Coshfodd[2][2]=0.00041955;
// Coshfodd[3][2]=0.000678353;
// Coshfodd[4][2]=0.000485832;
// Coshfodd[5][2]=0.000848716;
// Coshfodd[6][2]=-0.000236562;
// Coshfodd[7][2]=1.5063e-05;
// Coshfodd[8][2]=0.000798129;
 
// Sinhfodd[0][2]=0.000215519;
// Sinhfodd[1][2]=0.000146058;
// Sinhfodd[2][2]=-0.00283289;
// Sinhfodd[3][2]=0.000690193;
// Sinhfodd[4][2]=0.00108227;
// Sinhfodd[5][2]=0.000664551;
// Sinhfodd[6][2]=-0.00121495;
// Sinhfodd[7][2]=-0.0027726;
// Sinhfodd[8][2]=0.00030852;
 
// Coshfodd[0][3]=0.000162837;
// Coshfodd[1][3]=-0.000874044;
// Coshfodd[2][3]=0.00035853;
// Coshfodd[3][3]=0.000945221;
// Coshfodd[4][3]=-0.00198599;
// Coshfodd[5][3]=-0.00115776;
// Coshfodd[6][3]=0.000645275;
// Coshfodd[7][3]=0.00013624;
// Coshfodd[8][3]=-0.000519661;
 
// Sinhfodd[0][3]=-0.000629959;
// Sinhfodd[1][3]=-0.00134845;
// Sinhfodd[2][3]=0.0014902;
// Sinhfodd[3][3]=2.0094e-05;
// Sinhfodd[4][3]=-0.0015036;
// Sinhfodd[5][3]=0.000401872;
// Sinhfodd[6][3]=-4.17833e-05;
// Sinhfodd[7][3]=0.000692108;
// Sinhfodd[8][3]=-5.79014e-05;
 
// Coshfodd[0][4]=-9.97992e-05;
// Coshfodd[1][4]=0.00161562;
// Coshfodd[2][4]=0.000415132;
// Coshfodd[3][4]=-5.23123e-05;
// Coshfodd[4][4]=-0.000512059;
// Coshfodd[5][4]=-0.000286905;
// Coshfodd[6][4]=0.00221343;
// Coshfodd[7][4]=-0.00126035;
// Coshfodd[8][4]=0.000578021;
 
// Sinhfodd[0][4]=-0.0020402;
// Sinhfodd[1][4]=0.000384039;
// Sinhfodd[2][4]=-0.000910305;
// Sinhfodd[3][4]=0.000586758;
// Sinhfodd[4][4]=-0.00210053;
// Sinhfodd[5][4]=0.000582189;
// Sinhfodd[6][4]=-0.00161668;
// Sinhfodd[7][4]=0.00017261;
// Sinhfodd[8][4]=0.00010258;
 
// Coshfodd[0][5]=-0.00131347;
// Coshfodd[1][5]=-0.000370955;
// Coshfodd[2][5]=-0.00116581;
// Coshfodd[3][5]=0.000384822;
// Coshfodd[4][5]=0.000143466;
// Coshfodd[5][5]=-0.00084397;
// Coshfodd[6][5]=0.000760835;
// Coshfodd[7][5]=0.000881691;
// Coshfodd[8][5]=0.000200259;
 
// Sinhfodd[0][5]=-0.000928389;
// Sinhfodd[1][5]=-0.00127673;
// Sinhfodd[2][5]=-0.00119875;
// Sinhfodd[3][5]=0.00279268;
// Sinhfodd[4][5]=-0.00169461;
// Sinhfodd[5][5]=0.00135465;
// Sinhfodd[6][5]=-0.000624194;
// Sinhfodd[7][5]=-0.000211925;
// Sinhfodd[8][5]=0.00094446;
 
// Coshfodd[0][6]=-0.000234687;
// Coshfodd[1][6]=-0.000911214;
// Coshfodd[2][6]=-0.00318431;
// Coshfodd[3][6]=-0.000257884;
// Coshfodd[4][6]=-0.000235821;
// Coshfodd[5][6]=-0.00024404;
// Coshfodd[6][6]=-5.3688e-05;
// Coshfodd[7][6]=-0.00106808;
// Coshfodd[8][6]=-0.000384552;
 
// Sinhfodd[0][6]=0.000624196;
// Sinhfodd[1][6]=0.000150632;
// Sinhfodd[2][6]=0.00271124;
// Sinhfodd[3][6]=-0.000916504;
// Sinhfodd[4][6]=-7.33051e-06;
// Sinhfodd[5][6]=0.000684546;
// Sinhfodd[6][6]=0.00125433;
// Sinhfodd[7][6]=-0.00105472;
// Sinhfodd[8][6]=0.00136349;
 
// Coshfodd[0][7]=-1.89531e-05;
// Coshfodd[1][7]=-0.000606595;
// Coshfodd[2][7]=-0.00266761;
// Coshfodd[3][7]=-0.000763993;
// Coshfodd[4][7]=-0.000203556;
// Coshfodd[5][7]=0.000600682;
// Coshfodd[6][7]=-0.000989658;
// Coshfodd[7][7]=0.00179429;
// Coshfodd[8][7]=0.000447211;
 
// Sinhfodd[0][7]=0.00167228;
// Sinhfodd[1][7]=0.000902618;
// Sinhfodd[2][7]=0.00123068;
// Sinhfodd[3][7]=-9.18218e-05;
// Sinhfodd[4][7]=-0.00306594;
// Sinhfodd[5][7]=-0.000253984;
// Sinhfodd[6][7]=-0.0010158;
// Sinhfodd[7][7]=-0.000598866;
// Sinhfodd[8][7]=0.0014278;
 
// Coshfodd[0][8]=5.16379e-05;
// Coshfodd[1][8]=0.000355526;
// Coshfodd[2][8]=0.000221525;
// Coshfodd[3][8]=0.000433103;
// Coshfodd[4][8]=-0.00327425;
// Coshfodd[5][8]=-0.00082354;
// Coshfodd[6][8]=0.00137503;
// Coshfodd[7][8]=-0.000471161;
// Coshfodd[8][8]=-3.31209e-05;
 
// Sinhfodd[0][8]=0.000700268;
// Sinhfodd[1][8]=-0.000359222;
// Sinhfodd[2][8]=-0.00115549;
// Sinhfodd[3][8]=0.00154222;
// Sinhfodd[4][8]=-0.000595348;
// Sinhfodd[5][8]=0.000116252;
// Sinhfodd[6][8]=0.000313902;
// Sinhfodd[7][8]=-0.000710323;
// Sinhfodd[8][8]=8.66507e-05;
 
// Coshfodd[0][9]=-0.0014048;
// Coshfodd[1][9]=-0.000839483;
// Coshfodd[2][9]=-0.00146158;
// Coshfodd[3][9]=-0.000762714;
// Coshfodd[4][9]=-0.000252523;
// Coshfodd[5][9]=0.000756547;
// Coshfodd[6][9]=-0.000474622;
// Coshfodd[7][9]=-0.00128295;
// Coshfodd[8][9]=-0.000207222;
 
// Sinhfodd[0][9]=-0.00166732;
// Sinhfodd[1][9]=-0.00135635;
// Sinhfodd[2][9]=-0.000591909;
// Sinhfodd[3][9]=-0.00164965;
// Sinhfodd[4][9]=0.00105828;
// Sinhfodd[5][9]=0.000675255;
// Sinhfodd[6][9]=0.000359238;
// Sinhfodd[7][9]=0.000580591;
// Sinhfodd[8][9]=0.000747767;
 
 
// //Pos HF
// Coshfpodd[0][0]=1;
// Coshfpodd[1][0]=1;
// Coshfpodd[2][0]=1;
// Coshfpodd[3][0]=1;
// Coshfpodd[4][0]=1;
 
// Sinhfpodd[0][0]=0;
// Sinhfpodd[1][0]=0;
// Sinhfpodd[2][0]=0;
// Sinhfpodd[3][0]=0;
// Sinhfpodd[4][0]=0;
 
// Coshfpodd[0][1]=1;
// Coshfpodd[1][1]=1;
// Coshfpodd[2][1]=1;
// Coshfpodd[3][1]=1;
// Coshfpodd[4][1]=1;
 
// Sinhfpodd[0][1]=0;
// Sinhfpodd[1][1]=0;
// Sinhfpodd[2][1]=0;
// Sinhfpodd[3][1]=0;
// Sinhfpodd[4][1]=0;
 
// Coshfpodd[0][2]=1;
// Coshfpodd[1][2]=1;
// Coshfpodd[2][2]=1;
// Coshfpodd[3][2]=1;
// Coshfpodd[4][2]=1;
 
// Sinhfpodd[0][2]=0;
// Sinhfpodd[1][2]=0;
// Sinhfpodd[2][2]=0;
// Sinhfpodd[3][2]=0;
// Sinhfpodd[4][2]=0;
 
// Coshfpodd[0][3]=1;
// Coshfpodd[1][3]=1;
// Coshfpodd[2][3]=1;
// Coshfpodd[3][3]=1;
// Coshfpodd[4][3]=1;
 
// Sinhfpodd[0][3]=0;
// Sinhfpodd[1][3]=0;
// Sinhfpodd[2][3]=0;
// Sinhfpodd[3][3]=0;
// Sinhfpodd[4][3]=0;
 
// Coshfpodd[0][4]=1;
// Coshfpodd[1][4]=1;
// Coshfpodd[2][4]=1;
// Coshfpodd[3][4]=1;
// Coshfpodd[4][4]=1;
 
// Sinhfpodd[0][4]=0;
// Sinhfpodd[1][4]=0;
// Sinhfpodd[2][4]=0;
// Sinhfpodd[3][4]=0;
// Sinhfpodd[4][4]=0;
 
// Coshfpodd[0][5]=1;
// Coshfpodd[1][5]=1;
// Coshfpodd[2][5]=1;
// Coshfpodd[3][5]=1;
// Coshfpodd[4][5]=1;
 
// Sinhfpodd[0][5]=0;
// Sinhfpodd[1][5]=0;
// Sinhfpodd[2][5]=0;
// Sinhfpodd[3][5]=0;
// Sinhfpodd[4][5]=0;
 
// Coshfpodd[0][6]=1;
// Coshfpodd[1][6]=1;
// Coshfpodd[2][6]=1;
// Coshfpodd[3][6]=1;
// Coshfpodd[4][6]=1;
 
// Sinhfpodd[0][6]=0;
// Sinhfpodd[1][6]=0;
// Sinhfpodd[2][6]=0;
// Sinhfpodd[3][6]=0;
// Sinhfpodd[4][6]=0;
 
// Coshfpodd[0][7]=1;
// Coshfpodd[1][7]=1;
// Coshfpodd[2][7]=1;
// Coshfpodd[3][7]=1;
// Coshfpodd[4][7]=1;
 
// Sinhfpodd[0][7]=0;
// Sinhfpodd[1][7]=0;
// Sinhfpodd[2][7]=0;
// Sinhfpodd[3][7]=0;
// Sinhfpodd[4][7]=0;
 
// Coshfpodd[0][8]=1;
// Coshfpodd[1][8]=1;
// Coshfpodd[2][8]=1;
// Coshfpodd[3][8]=1;
// Coshfpodd[4][8]=1;
 
// Sinhfpodd[0][8]=0;
// Sinhfpodd[1][8]=0;
// Sinhfpodd[2][8]=0;
// Sinhfpodd[3][8]=0;
// Sinhfpodd[4][8]=0;
 
// Coshfpodd[0][9]=1;
// Coshfpodd[1][9]=1;
// Coshfpodd[2][9]=1;
// Coshfpodd[3][9]=1;
// Coshfpodd[4][9]=1;
 
// Sinhfpodd[0][9]=0;
// Sinhfpodd[1][9]=0;
// Sinhfpodd[2][9]=0;
// Sinhfpodd[3][9]=0;
// Sinhfpodd[4][9]=0;
 
 
// //Neg HF
// Coshfnodd[0][0]=1;
// Coshfnodd[1][0]=1;
// Coshfnodd[2][0]=1;
// Coshfnodd[3][0]=1;
// Coshfnodd[4][0]=1;
 
// Sinhfnodd[0][0]=0;
// Sinhfnodd[1][0]=0;
// Sinhfnodd[2][0]=0;
// Sinhfnodd[3][0]=0;
// Sinhfnodd[4][0]=0;
 
// Coshfnodd[0][1]=1;
// Coshfnodd[1][1]=1;
// Coshfnodd[2][1]=1;
// Coshfnodd[3][1]=1;
// Coshfnodd[4][1]=1;
 
// Sinhfnodd[0][1]=0;
// Sinhfnodd[1][1]=0;
// Sinhfnodd[2][1]=0;
// Sinhfnodd[3][1]=0;
// Sinhfnodd[4][1]=0;
 
// Coshfnodd[0][2]=1;
// Coshfnodd[1][2]=1;
// Coshfnodd[2][2]=1;
// Coshfnodd[3][2]=1;
// Coshfnodd[4][2]=1;
 
// Sinhfnodd[0][2]=0;
// Sinhfnodd[1][2]=0;
// Sinhfnodd[2][2]=0;
// Sinhfnodd[3][2]=0;
// Sinhfnodd[4][2]=0;
 
// Coshfnodd[0][3]=1;
// Coshfnodd[1][3]=1;
// Coshfnodd[2][3]=1;
// Coshfnodd[3][3]=1;
// Coshfnodd[4][3]=1;
 
// Sinhfnodd[0][3]=0;
// Sinhfnodd[1][3]=0;
// Sinhfnodd[2][3]=0;
// Sinhfnodd[3][3]=0;
// Sinhfnodd[4][3]=0;
 
// Coshfnodd[0][4]=1;
// Coshfnodd[1][4]=1;
// Coshfnodd[2][4]=1;
// Coshfnodd[3][4]=1;
// Coshfnodd[4][4]=1;
 
// Sinhfnodd[0][4]=0;
// Sinhfnodd[1][4]=0;
// Sinhfnodd[2][4]=0;
// Sinhfnodd[3][4]=0;
// Sinhfnodd[4][4]=0;
 
// Coshfnodd[0][5]=1;
// Coshfnodd[1][5]=1;
// Coshfnodd[2][5]=1;
// Coshfnodd[3][5]=1;
// Coshfnodd[4][5]=1;
 
// Sinhfnodd[0][5]=0;
// Sinhfnodd[1][5]=0;
// Sinhfnodd[2][5]=0;
// Sinhfnodd[3][5]=0;
// Sinhfnodd[4][5]=0;
 
// Coshfnodd[0][6]=1;
// Coshfnodd[1][6]=1;
// Coshfnodd[2][6]=1;
// Coshfnodd[3][6]=1;
// Coshfnodd[4][6]=1;
 
// Sinhfnodd[0][6]=0;
// Sinhfnodd[1][6]=0;
// Sinhfnodd[2][6]=0;
// Sinhfnodd[3][6]=0;
// Sinhfnodd[4][6]=0;
 
// Coshfnodd[0][7]=1;
// Coshfnodd[1][7]=1;
// Coshfnodd[2][7]=1;
// Coshfnodd[3][7]=1;
// Coshfnodd[4][7]=1;
 
// Sinhfnodd[0][7]=0;
// Sinhfnodd[1][7]=0;
// Sinhfnodd[2][7]=0;
// Sinhfnodd[3][7]=0;
// Sinhfnodd[4][7]=0;
 
// Coshfnodd[0][8]=1;
// Coshfnodd[1][8]=1;
// Coshfnodd[2][8]=1;
// Coshfnodd[3][8]=1;
// Coshfnodd[4][8]=1;
 
// Sinhfnodd[0][8]=0;
// Sinhfnodd[1][8]=0;
// Sinhfnodd[2][8]=0;
// Sinhfnodd[3][8]=0;
// Sinhfnodd[4][8]=0;
 
// Coshfnodd[0][9]=1;
// Coshfnodd[1][9]=1;
// Coshfnodd[2][9]=1;
// Coshfnodd[3][9]=1;
// Coshfnodd[4][9]=1;
 
// Sinhfnodd[0][9]=0;
// Sinhfnodd[1][9]=0;
// Sinhfnodd[2][9]=0;
// Sinhfnodd[3][9]=0;
// Sinhfnodd[4][9]=0;
 

 


}//End of Angular Corrections Function
