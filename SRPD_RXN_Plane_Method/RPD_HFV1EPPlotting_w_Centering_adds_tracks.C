#include "TH1F.h"
#include "TProfile.h"
#include <iostream>
#include <iomanip>
#include "TFile.h"
#include "TTree.h"
#include "TLeaf.h"

#include "TMath.h"
 
#include "/home/ebadams/Final_Thesis_Code/Header/MASTER_HEADER_FINAL.h"

void Initialize(int runnumber, string Systematics);
void FillAngularCorrections(int runnumber, string Systematics);
void EPPlotting(int runnumber, string Systematics);


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

//////////////////////////////////
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

void RPD_HFV1EPPlotting_w_Centering_adds_tracks(int runnumber, int numSystematics){
  using namespace std;

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
 // return 0;
}


void Initialize(int runnumber, string Systematics){

  Flattened_Odd  = new TH1D(Form("Histo_Odd_Flat_RXN_Plane_%d", runnumber),Form("RXN_Planes_%d_Odd; RPD Reaction Plane Angle Odd Flattened (#Phi, Radians); Count", runnumber),100,-3.5,3.5);
  Flattened_Even = new TH1D(Form("Histo_Even_Flat_RXN_Plane_%d", runnumber),Form("RXN_Planes_%d_Even; RPD Reaction Plane Angle Even Flattened (#Phi, Radians); Count", runnumber),100,-3.5,3.5);


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
  myFile = new TFile(Form("/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/%d/Sys_%s_RPD_EP_RN_%d_fourierflattened.root",runnumber, Systematics.c_str(), runnumber),"recreate");



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


void EPPlotting(int runnumber, string Systematics){
    TFile X(Form("/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/%d/Sys_%s_RN_%d_RPD_Fourier_Flattened_w_centering_RXN_Plane_Tree_and_Tracks_HF_JAIME.root",runnumber, Systematics.c_str(), runnumber), "recreate");
    TTree *newTree = new TTree("RXN_Plane","RXN_Plane_Tree");

      //create output root >>tree<<
    newTree->Branch("Even_Flattened_RXNP", &Even_Flattened_RXNP, "Even_Flattened_RXNP/D");
    newTree->Branch("Odd_Flattened_RXNP", &Odd_Flattened_RXNP, "Odd_Flattened_RXNP/D");
    newTree->Branch("CentralityBinNum", &CentralityBinNum,"CentralityBinNum/I");
    newTree->Branch("EventNum", &EventNum, "EventNum/I");
    newTree->Branch("run", &run, "run/I");
    newTree->Branch("numberoftracks", &numberoftracks, "numberoftracks/I");
    newTree->Branch("numberofAcceptracks", &numberofAcceptracks, "numberofAcceptracks/I");
  
    newTree->Branch("vtx_", &vtx_, "vtx_/D");
  
    newTree->Branch("phi_storage_vector", "std::vector<double>", &phi_storage_vector);
    //newTree->Branch("phiError_storage_vector", "std::vector<double>", &phiError_storage_vector);
    newTree->Branch("eta_storage_vector", "std::vector<double>", &eta_storage_vector);
   // newTree->Branch("etaError_storage_vector", "std::vector<double>", &etaError_storage_vector);
    newTree->Branch("Pt_storage_vector",  "std::vector<double>", &Pt_storage_vector);
   // newTree->Branch("ptError_storage_vector",  "std::vector<double>", &ptError_storage_vector);
    //newTree->Branch("chi2_storage_vector", "std::vector<double>", &chi2_storage_vector);


    TFile *f = new TFile(Form("/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/%d/Sys_%s_RN_%d_RXN_Plane_VECTORS_Tree_HF_JAIME.root", runnumber, Systematics.c_str(), runnumber));
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

    TFile* G;
    if (Systematics == "Normal"){
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


    TTree* ZDCDigiTree = (TTree*)G->Get("analyzer/zdcdigi");

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

    std::cout<<N_entries_tracks<<std::endl;

  /*  if (NumberOfEvents == N_entries_tracks){
      cout << "matching number of events!! :)\n";
    }

*/
  int q = 0;
  int EventNumZDC = 0;
  int addedvalue = 0;
  int numtracks = 0;
  for (Int_t i=0;i< N_entries_tracks;i++){

    //clears track vectors after each event

    delete phi_storage_vector;     phi_storage_vector = new std::vector<double>();
 //   delete phiError_storage_vector;    phiError_storage_vector = new std::vector<double>();
    delete eta_storage_vector;     eta_storage_vector = new std::vector<double>();
  //  delete etaError_storage_vector;    etaError_storage_vector = new std::vector<double>();
    delete Pt_storage_vector;    Pt_storage_vector = new std::vector<double>();
  //  delete ptError_storage_vector;     ptError_storage_vector = new std::vector<double>();
   // delete chi2_storage_vector;    chi2_storage_vector = new std::vector<double>();


    /////////////////////////////////////
      

      if ( !(i%10000) ) cout << " 3rd round, event # " << i << " / " << N_entries_tracks << endl;


      ZDCDigiTree->GetEntry(i);

      EventNumZDC = ZDCeventLeaf->GetValue();

      vtx_ = vtxLeaf->GetValue();

      t->GetEntry(q);

      EventNum = eventLeaf->GetValue();

       if (EventNumZDC != EventNum){
        continue;
      }
      else{
        q += 1;
       // cout << "matched event " << endl;
      }
      run = runLeaf->GetValue();
      centBin = CentralityLeaf->GetValue(); //getHiBinFromhiHF(CentralityValue);
      centIter_Array_Value = ALICE_Centrality_Array_Chooser(centBin);
      CentralityBinNum = centIter_Array_Value;

      //

      numberofAcceptracks  = AccpTrackLeaf->GetValue();
      numtracks= numberoftracksRAW->GetValue();

      numberoftracks = numtracks;


      for (int track = 0; track < numberofAcceptracks; track++){

        double etavalue = eta->at(track);
        double ptvalue = Pt->at(track);
        double phivalue = phi->at(track);

        eta_storage_vector->push_back(etavalue);
        Pt_storage_vector->push_back(ptvalue);
        phi_storage_vector->push_back(phivalue);

      };

      X_vector_EVEN = X_vector_evn->GetValue();
      X_vector_ODD = X_vector_odd->GetValue();
      Y_vector_EVEN = Y_vector_evn->GetValue();
      Y_vector_ODD = Y_vector_odd->GetValue();

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

Flattened_Odd->SaveAs(Form("/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/%d/Sys_%s_COMBINED_%d_RPD_RXN_Plane_Dists_ODD.root", runnumber, Systematics.c_str(), runnumber));
Flattened_Even->SaveAs(Form("/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/%d/Sys_%s_COMBINED_%d_RPD_RXN_Plane_Dists_EVEN.root", runnumber, Systematics.c_str(), runnumber));
}//end of ep plotting


void FillAngularCorrections(int runnumber, string Systematics){ //this is a cursed solution to legacy code

  switch (runnumber) {
    case 326776:
      if (Systematics == "Normal"){
        #include "/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/326776/Sys_Normal_RN_326776_CosAndSinValues.txt"
      }
      else if (Systematics == "LOOSE"){
        #include "/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/326776/Sys_LOOSE_RN_326776_CosAndSinValues.txt"
      }
      else if (Systematics == "NOMINAL"){
        #include "/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/326776/Sys_NOMINAL_RN_326776_CosAndSinValues.txt"
      }
      else if (Systematics == "TIGHT"){
        #include "/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/326776/Sys_TIGHT_RN_326776_CosAndSinValues.txt"
      }
      break;
    case 327078:
      if (Systematics == "Normal"){
        #include "/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/327078/Sys_Normal_RN_327078_CosAndSinValues.txt"
      }
      else if (Systematics == "LOOSE"){
        #include "/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/327078/Sys_LOOSE_RN_327078_CosAndSinValues.txt"
      }
      else if (Systematics == "NOMINAL"){
        #include "/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/327078/Sys_NOMINAL_RN_327078_CosAndSinValues.txt"
      }
      else if (Systematics == "TIGHT"){
        #include "/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/327078/Sys_TIGHT_RN_327078_CosAndSinValues.txt"
      }
      break;
    case 327524:
      if (Systematics == "Normal"){
        #include "/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/327524/Sys_Normal_RN_327524_CosAndSinValues.txt"
      }
      else if (Systematics == "LOOSE"){
        #include "/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/327524/Sys_LOOSE_RN_327524_CosAndSinValues.txt"
      }
      else if (Systematics == "NOMINAL"){
        #include "/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/327524/Sys_NOMINAL_RN_327524_CosAndSinValues.txt"
      }
      else if (Systematics == "TIGHT"){
        #include "/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/327524/Sys_TIGHT_RN_327524_CosAndSinValues.txt"
      }
      break;
    case 326822:
      if (Systematics == "Normal"){
        #include "/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/326822/Sys_Normal_RN_326822_CosAndSinValues.txt"
      }
      else if (Systematics == "LOOSE"){
        #include "/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/326822/Sys_LOOSE_RN_326822_CosAndSinValues.txt"
      }
      else if (Systematics == "NOMINAL"){
        #include "/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/326822/Sys_NOMINAL_RN_326822_CosAndSinValues.txt"
      }
      else if (Systematics == "TIGHT"){
        #include "/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/326822/Sys_TIGHT_RN_326822_CosAndSinValues.txt"
      }
      break;
    case 327211:
      if (Systematics == "Normal"){
        #include "/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/327211/Sys_Normal_RN_327211_CosAndSinValues.txt"
      }
      else if (Systematics == "LOOSE"){
        #include "/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/327211/Sys_LOOSE_RN_327211_CosAndSinValues.txt"
      }
      else if (Systematics == "NOMINAL"){
        #include "/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/327211/Sys_NOMINAL_RN_327211_CosAndSinValues.txt"
      }
      else if (Systematics == "TIGHT"){
        #include "/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/327211/Sys_TIGHT_RN_327211_CosAndSinValues.txt"
      }
      break;
    case 327560:
      if (Systematics == "Normal"){
        #include "/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/327560/Sys_Normal_RN_327560_CosAndSinValues.txt"
      }
      else if (Systematics == "LOOSE"){
        #include "/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/327560/Sys_LOOSE_RN_327560_CosAndSinValues.txt"
      }
      else if (Systematics == "NOMINAL"){
        #include "/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/327560/Sys_NOMINAL_RN_327560_CosAndSinValues.txt"
      }
      else if (Systematics == "TIGHT"){
        #include "/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/327560/Sys_TIGHT_RN_327560_CosAndSinValues.txt"
      }
      break;
    case 326883:
      if (Systematics == "Normal"){
        #include "/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/326883/Sys_Normal_RN_326883_CosAndSinValues.txt"
      }
      else if (Systematics == "LOOSE"){
        #include "/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/326883/Sys_LOOSE_RN_326883_CosAndSinValues.txt"
      }
      else if (Systematics == "NOMINAL"){
        #include "/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/326883/Sys_NOMINAL_RN_326883_CosAndSinValues.txt"
      }
      else if (Systematics == "TIGHT"){
        #include "/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/326883/Sys_TIGHT_RN_326883_CosAndSinValues.txt"
      }
      break;
    case 327327:
      if (Systematics == "Normal"){
        #include "/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/327327/Sys_Normal_RN_327327_CosAndSinValues.txt"
      }
      else if (Systematics == "LOOSE"){
        #include "/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/327327/Sys_LOOSE_RN_327327_CosAndSinValues.txt"
      }
      else if (Systematics == "NOMINAL"){
        #include "/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/327327/Sys_NOMINAL_RN_327327_CosAndSinValues.txt"
      }
      else if (Systematics == "TIGHT"){
        #include "/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/327327/Sys_TIGHT_RN_327327_CosAndSinValues.txt"
      }
      break;
    case 326943:
      if (Systematics == "Normal"){
        #include "/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/326943/Sys_Normal_RN_326943_CosAndSinValues.txt"
      }
      else if (Systematics == "LOOSE"){
        #include "/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/326943/Sys_LOOSE_RN_326943_CosAndSinValues.txt"
      }
      else if (Systematics == "NOMINAL"){
        #include "/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/326943/Sys_NOMINAL_RN_326943_CosAndSinValues.txt"
      }
      else if (Systematics == "TIGHT"){
        #include "/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/326943/Sys_TIGHT_RN_326943_CosAndSinValues.txt"
      }
      break;
    case 327464:
      if (Systematics == "Normal"){
        #include "/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/327464/Sys_Normal_RN_327464_CosAndSinValues.txt"
      }
      else if (Systematics == "LOOSE"){
        #include "/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/327464/Sys_LOOSE_RN_327464_CosAndSinValues.txt"
      }
      else if (Systematics == "NOMINAL"){
        #include "/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/327464/Sys_NOMINAL_RN_327464_CosAndSinValues.txt"
      }
      else if (Systematics == "TIGHT"){
        #include "/data2/users/ebadams/Eric_RPD_RXN_Data_Sets/327464/Sys_TIGHT_RN_327464_CosAndSinValues.txt"
      }
      break;
    default:
      std::cerr << "error: invalid run number " << runnumber << std::endl;
      std::exit(1);
  }
 
}//End of Angular Corrections Function