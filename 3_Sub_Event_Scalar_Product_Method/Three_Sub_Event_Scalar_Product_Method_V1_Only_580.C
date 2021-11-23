//https://arxiv.org/pdf/1910.14406.pdf
//https://arxiv.org/pdf/1209.2323.pdf



#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TH2.h"
#include "TF1.h"
#include "TProfile.h"
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
#include <iomanip>
#include <sstream>
#include "TGraph.h"
#include "TMultiGraph.h"
#include "TLegend.h"
#include "TProfile.h"
#include "TProfile2D.h"
#include "TStyle.h"

#include "/home/ebadams/Final_Thesis_Code/Header/MASTER_HEADER_FINAL.h"
#include "/home/ebadams/CMSSW_10_3_3/src/Tracking_Eff_2018/trackingEfficiency2018PbPb.h"



int Three_Sub_Event_Scalar_Product_Method_V1_Only_580(int EtaBins, int PtBins, int numRPD_or_HF, int numSystematics, int numVertex, int runnumber, int numEff){

	//numVertex 0 is normal  1 is "WIDE" cut 3 < |v_{z}| < 15 cm and 2 a "narrow" cut $|v_z| <$ 3 cm.
	//if runnumber is entered as 1 it will run the montecarlo
	///////////////////////////////////////////////////////////////////////////////
	string RPD_or_HF;
	if (numRPD_or_HF == 0){
		RPD_or_HF = "RPD";
	}
	else if (numRPD_or_HF == 1){
		RPD_or_HF = "HF";
	}

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

	string Vertex;
	if (numVertex == 0){
		Vertex = "Normal";
	}
	else if (numVertex == 1){
		Vertex = "WIDE";
	}
	else if (numVertex == 2){
		Vertex = "NARROW";
	}


	string EfficiencyStrings[4] = {"", "Loose", "", "Tight"};

    TrkEff2018PbPb TrackEffFxn("general", EfficiencyStrings[numSystematics].c_str(), true, "/home/ebadams/CMSSW_10_3_3/src/Tracking_Eff_2018/");

    double WeightValue = 1;

	float etamin = -1.25; //psuedorapidity .75
	float etamax = 1.25;
	float Ptmin = 0.0;
	float Ptmax = 8.0; //GeV/C //////////////////////////////////////////////////////////////////////////SET THIS BACK TO 12 ()

	double EtaRanges[EtaBins];

	for (int i = 0; i< (EtaBins+1); i++){ //for oragnijnz values into eta bins
		double adder = (2*etamax)/(EtaBins); //-2.5 to 2.5 eta is range of tracker
		//cout << "adder " << adder << endl;
		EtaRanges[i] = (etamin + adder*i);

		cout << "EtaRanges " << EtaRanges[i] << endl;;
	}

	double PtRanges[PtBins];

	for (int i = 0; i< (PtBins+1); i++){ //for organizing values into pt bins
		float adder = Ptmax/(PtBins);

		if (Ptmin == 0){
		PtRanges[i] = (Ptmin + adder*i);
		}
		else{
		PtRanges[i] = (adder*i);
		}
		cout << "PtRanges" << PtRanges[i] << endl;
	}
/*	string binchecker = "Cheese For Everyone";

	cout << " are the bins listed acceptable? If yes, please type yes, otherwise anything to say no... " << endl;
	cin >> binchecker;
	if (binchecker != "yes"){
		cout << "EXITING...\n";
		return;
	}
	else{
		cout << " proceeding... " << endl;
	}*/

	////////////HISTOGRAMS////////////////////////////////////////////////////////////////

	TFile *myFile;
	TDirectory* myPlots;
	myFile = new TFile(Form("/data2/users/ebadams/Eric_SP_Data_Sets/%s/%d/%d_HFEP_EPPlottingV1.root", RPD_or_HF.c_str(), runnumber,runnumber),"recreate");
	
	//Make Subdirectories for what will follow
	myPlots = myFile->mkdir("Plots");
	myPlots->cd();
	
	TDirectory* v1plots;
	v1plots = myPlots->mkdir("v1plots");
	TDirectory* v1PREQPlots;
	v1PREQPlots = v1plots->mkdir("v1PREQPlots");
	TDirectory* v1POSTQPlots;
	v1POSTQPlots = v1plots->mkdir("v1POSTQPlots");
	TDirectory* v1PreEta;
	v1PreEta = v1plots->mkdir("v1PreEta");
	TDirectory* v1PrePt;
	v1PrePt = v1plots->mkdir("v1PrePt");
	TDirectory* V1Even;
	V1Even = v1plots->mkdir("V1Even");
	TDirectory* V1Odd;
	V1Odd = v1plots->mkdir("V1Odd");


	string centralitystring[10] = {"0-10%", "10-20%", "20-30%", "30-40%", "40-50%", "50-60%", "60-70%", "70-80%", "80-90%", "90-100%"};

	v1PREQPlots->cd();

	TProfile2D* TP2D_Pre_QNEGx = new TProfile2D("TP2D_Pre_QNEGx","TP2D_Pre_QNEGx_w_CENT; Centrality Bins (10); Top ROW Errors (STDDEV), Bottom Row Means",10,0,10,2,0,2,"s(j)");
	TProfile2D* TP2D_Pre_QNEGy = new TProfile2D("TP2D_Pre_QNEGy","TP2D_Pre_QNEGy_w_CENT; Centrality Bins (10); Top ROW Errors (STDDEV), Bottom Row Means",10,0,10,2,0,2,"s(j)");
	TProfile2D* TP2D_Pre_QPOSx = new TProfile2D("TP2D_Pre_QPOSx","TP2D_Pre_QPOSx_w_CENT; Centrality Bins (10); Top ROW Errors (STDDEV), Bottom Row Means",10,0,10,2,0,2,"s(j)");
	TProfile2D* TP2D_Pre_QPOSy = new TProfile2D("TP2D_Pre_QPOSy","TP2D_Pre_QPOSy_w_CENT; Centrality Bins (10); Top ROW Errors (STDDEV), Bottom Row Means",10,0,10,2,0,2,"s(j)");

	v1POSTQPlots->cd();

	TProfile2D* TP2D_Post_QNEGx = new TProfile2D("TP2D_Post_QNEGx","TP2D_Post_QNEGx_w_CENT; Centrality Bins (10); Top ROW Errors (STDDEV), Bottom Row Means",10,0,10,2,0,2,"s(j)");
	TProfile2D* TP2D_Post_QNEGy = new TProfile2D("TP2D_Post_QNEGy","TP2D_Post_QNEGy_w_CENT; Centrality Bins (10); Top ROW Errors (STDDEV), Bottom Row Means",10,0,10,2,0,2,"s(j)");
	TProfile2D* TP2D_Post_QPOSx = new TProfile2D("TP2D_Post_QPOSx","TP2D_Post_QPOSx_w_CENT; Centrality Bins (10); Top ROW Errors (STDDEV), Bottom Row Means",10,0,10,2,0,2,"s(j)");
	TProfile2D* TP2D_Post_QPOSy = new TProfile2D("TP2D_Post_QPOSy","TP2D_Post_QPOSy_w_CENT; Centrality Bins (10); Top ROW Errors (STDDEV), Bottom Row Means",10,0,10,2,0,2,"s(j)");

	v1PreEta->cd();

	TProfile2D* V1_Eta_Pre_Denominator = new TProfile2D("V1_Eta_Pre_Denominator","V1_Eta_Pre_Denominator_w_CENT; EtaBin; Centrality Bins (10)",EtaBins,EtaRanges,10,0,10,"s(j)");
	TProfile2D* V1_Neg_Eta_Pre_Numerator  = new TProfile2D("Neg_Eta_Pre_Numerator" ,"Neg_Eta_Pre_Numerator_w_CENT; EtaBin; Centrality Bins (10)",EtaBins,EtaRanges,10,0,10,"s(j)");
	TProfile2D* V1_Pos_Eta_Pre_Numerator  = new TProfile2D("Pos_Eta_Pre_Numerator" ,"Pos_Eta_Pre_Numerator_w_CENT; EtaBin; Centrality Bins (10)",EtaBins,EtaRanges,10,0,10,"s(j)");
 
 	v1PrePt->cd();

	TProfile2D* V1_Pt_Pre_Denominator   = new TProfile2D("PV1_Pt_Pre_Denominator","V1_Pt_Pre_Denominator_w_CENT; PtBin; Centrality Bins (10)",PtBins,PtRanges,10,0,10,"s(j)");
	TProfile2D* V1_Neg_Pt_Pre_Numerator = new TProfile2D("PV1_Neg_Pt_Pre_Numerator","V1_Neg_Pt_Pre_Numerator_w_CENT; PtBin; Centrality Bins (10)",PtBins,PtRanges,10,0,10,"s(j)");
	TProfile2D* V1_Pos_Pt_Pre_Numerator = new TProfile2D("PV1_Pos_Pt_Pre_Numerator","V1_Pos_Pt_Pre_Numerator_w_CENT; PtBin; Centrality Bins (10)",PtBins,PtRanges,10,0,10,"s(j)");

	TH1D* V1_Even_Histo[10];
	TH1D* V1_Odd_Histo[10];

	for ( int i = 0; i < 10; i++){

		V1Even->cd();
		V1_Even_Histo[i] = new TH1D(Form("%s_V1_Even_%s", RPD_or_HF.c_str(),centralitystring[i].c_str()),Form("%s_V1_Even_5-80;  Pt; V1 Magnitude [A.U.]", RPD_or_HF.c_str(), centralitystring[i].c_str()), PtBins, Ptmin, Ptmax);
	
		V1Odd->cd();
		V1_Odd_Histo[i] = new TH1D(Form("%s_V1_Odd_%s", RPD_or_HF.c_str(), centralitystring[i].c_str()), Form("%s_V1_Odd_5-80; Eta; V1 Magnitude [A.U.]", RPD_or_HF.c_str(), centralitystring[i].c_str()), EtaBins, etamin, etamax);
	}

	////////////Global_Variables////////////////////////////////////////////////////////

	double Accep_N_Tracks = 0;
	double numHFTowersNeg = 0;
	double numHFTowersPos = 0;

	//pos
	double QPOSx_Numerator  = 0;
	double QPOSy_Numerator  = 0;
	double QPOS_Denominator = 0;
	//neg
	double QNEGx_Numerator  = 0;
	double QNEGy_Numerator  = 0;
	double QNEG_Denominator = 0;

	double QNEGx = 0;
	double QNEGy = 0;
	double QPOSx = 0;
	double QPOSy = 0;

	double AVG_QNEGx[10] = {0};
	double AVG_QNEGy[10] = {0};
	double AVG_QPOSx[10] = {0};
	double AVG_QPOSy[10] = {0};
	double Bin_Err_TP2D_QNEGx[10] = {0};
	double Bin_Err_TP2D_QNEGy[10] = {0};
	double Bin_Err_TP2D_QPOSx[10] = {0};
	double Bin_Err_TP2D_QPOSy[10] = {0};

	double vtx_value = 0;

	double RPD_Weights[2][10][16] = {0};

	int Eta_binfilled_Check[EtaBins];
	int Pt_binfilled_Check[PtBins];


	std::vector<double>* phi = 0;
	//std::vector<double>* phiError = 0;
	std::vector<double>* eta = 0;
	//std::vector<double>* etaError = 0;
	std::vector<double>* Pt = 0;
	//std::vector<double>* ptError = 0;
	std::vector<double>* chi2 = 0;

	std::vector<double>* HFPos_Phi = 0;
    std::vector<double>* HFPos_Eta = 0;
    std::vector<double>* HFPos_Energy = 0;
    std::vector<double>* HFNeg_Phi = 0;
    std::vector<double>* HFNeg_Eta = 0;
    std::vector<double>* HFNeg_Energy = 0;
    std::vector<double>* HFPos_Et = 0;
    std::vector<double>* HFNeg_Et = 0;
  
  
   	TFile *f;

	////////////////////////////////////////////////////////////////////////////////////
  	if ( runnumber == 1){
  		f = new TFile(Form("/data2/users/ebadams/Erics_Data_Sets/MonteCarlo_NO_FLOW/AMPT_Official/AMPT_Official_2018_PbPb.root"));
  	}
  	if (runnumber == 2){
  		//Private Monte Carlo
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
	ZDCDigiTree->SetBranchStatus("vtx",1); //turnns on branch

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

	ZDCDigiTree->SetBranchStatus("nHF_pos", 1);// turns branch on
	ZDCDigiTree->SetBranchStatus("nHF_neg", 1);// turns branch on
	ZDCDigiTree->SetBranchStatus("HFPos_Phi", 1);// turns branch on
	ZDCDigiTree->SetBranchStatus("HFPos_Eta", 1);// turns branch on
	ZDCDigiTree->SetBranchStatus("HFPos_Energy", 1);// turns branch on
	ZDCDigiTree->SetBranchStatus("HFNeg_Phi", 1);// turns branch on
	ZDCDigiTree->SetBranchStatus("HFNeg_Eta", 1);// turns branch on
	ZDCDigiTree->SetBranchStatus("HFNeg_Energy", 1);// turns branch on

	TLeaf* zsideLeaf ; 
	TLeaf* sectionLeaf;
	TLeaf* channelLeaf;

	TLeaf* eventLeaf;  
	TLeaf* runLeaf;    
	TLeaf* AccpTrackLeaf;
	TLeaf* CentralityLeaf;
	TLeaf* vtxLeaf;

	TLeaf* NHFPos;
	TLeaf* NHFNeg;

	TLeaf* fCleaf[10];

	CentralityLeaf =(TLeaf*)ZDCDigiTree->GetLeaf("cent");
	eventLeaf = (TLeaf*)ZDCDigiTree->GetLeaf("event");
	runLeaf = (TLeaf*)ZDCDigiTree->GetLeaf("run");
	AccpTrackLeaf = (TLeaf*)ZDCDigiTree->GetLeaf("nAcceptedTracks");
	vtxLeaf = (TLeaf*)ZDCDigiTree->GetLeaf("vtx");
	
	ZDCDigiTree->SetBranchAddress("phi", &phi);
	ZDCDigiTree->SetBranchAddress("eta", &eta);
	ZDCDigiTree->SetBranchAddress("Pt", &Pt);

	NHFPos = (TLeaf*)ZDCDigiTree->GetLeaf("nHF_pos");
	NHFNeg = (TLeaf*)ZDCDigiTree->GetLeaf("nHF_neg");


	if (RPD_or_HF == "HF"){
	
		ZDCDigiTree->SetBranchAddress("HFPos_Phi", &HFPos_Phi);
		ZDCDigiTree->SetBranchAddress("HFPos_Eta", &HFPos_Eta);
		ZDCDigiTree->SetBranchAddress("HFPos_Energy", &HFPos_Energy);
		ZDCDigiTree->SetBranchAddress("HFNeg_Phi", &HFNeg_Phi);
		ZDCDigiTree->SetBranchAddress("HFNeg_Eta", &HFNeg_Eta);
		ZDCDigiTree->SetBranchAddress("HFNeg_Energy", &HFNeg_Energy);
	
		}
	if (RPD_or_HF == "RPD"){

  		zsideLeaf = (TLeaf*)ZDCDigiTree->GetLeaf("zside");
  		sectionLeaf = (TLeaf*)ZDCDigiTree->GetLeaf("section");
  		channelLeaf = (TLeaf*)ZDCDigiTree->GetLeaf("channel");

 		for (int iTS = 0; iTS < NTS; iTS++){
 		    fCleaf[iTS] = (TLeaf*)ZDCDigiTree->GetLeaf(Form("nfC%d", iTS));
 		}

	}




	int N_entries = ZDCDigiTree->GetEntries();

	std::cout<<"Number_of_Events = " << N_entries <<std::endl;

  	ZDCDigiTree->GetEntry(0);

  	int RunNumber = runLeaf->GetValue(0);
  	cout << "RUNNUMBER  = " << RunNumber << endl;

  	cout << "RUN STYLE IS ... " << RPD_or_HF << "\n Press enter to continue...\n";
  	//cin.ignore();


	///////////////////////////////////////////////////////////////////////////

	//use hf phi and cos sin like jaimes


	Returns_SRPD_Calibrations(RunNumber,RPD_Weights);


	bool TF = true;
	bool Recentered = false;

	Restart_Recentered:

	int C = 0;
	////////////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < N_entries; i++){ //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<EVENT LOOP
	
	    	ZDCDigiTree->GetEntry(i);
	
	    	if ( !(i%10000) ) std::cout<<"Events # " << i <<std::endl;
	
	    	Accep_N_Tracks = AccpTrackLeaf->GetValue();
	
	    	if (Accep_N_Tracks < 10){
	    		continue; //If you have less than 10 tracks how can you possibly measure flow consitently...
	    	}
	
	    	numHFTowersPos = NHFPos->GetValue();
		numHFTowersNeg = NHFNeg->GetValue();
	
		if (numHFTowersNeg < 1 or numHFTowersPos < 1){ //(prevents divding by zero)
			continue;
		}
	
		vtx_value = vtxLeaf->GetValue();
	
		//vertex systematics
	
		if (numVertex == 1){
			if (abs(vtx_value) > 15 and abs(vtx_value) < 3 ){
				continue;
			}
		}
		else if (numVertex == 2){ 
			if (abs(vtx_value) > 3){
				continue;
			}
		}
	
	    	CentralityValue = CentralityLeaf->GetValue();
	      centBin = getHiBinFromhiHF(CentralityValue);
	      //C = Centrality_Array_Chooser(centBin);
	      C = ALICE_Centrality_Array_Chooser(centBin);
	
	    
	        //cent range code
	      if (C == 1 or C == 2 or C == 3 or C == 4 or C == 5 or C == 6 or C == 7  or C == 8 
	      	  or C == 9 or C == 10 or C == 11 or C == 12 or C == 13 or C == 14 or C == 15){ // it looks like garbage i know but the code wont work any other way
	      	C = 1;
	      	
	      }
	      else{
	      	C = C;
	      }
	
	      //normal code
	
	      // C = C;
	        
	
	
	/*        if (C == 9){
	        	continue;
	        }*/
	
	    QPOSx_Numerator  = 0;
		QPOSy_Numerator  = 0;
		QPOS_Denominator = 0;
	
		QNEGx_Numerator  = 0;
		QNEGy_Numerator  = 0;
		QNEG_Denominator = 0;
	
		QNEGx = 0;
		QNEGy = 0;
		QPOSx = 0;
		QPOSy = 0;
		
		//////////////////////basic cuts///
		if (TF){
		
	      	RPD_Cuts_Generator(RunNumber, RPD_Cuts);
	      	TF = false;
	    	}
		    //////////////////////////////////
		    //HF
		
		if (RPD_or_HF == "HF"){
	
			double EnergyHF = 0;
			double hfPhi = 0;
	
			for (int numNegHF = 0; numNegHF < numHFTowersNeg; numNegHF++){
			
				EnergyHF = (HFNeg_Energy->at(numNegHF));
				hfPhi = (HFNeg_Phi->at(numNegHF));
			
				QNEGx_Numerator += (cos(hfPhi)*EnergyHF);
				QNEGy_Numerator += (sin(hfPhi)*EnergyHF);
				QNEG_Denominator += (EnergyHF); 
			}
			
			for (int numPosHF = 0; numPosHF < numHFTowersPos; numPosHF++){
			
				EnergyHF = (HFPos_Energy->at(numPosHF));
				hfPhi = (HFPos_Phi->at(numPosHF));
			
				QPOSx_Numerator += (cos(hfPhi)*EnergyHF);
				QPOSy_Numerator += (sin(hfPhi)*EnergyHF);
				QPOS_Denominator += (EnergyHF); 
			}
		}
	
		//RPD
		if (RPD_or_HF == "RPD"){
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
	
	   			/*if (OC_RPD_Data[0][0] == -343 or OC_RPD_Data[1][0] == -343){
	   				if (i != (N_entries - 1) ){
	   			  		continue; //continues event loop
	   			  	}
	   			}
	*/
	   			double OCC_RPD_Data[2][16] = {0};
	  
	   			for (int s = 0; s < 2; s ++){
	   			  if (OC_RPD_Data[s][0] == -343){ //NOTE this can produce arrays that are defualt filled with zeros need logic to not fill if entire array is zeros
	   			    continue;//continues event loop
	   			    for ( int c = 0; c < 16; c++){
	   			        OCC_RPD_Data[s][c] = (OC_RPD_Data[s][c]);
	   			    }
	   			  }
	   			  else{
	   			    for ( int c = 0; c < 16; c++){
	   			      //if (c != 1 or c != 10 or c != 11){
	   			        OCC_RPD_Data[s][c] = (OC_RPD_Data[s][c] * RPD_Weights[s][C][c]/** RPD_Frac[s][c]*/);
	  
	   			      //}
	   			     // if (c == 1 or c == 10 or c == 11 ){ 
	   			     //  OCC_RPD_Data[s][c] = (OC_RPD_Data[0][c] * RPD_Weights[0][c]/** RPD_Frac[0][c]*/);
	
	   			      //}
	   			    }
	   			  }
	   			}
	   			////
	
	   		for (int c = 0; c < 16; c++){
	
	    	  //pos
	    	  QPOSx_Numerator += (OCC_RPD_Data[1][c] * cos(RPD_Block_In_Phi[c]));
	    	  QPOSy_Numerator += (OCC_RPD_Data[1][c] * sin(RPD_Block_In_Phi[c]));
	    	  QPOS_Denominator += OCC_RPD_Data[1][c];
		
	    	  //neg
	    	  QNEGx_Numerator += ((OCC_RPD_Data[0][c]) * cos(RPD_Block_In_Phi[c]));
	    	  QNEGy_Numerator += ((OCC_RPD_Data[0][c]) * sin(RPD_Block_In_Phi[c]));
	    	  QNEG_Denominator += OCC_RPD_Data[0][c];
	
	    	}
	
		}//end if rpd
	
			
		if (RPD_or_HF == "RPD"){
			QNEGx = (/*-1**/QNEGx_Numerator)/QNEG_Denominator;
			QNEGy = (/*-1**/QNEGy_Numerator)/QNEG_Denominator;
			QPOSx = QPOSx_Numerator/QPOS_Denominator;
			QPOSy = QPOSy_Numerator/QPOS_Denominator;
	
		}
		if(RPD_or_HF == "HF"){
	
			QNEGx = (QNEGx_Numerator)/QNEG_Denominator;
			QNEGy = (QNEGy_Numerator)/QNEG_Denominator;
			QPOSx = QPOSx_Numerator/QPOS_Denominator;
			QPOSy = QPOSy_Numerator/QPOS_Denominator;
	
		}
	
	
		if(isnan(QNEGx)){
			continue;
		}
		if(isnan(QNEGy)){
			continue;
		}
		if(isnan(QPOSx)){
			continue;
		}
		if(isnan(QPOSy)){
			continue;
		}
		
		///////////////////////////////////////////////////////////// creating q vectors and recentering
		
		//tprofile 2d fill 4 histo 10 times
		
		//https://arxiv.org/pdf/1910.14406.pdf
			
		if (Recentered == false){
		//	cout << "C " << C << endl;
		//	cout << "QPOSx " << QPOSx << endl;
		
			TP2D_Pre_QNEGx->Fill( C+0.2, 0.2, QNEGx);
			TP2D_Pre_QNEGy->Fill( C+0.2, 0.2, QNEGy); 
			TP2D_Pre_QPOSx->Fill( C+0.2, 0.2, QPOSx);
			TP2D_Pre_QPOSy->Fill( C+0.2, 0.2, QPOSy);
	
			/*if (C == 9){
	
				cout<< "QNEGx " << QNEGx << endl;			
				cout<< "QNEGy " << QNEGy << endl;
				cout<< "QPOSx " << QPOSx << endl;
				cout<< "QPOSy " << QPOSy << endl;
	
				if (isnan(QNEGx) or isnan(QNEGy) or isnan(QPOSx) or isnan(QPOSy)){
					cout << "NAN DETECTED!!!!!!!!\n";
					return;
				}
	
			}*/
	
			
			if (i == (N_entries - 1) ){
				cout <<"RECENTERING!!!...."<< endl;
				Recentered = true;
			
				for (int k = 0; k < 10; k++){
					AVG_QNEGx[k] = TP2D_Pre_QNEGx->GetBinContent(k+1,1.0); //tprofile inehrently stores averages
					AVG_QNEGy[k] = TP2D_Pre_QNEGy->GetBinContent(k+1,1.0);
					AVG_QPOSx[k] = TP2D_Pre_QPOSx->GetBinContent(k+1,1.0);
					AVG_QPOSy[k] = TP2D_Pre_QPOSy->GetBinContent(k+1,1.0);
	
					//cout << AVG_QPOSy[k] << endl;
			
					Bin_Err_TP2D_QNEGx[k] = TP2D_Pre_QNEGx->GetBinError(k+1,1.0);
					Bin_Err_TP2D_QNEGy[k] = TP2D_Pre_QNEGy->GetBinError(k+1,1.0);
					Bin_Err_TP2D_QPOSx[k] = TP2D_Pre_QPOSx->GetBinError(k+1,1.0);
					Bin_Err_TP2D_QPOSy[k] = TP2D_Pre_QPOSy->GetBinError(k+1,1.0);
		
					TP2D_Pre_QNEGx->Fill( k+0.2, 1.2, Bin_Err_TP2D_QNEGx[k]);
					TP2D_Pre_QNEGy->Fill( k+0.2, 1.2, Bin_Err_TP2D_QNEGy[k]); 
					TP2D_Pre_QPOSx->Fill( k+0.2, 1.2, Bin_Err_TP2D_QPOSx[k]);
					TP2D_Pre_QPOSy->Fill( k+0.2, 1.2, Bin_Err_TP2D_QPOSy[k]);
					//cout << Bin_Err_TP2D_QPOSy[k] << endl;
				}
	/*	
				TP2D_Pre_QNEGx->Draw("text");
				TP2D_Pre_QNEGy->Draw("text"); 
				TP2D_Pre_QPOSx->Draw("text");
				TP2D_Pre_QPOSy->Draw("text");*/
		
				goto Restart_Recentered;
			
			}
		
			continue;
		}
		
		if (Recentered == true){
	
			//cout << "YES_2\n";
		
			QNEGx = (QNEGx - AVG_QNEGx[C])/Bin_Err_TP2D_QNEGx[C];
			QNEGy = (QNEGy - AVG_QNEGy[C])/Bin_Err_TP2D_QNEGy[C];
			QPOSx = (QPOSx - AVG_QPOSx[C])/Bin_Err_TP2D_QPOSx[C];
			QPOSy = (QPOSy - AVG_QPOSy[C])/Bin_Err_TP2D_QPOSy[C];
	
	/*		cout << "QNEGx " << QNEGx << endl;
			cout << "QNEGy " << QNEGy << endl;
			cout << "QPOSx " << QPOSx << endl;
			cout << "QPOSy " << QPOSy << endl;*/
		
			TP2D_Post_QNEGx->Fill( C+0.2, 0.2, QNEGx);
			TP2D_Post_QNEGy->Fill( C+0.2, 0.2, QNEGy); 
			TP2D_Post_QPOSx->Fill( C+0.2, 0.2, QPOSx);
			TP2D_Post_QPOSy->Fill( C+0.2, 0.2, QPOSy);
		
			if (i == (N_entries - 1) ){
				for (int k = 0; k < 10; k++){
		
					Bin_Err_TP2D_QNEGx[k] = TP2D_Post_QNEGx->GetBinError(k+1,1.0);
					Bin_Err_TP2D_QNEGy[k] = TP2D_Post_QNEGy->GetBinError(k+1,1.0);
					Bin_Err_TP2D_QPOSx[k] = TP2D_Post_QPOSx->GetBinError(k+1,1.0);
					Bin_Err_TP2D_QPOSy[k] = TP2D_Post_QPOSy->GetBinError(k+1,1.0);
	
	/*				cout << "Bin_Err_TP2D_QNEGx[k] " << Bin_Err_TP2D_QNEGx[k] << endl;
					cout << "Bin_Err_TP2D_QNEGy[k] " << Bin_Err_TP2D_QNEGy[k] << endl;
					cout << "Bin_Err_TP2D_QPOSx[k] " << Bin_Err_TP2D_QPOSx[k] << endl;
					cout << "Bin_Err_TP2D_QPOSy[k] " << Bin_Err_TP2D_QPOSy[k] << endl;*/
		
					TP2D_Post_QNEGx->Fill( k+0.2, 1.2, Bin_Err_TP2D_QNEGx[k]);
					TP2D_Post_QNEGy->Fill( k+0.2, 1.2, Bin_Err_TP2D_QNEGy[k]); 
					TP2D_Post_QPOSx->Fill( k+0.2, 1.2, Bin_Err_TP2D_QPOSx[k]);
					TP2D_Post_QPOSy->Fill( k+0.2, 1.2, Bin_Err_TP2D_QPOSy[k]);
				}
	/*	
				TP2D_Post_QNEGx->Draw("text");
				TP2D_Post_QNEGy->Draw("text"); 
				TP2D_Post_QPOSx->Draw("text");
				TP2D_Post_QPOSy->Draw("text");*/
		
			}
		
		}
		//////////////////////////////////////////////////////////// u vector generation and average denom and num for V1
								//number eta bins will be set by user // 10 bins for cent
				
		double Normalization_Addition = (QNEGx*QPOSx + QNEGy*QPOSy);
	
		//cout << "Normalization_Addition " << Normalization_Addition << endl;
		
		for (int N_Tracks = 0; N_Tracks < Accep_N_Tracks; N_Tracks++){
			///////
			if (N_Tracks == 0){
				memset(Eta_binfilled_Check, 0, sizeof(Eta_binfilled_Check));
				memset(Pt_binfilled_Check, 0, sizeof(Pt_binfilled_Check));
			}
			///////
		
			double Eta_Value = eta->at(N_Tracks);
			double Pt_Value = Pt->at(N_Tracks);
			double Phi_Value = phi->at(N_Tracks);

			if (numEff == 1){
			    WeightValue = (TrackEffFxn.getCorrection(Pt_Value, Eta_Value, centBin));
			}
			else{
			   	WeightValue = 1;
			}
	
	
	
			double Neg_Numerator_Addition = ((cos(Phi_Value)*QNEGx + sin(Phi_Value)*QNEGy)*WeightValue);
			double Pos_Numerator_Addition = ((cos(Phi_Value)*QPOSx + sin(Phi_Value)*QPOSy)*WeightValue); //i think this neg 1 is in the right place IT MAY NOT BE
			//cout << "C " << C << endl;
	/*
			cout << "Neg_Numerator_Addition " << Neg_Numerator_Addition <<endl;
			cout << "Pos_Numerator_Addition " << Pos_Numerator_Addition <<endl;
	*/	
			///////////////////////////////////////////////////////////////////////////////////////
		
			if ( etamin < Eta_Value and Eta_Value < etamax){
				//////////
				int EtaBin = 0;
				for (int m = 0; m < EtaBins; m++){
					//cout << "EtaRange L " << EtaRanges[m] << " EtaRange H " << EtaRanges[m+1] << endl;
					if (EtaRanges[m] <= Eta_Value and Eta_Value <  EtaRanges[m+1]){
						EtaBin = m;
						//cout << "in loop EtaBin " << EtaBin << endl;
						break;
					}
				}
		
				if (Eta_binfilled_Check[EtaBin] == 0){ //only want to bin once per event
					Eta_binfilled_Check[EtaBin] = 1;
		
					V1_Eta_Pre_Denominator->Fill(Eta_Value, C+ 0.2, Normalization_Addition);
				}
				//////////
		
				V1_Neg_Eta_Pre_Numerator->Fill(Eta_Value, C+ 0.2, Neg_Numerator_Addition);
				V1_Pos_Eta_Pre_Numerator->Fill(Eta_Value, C+ 0.2, Pos_Numerator_Addition);
	
				//cout << "EtaBin " << EtaBin << endl;
				//cout << "EtaValue " << Eta_Value << endl;
			}
			///////////////////////////////////////////////////////////////////////////////////////
			
			if ( Ptmin < Pt_Value and Pt_Value < Ptmax){
				//////////
				int PtBin = 0;
				for (int m = 0; m < PtBins; m++){
					if (PtRanges[m] <= Pt_Value and Pt_Value <  PtRanges[m+1]){
						PtBin = m;
						break;
					}
				}
		
				if (Pt_binfilled_Check[PtBin] == 0){ //only want to bin once per event
					Pt_binfilled_Check[PtBin] = 1;
		
					V1_Pt_Pre_Denominator->Fill(Pt_Value, C+ 0.2, Normalization_Addition);
				}
				//////////
		
				V1_Neg_Pt_Pre_Numerator->Fill(Pt_Value, C+ 0.2, Neg_Numerator_Addition);
				V1_Pos_Pt_Pre_Numerator->Fill(Pt_Value, C+ 0.2, Pos_Numerator_Addition);
		
			}
			///////////////////////////////////////////////////////////////////////////////////////
		} // track loop
	}// event loop

	for (int Cent = 0; Cent < 10; Cent++){

		double V1_Eta_Post_Denominator_Value = 0;
		double V1_Neg_Eta_Pre_Numerator_Value = 0;
		double V1_Pos_Eta_Pre_Numerator_Value = 0;

		double V1_Odd = 0;

		for (int N_EtaBin = 0; N_EtaBin < EtaBins; N_EtaBin++){
			V1_Eta_Post_Denominator_Value = sqrt(abs(V1_Eta_Pre_Denominator->GetBinContent(N_EtaBin+1, Cent + 1)));
			V1_Neg_Eta_Pre_Numerator_Value = V1_Neg_Eta_Pre_Numerator->GetBinContent(N_EtaBin+1, Cent + 1);
			V1_Pos_Eta_Pre_Numerator_Value = V1_Pos_Eta_Pre_Numerator->GetBinContent(N_EtaBin+1, Cent + 1);

			if (numRPD_or_HF == 0){
	
				V1_Odd = -1*((V1_Pos_Eta_Pre_Numerator_Value/V1_Eta_Post_Denominator_Value) + (V1_Neg_Eta_Pre_Numerator_Value/V1_Eta_Post_Denominator_Value))/2; //(one side has to be inverted to face right direction, its still addition)
			}
			else{
				V1_Odd = ((V1_Pos_Eta_Pre_Numerator_Value/V1_Eta_Post_Denominator_Value) - (V1_Neg_Eta_Pre_Numerator_Value/V1_Eta_Post_Denominator_Value))/2; //do not touch rfor hf has to be like this countering a bug.
			}

			cout << "V1_Odd " << V1_Odd << endl;
	
			V1_Odd_Histo[Cent]->Fill((EtaRanges[N_EtaBin]+EtaRanges[N_EtaBin])/2, V1_Odd);
		}

		double V1_Pt_Pre_Denominator_Value = 0;
		double V1_Neg_Pt_Pre_Numerator_Value = 0;
		double V1_Pos_Pt_Pre_Numerator_Value = 0;

		double V1_Even = 0;

		for (int N_PtBin = 0; N_PtBin < PtBins; N_PtBin++){
			V1_Pt_Pre_Denominator_Value = sqrt(abs(V1_Pt_Pre_Denominator->GetBinContent(N_PtBin+1, Cent + 1)));
			V1_Neg_Pt_Pre_Numerator_Value = V1_Neg_Pt_Pre_Numerator->GetBinContent(N_PtBin+1, Cent + 1);
			V1_Pos_Pt_Pre_Numerator_Value = V1_Pos_Pt_Pre_Numerator->GetBinContent(N_PtBin+1, Cent + 1);
		
			V1_Even = ((V1_Pos_Pt_Pre_Numerator_Value/V1_Pt_Pre_Denominator_Value) + (V1_Neg_Pt_Pre_Numerator_Value/V1_Pt_Pre_Denominator_Value))/2; //fixes a bug

			if (RPD_or_HF == "HF"){
				V1_Even = -1*V1_Even;
			}

			cout << "V1_Even " << V1_Even << endl;

			V1_Even_Histo[Cent]->Fill((PtRanges[N_PtBin]+PtRanges[N_PtBin+1])/2, V1_Even);
		}
	}//filling plots

	myFile->Write();

	TCanvas* D = new TCanvas("D", "D", 3000, 2000);
	gStyle->SetOptStat(0);

	TP2D_Pre_QNEGx->Draw("text");
	D->SaveAs(Form("/data2/users/ebadams/Eric_SP_Data_Sets/%s/%d/PNG/%s/%s/FivetoEighty_RunNumber_%d_Sys_%s_Vtx_%s_%s_TP2D_Pre_QNEGx.png", RPD_or_HF.c_str(), RunNumber, Systematics.c_str(), Vertex.c_str(), RunNumber, Systematics.c_str(), Vertex.c_str(), RPD_or_HF.c_str()));
	TP2D_Pre_QNEGx->SaveAs(Form("/data2/users/ebadams/Eric_SP_Data_Sets/%s/%d/ROOT/%s/%s/FivetoEighty_RunNumber_%d_Sys_%s_Vtx_%s_%s_TP2D_Pre_QNEGx.root", RPD_or_HF.c_str(), RunNumber, Systematics.c_str(), Vertex.c_str(), RunNumber, Systematics.c_str(), Vertex.c_str(), RPD_or_HF.c_str()));
	TP2D_Pre_QNEGy->Draw("text");
	D->SaveAs(Form("/data2/users/ebadams/Eric_SP_Data_Sets/%s/%d/PNG/%s/%s/FivetoEighty_RunNumber_%d_Sys_%s_Vtx_%s_%s_TP2D_Pre_QNEGy.png", RPD_or_HF.c_str(), RunNumber, Systematics.c_str(), Vertex.c_str(), RunNumber, Systematics.c_str(), Vertex.c_str(), RPD_or_HF.c_str()));
	TP2D_Pre_QNEGy->SaveAs(Form("/data2/users/ebadams/Eric_SP_Data_Sets/%s/%d/ROOT/%s/%s/FivetoEighty_RunNumber_%d_Sys_%s_Vtx_%s_%s_TP2D_Pre_QNEGy.root", RPD_or_HF.c_str(), RunNumber, Systematics.c_str(), Vertex.c_str(), RunNumber, Systematics.c_str(), Vertex.c_str(), RPD_or_HF.c_str()));
	TP2D_Pre_QPOSx->Draw("text");
	D->SaveAs(Form("/data2/users/ebadams/Eric_SP_Data_Sets/%s/%d/PNG/%s/%s/FivetoEighty_RunNumber_%d_Sys_%s_Vtx_%s_%s_TP2D_Pre_QPOSx.png", RPD_or_HF.c_str(), RunNumber, Systematics.c_str(), Vertex.c_str(), RunNumber, Systematics.c_str(), Vertex.c_str(), RPD_or_HF.c_str()));
	TP2D_Pre_QPOSx->SaveAs(Form("/data2/users/ebadams/Eric_SP_Data_Sets/%s/%d/ROOT/%s/%s/FivetoEighty_RunNumber_%d_Sys_%s_Vtx_%s_%s_TP2D_Pre_QPOSx.root", RPD_or_HF.c_str(), RunNumber, Systematics.c_str(), Vertex.c_str(), RunNumber, Systematics.c_str(), Vertex.c_str(), RPD_or_HF.c_str()));
	TP2D_Pre_QPOSy->Draw("text");
	D->SaveAs(Form("/data2/users/ebadams/Eric_SP_Data_Sets/%s/%d/PNG/%s/%s/FivetoEighty_RunNumber_%d_Sys_%s_Vtx_%s_%s_TP2D_Pre_QPOSy.png", RPD_or_HF.c_str(), RunNumber , Systematics.c_str(), Vertex.c_str(), RunNumber, Systematics.c_str(), Vertex.c_str(), RPD_or_HF.c_str()));
	TP2D_Pre_QPOSy->SaveAs(Form("/data2/users/ebadams/Eric_SP_Data_Sets/%s/%d/ROOT/%s/%s/FivetoEighty_RunNumber_%d_Sys_%s_Vtx_%s_%s_TP2D_Pre_QPOSy.root", RPD_or_HF.c_str(), RunNumber , Systematics.c_str(), Vertex.c_str(), RunNumber, Systematics.c_str(), Vertex.c_str(), RPD_or_HF.c_str()));
	TP2D_Post_QNEGx->Draw("text");
	D->SaveAs(Form("/data2/users/ebadams/Eric_SP_Data_Sets/%s/%d/PNG/%s/%s/FivetoEighty_RunNumber_%d_Sys_%s_Vtx_%s_%s_TP2D_Post_QNEGx.png", RPD_or_HF.c_str(), RunNumber , Systematics.c_str(), Vertex.c_str(), RunNumber, Systematics.c_str(), Vertex.c_str(), RPD_or_HF.c_str()));
	TP2D_Post_QNEGx->SaveAs(Form("/data2/users/ebadams/Eric_SP_Data_Sets/%s/%d/ROOT/%s/%s/FivetoEighty_RunNumber_%d_Sys_%s_Vtx_%s_%s_TP2D_Post_QNEGx.root", RPD_or_HF.c_str(), RunNumber , Systematics.c_str(), Vertex.c_str(), RunNumber, Systematics.c_str(), Vertex.c_str(), RPD_or_HF.c_str()));
	TP2D_Post_QNEGy->Draw("text");
	D->SaveAs(Form("/data2/users/ebadams/Eric_SP_Data_Sets/%s/%d/PNG/%s/%s/FivetoEighty_RunNumber_%d_Sys_%s_Vtx_%s_%s_TP2D_Post_QNEGy.png", RPD_or_HF.c_str(), RunNumber , Systematics.c_str(), Vertex.c_str(), RunNumber, Systematics.c_str(), Vertex.c_str(), RPD_or_HF.c_str()));
	TP2D_Post_QNEGy->SaveAs(Form("/data2/users/ebadams/Eric_SP_Data_Sets/%s/%d/ROOT/%s/%s/FivetoEighty_RunNumber_%d_Sys_%s_Vtx_%s_%s_TP2D_Post_QNEGy.root", RPD_or_HF.c_str(), RunNumber , Systematics.c_str(), Vertex.c_str(), RunNumber, Systematics.c_str(), Vertex.c_str(), RPD_or_HF.c_str()));
	TP2D_Post_QPOSx->Draw("text");
	D->SaveAs(Form("/data2/users/ebadams/Eric_SP_Data_Sets/%s/%d/PNG/%s/%s/FivetoEighty_RunNumber_%d_Sys_%s_Vtx_%s_%s_TP2D_Post_QPOSx.png", RPD_or_HF.c_str(), RunNumber , Systematics.c_str(), Vertex.c_str(), RunNumber, Systematics.c_str(), Vertex.c_str(), RPD_or_HF.c_str()));
	TP2D_Post_QPOSx->SaveAs(Form("/data2/users/ebadams/Eric_SP_Data_Sets/%s/%d/ROOT/%s/%s/FivetoEighty_RunNumber_%d_Sys_%s_Vtx_%s_%s_TP2D_Post_QPOSx.root", RPD_or_HF.c_str(), RunNumber , Systematics.c_str(), Vertex.c_str(), RunNumber, Systematics.c_str(), Vertex.c_str(), RPD_or_HF.c_str()));
	TP2D_Post_QPOSy->Draw("text");
	D->SaveAs(Form("/data2/users/ebadams/Eric_SP_Data_Sets/%s/%d/PNG/%s/%s/FivetoEighty_RunNumber_%d_Sys_%s_Vtx_%s_%s_TP2D_Post_QPOSy.png", RPD_or_HF.c_str(), RunNumber , Systematics.c_str(), Vertex.c_str(), RunNumber, Systematics.c_str(), Vertex.c_str(), RPD_or_HF.c_str()));
	TP2D_Post_QPOSy->SaveAs(Form("/data2/users/ebadams/Eric_SP_Data_Sets/%s/%d/ROOT/%s/%s/FivetoEighty_RunNumber_%d_Sys_%s_Vtx_%s_%s_TP2D_Post_QPOSy.root", RPD_or_HF.c_str(), RunNumber , Systematics.c_str(), Vertex.c_str(), RunNumber, Systematics.c_str(), Vertex.c_str(), RPD_or_HF.c_str()));
	V1_Eta_Pre_Denominator->Draw("text");
	D->SaveAs(Form("/data2/users/ebadams/Eric_SP_Data_Sets/%s/%d/PNG/%s/%s/FivetoEighty_RunNumber_%d_Sys_%s_Vtx_%s_%s_V1_Eta_Pre_Denominator.png", RPD_or_HF.c_str(), RunNumber, Systematics.c_str(), Vertex.c_str(), RunNumber, Systematics.c_str(), Vertex.c_str(), RPD_or_HF.c_str()));
	V1_Eta_Pre_Denominator->SaveAs(Form("/data2/users/ebadams/Eric_SP_Data_Sets/%s/%d/ROOT/%s/%s/FivetoEighty_RunNumber_%d_Sys_%s_Vtx_%s_%s_V1_Eta_Pre_Denominator.root", RPD_or_HF.c_str(), RunNumber , Systematics.c_str(), Vertex.c_str(), RunNumber, Systematics.c_str(), Vertex.c_str(), RPD_or_HF.c_str()));
	V1_Neg_Eta_Pre_Numerator->Draw("text");
	D->SaveAs(Form("/data2/users/ebadams/Eric_SP_Data_Sets/%s/%d/PNG/%s/%s/FivetoEighty_RunNumber_%d_Sys_%s_Vtx_%s_%s_V1_Neg_Eta_Pre_Numerator.png", RPD_or_HF.c_str(), RunNumber , Systematics.c_str(), Vertex.c_str(), RunNumber, Systematics.c_str(), Vertex.c_str(), RPD_or_HF.c_str()));
	V1_Neg_Eta_Pre_Numerator->SaveAs(Form("/data2/users/ebadams/Eric_SP_Data_Sets/%s/%d/ROOT/%s/%s/FivetoEighty_RunNumber_%d_Sys_%s_Vtx_%s_%s_V1_Neg_Eta_Pre_Numerator.root", RPD_or_HF.c_str(), RunNumber , Systematics.c_str(), Vertex.c_str(), RunNumber, Systematics.c_str(), Vertex.c_str(), RPD_or_HF.c_str()));
	V1_Pos_Eta_Pre_Numerator->Draw("text");
	D->SaveAs(Form("/data2/users/ebadams/Eric_SP_Data_Sets/%s/%d/PNG/%s/%s/FivetoEighty_RunNumber_%d_Sys_%s_Vtx_%s_%s_V1_Pos_Eta_Pre_Numerator.png", RPD_or_HF.c_str(), RunNumber , Systematics.c_str(), Vertex.c_str(), RunNumber, Systematics.c_str(), Vertex.c_str(), RPD_or_HF.c_str()));
	V1_Pos_Eta_Pre_Numerator->SaveAs(Form("/data2/users/ebadams/Eric_SP_Data_Sets/%s/%d/ROOT/%s/%s/FivetoEighty_RunNumber_%d_Sys_%s_Vtx_%s_%s_V1_Pos_Eta_Pre_Numerator.root", RPD_or_HF.c_str(), RunNumber , Systematics.c_str(), Vertex.c_str(), RunNumber, Systematics.c_str(), Vertex.c_str(), RPD_or_HF.c_str()));
	V1_Pt_Pre_Denominator->Draw("text");
	D->SaveAs(Form("/data2/users/ebadams/Eric_SP_Data_Sets/%s/%d/PNG/%s/%s/FivetoEighty_RunNumber_%d_Sys_%s_Vtx_%s_%s_V1_Pt_Pre_Denominator.png", RPD_or_HF.c_str(), RunNumber , Systematics.c_str(), Vertex.c_str(), RunNumber, Systematics.c_str(), Vertex.c_str(), RPD_or_HF.c_str()));
	V1_Pt_Pre_Denominator->SaveAs(Form("/data2/users/ebadams/Eric_SP_Data_Sets/%s/%d/ROOT/%s/%s/FivetoEighty_RunNumber_%d_Sys_%s_Vtx_%s_%s_V1_Pt_Pre_Denominator.root", RPD_or_HF.c_str(), RunNumber , Systematics.c_str(), Vertex.c_str(), RunNumber, Systematics.c_str(), Vertex.c_str(), RPD_or_HF.c_str()));
	V1_Neg_Pt_Pre_Numerator->Draw("text");
	D->SaveAs(Form("/data2/users/ebadams/Eric_SP_Data_Sets/%s/%d/PNG/%s/%s/FivetoEighty_RunNumber_%d_Sys_%s_Vtx_%s_%s_V1_Neg_Pt_Pre_Numerator.png", RPD_or_HF.c_str(), RunNumber , Systematics.c_str(), Vertex.c_str(), RunNumber, Systematics.c_str(), Vertex.c_str(), RPD_or_HF.c_str()));
	V1_Neg_Pt_Pre_Numerator->SaveAs(Form("/data2/users/ebadams/Eric_SP_Data_Sets/%s/%d/ROOT/%s/%s/FivetoEighty_RunNumber_%d_Sys_%s_Vtx_%s_%s_V1_Neg_Pt_Pre_Numerator.root", RPD_or_HF.c_str(), RunNumber , Systematics.c_str(), Vertex.c_str(), RunNumber, Systematics.c_str(), Vertex.c_str(), RPD_or_HF.c_str()));
	V1_Pos_Pt_Pre_Numerator->Draw("text");
	D->SaveAs(Form("/data2/users/ebadams/Eric_SP_Data_Sets/%s/%d/PNG/%s/%s/FivetoEighty_RunNumber_%d_Sys_%s_Vtx_%s_%s_V1_Pos_Pt_Pre_Numerator.png", RPD_or_HF.c_str(), RunNumber , Systematics.c_str(), Vertex.c_str(), RunNumber, Systematics.c_str(), Vertex.c_str(), RPD_or_HF.c_str()));
	V1_Pos_Pt_Pre_Numerator->SaveAs(Form("/data2/users/ebadams/Eric_SP_Data_Sets/%s/%d/ROOT/%s/%s/FivetoEighty_RunNumber_%d_Sys_%s_Vtx_%s_%s_V1_Pos_Pt_Pre_Numerator.root", RPD_or_HF.c_str(), RunNumber , Systematics.c_str(), Vertex.c_str(), RunNumber, Systematics.c_str(), Vertex.c_str(), RPD_or_HF.c_str()));
	
	
	for (int d = 0; d < 10; d++){
		V1_Even_Histo[d]->Draw("HIST E0");
		V1_Even_Histo[d]->SetMarkerStyle(20);
		V1_Even_Histo[d]->SetMarkerSize(3);
		D->SaveAs(Form("/data2/users/ebadams/Eric_SP_Data_Sets/%s/%d/PNG/%s/%s/RunNumber_%d_Sys_%s_Vtx_%s_%s_V1_Even_Histo_5-80_%s.png", RPD_or_HF.c_str(), RunNumber,  Systematics.c_str(), Vertex.c_str(),RunNumber, Systematics.c_str(), Vertex.c_str(), RPD_or_HF.c_str(), centralitystring[d].c_str()));
		V1_Even_Histo[d]->SaveAs(Form("/data2/users/ebadams/Eric_SP_Data_Sets/%s/%d/ROOT/%s/%s/RunNumber_%d_Sys_%s_Vtx_%s_%s_V1_Even_Histo_5-80_%s.root", RPD_or_HF.c_str(), RunNumber,  Systematics.c_str(), Vertex.c_str(),RunNumber, Systematics.c_str(), Vertex.c_str(), RPD_or_HF.c_str(), centralitystring[d].c_str()));
		V1_Odd_Histo[d]->Draw("HIST E0"); 
		V1_Odd_Histo[d]->SetMarkerStyle(20);
		V1_Odd_Histo[d]->SetMarkerSize(3);
		D->SaveAs(Form("/data2/users/ebadams/Eric_SP_Data_Sets/%s/%d/PNG/%s/%s/RunNumber_%d_Sys_%s_Vtx_%s_%s_V1_Odd_Histo_5-80_%s.png", RPD_or_HF.c_str(), RunNumber,  Systematics.c_str(), Vertex.c_str(),RunNumber, Systematics.c_str(), Vertex.c_str(), RPD_or_HF.c_str(), centralitystring[d].c_str()));
		V1_Odd_Histo[d]->SaveAs(Form("/data2/users/ebadams/Eric_SP_Data_Sets/%s/%d/ROOT/%s/%s/RunNumber_%d_Sys_%s_Vtx_%s_%s_V1_Odd_Histo_5-80_%s.root", RPD_or_HF.c_str(), RunNumber,  Systematics.c_str(), Vertex.c_str(),RunNumber, Systematics.c_str(), Vertex.c_str(), RPD_or_HF.c_str(), centralitystring[d].c_str()));
	}

	return 0;
}


