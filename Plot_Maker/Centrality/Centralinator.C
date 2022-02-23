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

void Centralinator(int runnumber){
  TFile *f;

	f = new TFile(Form("/data2/users/ebadams/Erics_Data_Sets/Complete_%d/Full_Erics_rereco_PbPb2018_AOD_MinBias2_%d_RPDZDC.root",runnumber,runnumber));//(Form("%s",FileandPath.c_str()));
 
 	TH1D* Centrality;
 	Centrality = new TH1D(Form("Centrality_%d", runnumber),Form("Centrality_%d", runnumber),20,0,20);

  	//std::cout<<f<<std::endl;
  	
  	TTree *t = (TTree*)f->Get("analyzer/zdcdigi");
  	//std::cout<<t<<std::endl;

  	TLeaf* CentralityLeaf =(TLeaf*)t->GetLeaf("cent");

  	Int_t NumberOfEvents = t->GetEntries();

    std::cout<<NumberOfEvents<<std::endl;

    Double_t pi = TMath::Pi();

    for (Int_t i=0; i< NumberOfEvents; i++){
    	t->GetEntry(i);

    	   if ( !(i%10000) ) std::cout<<"Events # " << i <<std::endl;
	
    	  CentralityValue = CentralityLeaf->GetValue();
    	  centBin = getHiBinFromhiHF(CentralityValue);
    	  centIter_Array_Value = ALICE_Centrality_Array_Chooser(centBin);

    	  Centrality->Fill(centIter_Array_Value, 1);

    }

      gStyle->SetOptStat(0);

     auto D = new TCanvas("D", "D", 3000, 2000);
     Centrality->Draw("Hist E");
     D->SaveAs(Form("/home/ebadams/Final_Thesis_Code/Plot_Maker/Centrality/Cent_Dist_%d_.png", runnumber));
     D->SaveAs(Form("/home/ebadams/Final_Thesis_Code/Plot_Maker/Centrality/Cent_Dist_%d_.ROOT", runnumber));

}