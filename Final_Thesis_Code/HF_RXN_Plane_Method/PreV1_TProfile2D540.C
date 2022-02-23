// pre v1 plotter no resolution!
#include "TH1F.h"
#include "TH2D.h"
#include "TProfile.h"
#include "TProfile2D.h"
#include "TMath.h"
#include<iostream>
#include<iomanip>
#include"TFile.h"
#include"TTree.h"
#include"TLeaf.h"
#include "TCanvas.h"
#include "TCanvas.h"
#include "TStyle.h"


#include "/home/ebadams/Final_Thesis_Code/Header/MASTER_HEADER_FINAL.h"
#include "/home/ebadams/CMSSW_10_3_3/src/Tracking_Eff_2018/trackingEfficiency2018PbPb.h"


double EvenResolution[10] = {0};
double OddResolution[10] = {0};

double BinContent = 0;
double BinContentEeta = 0;
double BinContentOeta = 0;
double BinContentEPt = 0;
double BinContentOPt = 0;




void PreV1_TProfile2D540(int runnumber, int EtaBins ,int PtBins, int numSystematics, int numVertex, int numEff){

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

	//V1 Plots
	//v1(even)
	char v1etaevenname[128],v1etaeventitle[128];
	char v1ptevenname[128],v1pteventitle[128];
	//v1(odd)
	char v1etaoddname[128],v1etaoddtitle[128];
	char v1ptoddname[128],v1ptoddtitle[128];

	//PT Centers
	char ptcentername[128],ptcentertitle[128];
	
	std::vector<double>* phi = 0;
	//std::vector<double>* phiError = 0;
	std::vector<double>* eta = 0;
	//std::vector<double>* etaError = 0;
	std::vector<double>* Pt = 0;

	TFile *f = new TFile(Form("/data2/users/ebadams/Eric_HF_RXN_Data_Sets/%d/Sys_%s_RN_%d_Fourier_Flattened_w_centering_RXN_Plane_Tree_and_Tracks_HF_JAIME.root", runnumber, Systematics.c_str(), runnumber));
    //std::cout<<f<<std::endl;
  
    TTree *t = (TTree*)f->Get("RXN_Plane");

  	TLeaf* Even_Flattened_RXNP;
 	Even_Flattened_RXNP = (TLeaf*)t->GetLeaf("Even_Flattened_RXNP");

 	TLeaf* Odd_Flattened_RXNP;
 	Odd_Flattened_RXNP = (TLeaf*)t->GetLeaf("Odd_Flattened_RXNP");
 	 
 	TLeaf* CentralityBinNum;
 	CentralityBinNum = (TLeaf*)t->GetLeaf("CentralityBinNum");

 	TLeaf* AccepNumberofTracks;
 	AccepNumberofTracks = (TLeaf*)t->GetLeaf("numberofAcceptracks");

 	TLeaf* EventNum;
 	EventNum = (TLeaf*)t->GetLeaf("EventNum");

 	TLeaf* vtxLeaf;
 	vtxLeaf = (TLeaf*)t->GetLeaf("vtx_");

 	TLeaf* run;
 	run = (TLeaf*)t->GetLeaf("run");
	
 	t->SetBranchAddress("phi_storage_vector", &phi);
 	 //newTree->Branch("phiError_storage_vector", "std::vector<double>", &phiError_storage_vector);
 	t->SetBranchAddress("eta_storage_vector", &eta);
 	// newTree->Branch("etaError_storage_vector", "std::vector<double>", &etaError_storage_vector);
 	t->SetBranchAddress("Pt_storage_vector", &Pt);
	
	const Int_t nCent=9;//Number of Centrality classes

	string centralitystring[10] = {"0-10%", "10-20%", "20-30%", "30-40%", "40-50%", "50-60%", "60-70%", "70-80%", "80-90%", "90-100%"};
	
	float centlo[nCent] = {0, 10, 20, 30, 40, 50, 60, 70, 80};
	float centhi[nCent] = {10, 20, 30, 40, 50, 60, 70, 80, 90};

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

  	double EPfinalhfodd = 0;
  	double EPfinalhfeven = 0;

  	double EventNumber = 0;
  	double centIter_Array_Value = 0;

  	int AcceptedNumberOfTracks = 0;

  	double Pt_Value = 0;
	double phi_Value = 0;
	double eta_Value =0;
	
	//Create the output ROOT file
	TFile *myFile;
	TDirectory* myPlots;
	myFile = new TFile(Form("/data2/users/ebadams/Eric_HF_RXN_Data_Sets/%d/Sys_%s_%d_HFEP_EPPlottingV1.root", runnumber, Systematics.c_str(), runnumber),"recreate");
	
	//Make Subdirectories for what will follow
	myPlots = myFile->mkdir("Plots");
	myPlots->cd();
	
	TDirectory* v1plots;
	v1plots = myPlots->mkdir("V1Results");
	TDirectory* v1etaoddplots;
	v1etaoddplots = v1plots->mkdir("V1EtaNeg");
	TDirectory* v1etaevenplots;
	v1etaevenplots = v1plots->mkdir("V1EtaPos");
	TDirectory* v1ptevenplots;
	v1ptevenplots = v1plots->mkdir("V1pTPos");
	TDirectory* v1ptoddplots;
	v1ptoddplots = v1plots->mkdir("V1pTNeg");
	TDirectory* v1PreResplots;
	v1PreResplots = v1plots->mkdir("v1Res");
	TDirectory* V1_Evenplots;
	V1_Evenplots = v1plots->mkdir("V1_Even");
	TDirectory* V1_Oddplots;
	V1_Oddplots = v1plots->mkdir("V1_Odd");

	//V1 Plots
	TH1D* V1_Even_Histo[nCent];
	TH1D* V1_Odd_Histo[nCent];


	//V1 Plots
	TH1D *V1Odd[nCent];
	TProfile *V1EtaOdd[nCent];
	TProfile *V1EtaEven[nCent];
	TH1D *V1Even[nCent];
	TProfile *V1PtEven[nCent];
	TProfile *V1PtOdd[nCent];
	//Single Side HF v 1
	TProfile *V1OddHFP[nCent];
	TProfile *V1OddHFM[nCent];
	TProfile *V1EvenHFP[nCent];
	TProfile *V1EvenHFM[nCent];
	
	//PT Bin Centers
	TProfile *PTCenters[nCent];
	//Resolution

	TProfile *Res;
	TProfile *FinalRes;

	TH2D* CompareRXN;

	for (Int_t i=0;i<nCent;i++){		
		/////////////////////////////////////////////////
		//////V1 Plots

		//V1Even->cd();
		V1_Even_Histo[i] = new TH1D(Form("HF_V1_Even_%s", centralitystring[i].c_str()),Form("HF_V1_Even_5-40;  Pt; V1 Magnitude [A.U.]", centralitystring[i].c_str()), PtBins, Ptmin, Ptmax);
	
		//V1Odd->cd();
		V1_Odd_Histo[i] = new TH1D(Form("HF_V1_Odd_%s", centralitystring[i].c_str()), Form("HF_V1_Odd_5-40; Eta; V1 Magnitude [A.U.]", centralitystring[i].c_str()), EtaBins, etamin, etamax);


		//V1 Eta
		
		//Even
		v1etaevenplots->cd();
		sprintf(v1etaevenname,"V1Eta_Even_%1.0lfto%1.0lf",centlo[i],centhi[i]);
		sprintf(v1etaeventitle,"v_{1}^{even}(#eta) %1.0lfto%1.0lf",centlo[i],centhi[i]);
		V1EtaEven[i]= new TProfile(v1etaevenname,v1etaeventitle,EtaBins,EtaRanges);
		//Odd
		v1etaoddplots->cd();
		sprintf(v1etaoddname,"V1Eta_Odd_%1.0lfto%1.0lf",centlo[i],centhi[i]);
		sprintf(v1etaoddtitle,"v_{1}^{odd}(#eta) %1.0lfto%1.0lf",centlo[i],centhi[i]);
		V1EtaOdd[i]= new TProfile(v1etaoddname,v1etaoddtitle,EtaBins,EtaRanges);
/*		V1EtaOdd[i]->SetMaximum(0.011);
		V1EtaOdd[i]->SetMinimum(-0.011);*/
		V1_Oddplots->cd();
		sprintf(v1ptoddname,"V1_Odd_%1.0lfto%1.0lf",centlo[i],centhi[i]);
		sprintf(v1ptoddtitle,"v_{1}^{odd} %1.0lfto%1.0lf;  Pt; V1 Magnitude [A.U.]",centlo[i],centhi[i]);
		V1Odd[i] = new TH1D(v1ptoddname,v1ptoddtitle,EtaBins,EtaRanges);
		
		
		
		//V1 Pt
		
		//Even
		v1ptevenplots->cd();
		sprintf(v1ptevenname,"V1Pt_Even_%1.0lfto%1.0lf",centlo[i],centhi[i]);
		sprintf(v1pteventitle,"v_{1}^{even}(p_{T}) %1.0lfto%1.0lf",centlo[i],centhi[i]);
		V1PtEven[i]= new TProfile(v1ptevenname,v1pteventitle,PtBins,PtRanges);
		//Odd
		v1ptoddplots->cd();
		sprintf(v1ptoddname,"V1Pt_Odd_%1.0lfto%1.0lf",centlo[i],centhi[i]);
		sprintf(v1ptoddtitle,"v_{1}^{odd}(p_{T}) %1.0lfto%1.0lf",centlo[i],centhi[i]);
		V1PtOdd[i]= new TProfile(v1ptoddname,v1ptoddtitle,PtBins,PtRanges);
/*		V1PtOdd[i]->SetMaximum(0.02);
		V1PtOdd[i]->SetMinimum(-0.0EtaBins);*/
		V1_Evenplots->cd();
		sprintf(v1ptevenname,"V1_Even_%1.0lfto%1.0lf",centlo[i],centhi[i]);
		sprintf(v1pteventitle,"v_{1}^{Even} %1.0lfto%1.0lf; Eta; V1 Magnitude [A.U.]",centlo[i],centhi[i]);
		V1Even[i] = new TH1D(v1ptevenname,v1pteventitle,PtBins,PtRanges);
		
		//pT Centers
		v1plots->cd();
		sprintf(ptcentername,"PTCenter_%1.0lfto%1.0lf",centlo[i],centhi[i]);
		sprintf(ptcentertitle,"PTCenter_%1.0lfto%1.0lf",centlo[i],centhi[i]);
		PTCenters[i] = new TProfile(ptcentername,ptcentertitle,PtBins,PtRanges);
	}

	v1PreResplots->cd();
	Res = new TProfile("Pre_RES","V1_PRE_Resolution",10,0,10);
	FinalRes = new TProfile("RES",Form("V1_Resolution_HF_%d; 10 Percent Centrality Bins; #sqrt{<cos(#Psi_{1}^{HF^{+}} - #Psi_{1}^{HF^{-}})>}",runnumber),10,0,10);

	Int_t NumberOfEvents = t->GetEntries();

	CompareRXN = new TH2D("comp","compare pos neg rxn; Neg RXN (radians); Pos RXN (radians)", 100, -3.5, 3.5, 100, -3.5, 3.5);
	//Consumes_TProfile(runnumber);


	// theory of spectator deflection plots

	TProfile* TP_Pos_NumTrk_wo_RXN_vs_Cent = new TProfile("TP_Pos_NumTrk_wo_RXN_vs_Cent","TP_Pos_NumTrk_wo_RP_vs_Cent; Centrality Bins (10%); Values <Sin(#phi - #Psi)>",10,0,10);
	TProfile* TP_Neg_NumTrk_wo_RXN_vs_Cent = new TProfile("TP_Neg_NumTrk_wo_RXN_vs_Cent","TP_Neg_NumTrk_wo_RP_vs_Cent; Centrality Bins (10%); Values <Sin(#phi - #Psi)>",10,0,10);

	TProfile* TP_Pos_NumTrk_wo_RXN_vs_NTrack = new TProfile("TP_Pos_NumTrk_wo_RXN_vs_NTrack","TP_Pos_NumTrk_wo_RP_vs_NTrack; Number of Tracks; Values <Sin(#phi - #Psi)>",50,0,1600); //inverted to force it to auto bin
	TProfile* TP_Neg_NumTrk_wo_RXN_vs_NTrack = new TProfile("TP_Neg_NumTrk_wo_RXN_vs_NTrack","TP_Neg_NumTrk_wo_RP_vs_NTrack; Number of Tracks; Values <Sin(#phi - #Psi)>",50,0,1600);

	TProfile* TP_Cent_vs_NTrack = new TProfile("TP_Cent_vs_NTrack","TP_Cent_vs_NTrack; Centrality Bins (10%); <Number of Tracks>",10,0,10);

	TH2D* PtvsCentrality = new TH2D("PtvsCentrality", "Centrality vs #P_T; Centrality Bins (10%); #P_T (GeV)", 10, 0, 10, 200, 0, 20);
	//


	for (Int_t i=0;i<NumberOfEvents;i++){

		t->GetEntry(i);

		if ( !(i%10000) ) cout << " 4rd round, event # " << i << " / " << NumberOfEvents << endl;


    EventNumber = EventNum->GetValue();
    runnumber = run->GetValue();
    centIter_Array_Value = CentralityBinNum->GetValue(); //getHiBinFromhiHF(CentralityValue);
    //cout << centIter_Array_Value << endl;

    EPfinalhfodd = Odd_Flattened_RXNP->GetValue();
		EPfinalhfeven = Even_Flattened_RXNP->GetValue();

		CompareRXN->Fill(EPfinalhfodd, EPfinalhfeven);

		int C = 0;
		double vtx_value = 0;
		double WeightValue = 0;
		for (Int_t c=0;c<nCent;c++){
      if ( (centIter_Array_Value*6.3) > centhi[c] ) continue;
      if ( (centIter_Array_Value*6.3) < centlo[c] ) continue;	

       //cout << "centIter_Array_Value " << centIter_Array_Value << endl;

        //cent range code
      if (centIter_Array_Value == 1 or centIter_Array_Value == 2  or centIter_Array_Value == 3 or centIter_Array_Value == 4 or centIter_Array_Value == 5 or centIter_Array_Value == 6 or centIter_Array_Value == 7 /*or centIter_Array_Value == 8 
      	  or centIter_Array_Value == 9 or centIter_Array_Value == 10 or centIter_Array_Value == 11 or centIter_Array_Value == 12 or centIter_Array_Value == 13 or centIter_Array_Value == 14 or centIter_Array_Value == 15*/){ // it looks like garbage i know but the code wont work any other way
      	C = 1;
      	
      }
      else{
      	C = c;
      }
       //normal code

      //C = c;

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

			//cout << "C " << C << endl;
			AcceptedNumberOfTracks= AccepNumberofTracks->GetValue();
			for (Int_t ii=0;ii<AcceptedNumberOfTracks;ii++){
			   Pt_Value=0.;
			   phi_Value=0.;
			   eta_Value=0.;
			   Pt_Value=Pt->at(ii);
			   phi_Value=phi->at(ii);
			   eta_Value=eta->at(ii);

			   	if (numEff == 1){
			   		WeightValue = (TrackEffFxn.getCorrection(Pt_Value, eta_Value, centIter_Array_Value*5));
			   	}
			   	else{
			   		WeightValue = 1;
			    }

			   if(Pt_Value<0){
			     continue;
			   }
			   if(fabs(eta_Value)<=1.6 &&Pt_Value>0.4){

			   		 TP_Neg_NumTrk_wo_RXN_vs_Cent->Fill(C+0.2, TMath::Sin(phi_Value-EPfinalhfodd)*WeightValue);
						 TP_Pos_NumTrk_wo_RXN_vs_Cent->Fill(C+0.2, TMath::Sin(phi_Value-EPfinalhfeven)*WeightValue);
						 TP_Pos_NumTrk_wo_RXN_vs_NTrack->Fill(AcceptedNumberOfTracks, TMath::Sin(phi_Value-EPfinalhfodd)*WeightValue);
						 TP_Neg_NumTrk_wo_RXN_vs_NTrack->Fill(AcceptedNumberOfTracks, TMath::Sin(phi_Value-EPfinalhfeven)*WeightValue);
						 TP_Cent_vs_NTrack->Fill(C+0.2, AcceptedNumberOfTracks);
						 PtvsCentrality->Fill(C+0.2, Pt_Value);

			   	   Res->Fill(C, (TMath::Cos(EPfinalhfodd - EPfinalhfeven)));
			       V1EtaOdd[C]->Fill(eta_Value,(TMath::Cos(phi_Value-EPfinalhfodd)*WeightValue/*/OddResolution[C]*/));
			       V1EtaEven[C]->Fill(eta_Value,(TMath::Cos(phi_Value-EPfinalhfeven)*WeightValue/*/EvenResolution[C]*/));
			       V1PtEven[C]->Fill(Pt_Value,(-1*TMath::Cos(phi_Value-EPfinalhfeven)*WeightValue/*/EvenResolution[C]*/));
			       PTCenters[C]->Fill(Pt_Value,Pt_Value);
			       V1PtOdd[C]->Fill(Pt_Value,(TMath::Cos(phi_Value-EPfinalhfodd)*WeightValue/*/OddResolution[c]*/));//can find offset later with removing the eta gate here
			   }//only central tracks
			}
		}
	}

	TCanvas* D = new TCanvas("D", "D", 3000, 2000);
	gStyle->SetOptStat(0);

	for (int q = 1; q < 10; q++){
		BinContent = Res->GetBinContent(q);

		BinContent = sqrt(abs(BinContent));

		cout << "res " << BinContent << endl;

		FinalRes->Fill(q-1,BinContent);

		for (int e = 0; e < EtaBins; e++){

			BinContentEeta = V1EtaEven[q-1]->GetBinContent(e);
			BinContentOeta = V1EtaOdd[q-1]->GetBinContent(e);
			cout << "BinContentOeta "<< BinContentOeta << endl;
			cout << "BinContentEeta "<< BinContentEeta << endl;
			V1Odd[q-1]->Fill(EtaRanges[e],(0.85*BinContentOeta + BinContentEeta)/(2*BinContent)); //must be a bug somewhere changing to addition fixes the slope and the results are good, im guessinf there is some hidden (-) somewhere... oops
			V1_Odd_Histo[q-1]->Fill(EtaRanges[e],(0.85*BinContentOeta + BinContentEeta)/(2*BinContent));

		}
		for (int pt = 0; pt < PtBins; pt++){

			BinContentEPt = V1PtEven[q-1]->GetBinContent(pt);
			BinContentOPt = V1PtOdd[q-1]->GetBinContent(pt);
			cout << "BinConentOPt " << BinContentOPt << endl;
			cout << "BinConentEPt " << BinContentEPt << endl;
			V1Even[q-1]->Fill(PtRanges[pt],(BinContentEPt + BinContentOPt)/(2*BinContent));
			V1_Even_Histo[q-1]->Fill(PtRanges[pt],(BinContentEPt + BinContentOPt)/(2*BinContent));
		}


		TP_Neg_NumTrk_wo_RXN_vs_Cent->Draw("HIST E");
		D->SaveAs(Form("/data2/users/ebadams/Eric_HF_RXN_Data_Sets/%d/PNG/%s/%s/%d_TP_Neg_NumTrk_wo_RXN_vs_Cent_540.png", runnumber, Systematics.c_str(), Vertex.c_str(), runnumber));
		TP_Pos_NumTrk_wo_RXN_vs_Cent->Draw("HIST E");
		D->SaveAs(Form("/data2/users/ebadams/Eric_HF_RXN_Data_Sets/%d/PNG/%s/%s/%d_TP_Pos_NumTrk_wo_RXN_vs_Cent_540.png", runnumber, Systematics.c_str(), Vertex.c_str(), runnumber));

		TP_Pos_NumTrk_wo_RXN_vs_NTrack->Draw("HIST E");
		D->SaveAs(Form("/data2/users/ebadams/Eric_HF_RXN_Data_Sets/%d/PNG/%s/%s/%d_TP_Pos_NumTrk_wo_RXN_vs_NTrack_540.png", runnumber, Systematics.c_str(), Vertex.c_str(), runnumber));
		TP_Neg_NumTrk_wo_RXN_vs_NTrack->Draw("HIST E");
		D->SaveAs(Form("/data2/users/ebadams/Eric_HF_RXN_Data_Sets/%d/PNG/%s/%s/%d_TP_Neg_NumTrk_wo_RXN_vs_NTrack_540.png", runnumber, Systematics.c_str(), Vertex.c_str(), runnumber));

		TP_Cent_vs_NTrack->Draw("HIST E");
		D->SaveAs(Form("/data2/users/ebadams/Eric_HF_RXN_Data_Sets/%d/PNG/%s/%s/%d_TP_Cent_vs_NTrack_540.png", runnumber, Systematics.c_str(), Vertex.c_str(), runnumber));

		PtvsCentrality->Draw("COLZ");
		D->SaveAs(Form("/data2/users/ebadams/Eric_HF_RXN_Data_Sets/%d/PNG/%s/%s/%d_PtvsCentrality_540.png", runnumber, Systematics.c_str(), Vertex.c_str(), runnumber));

		CompareRXN->Draw("COLZ");
		D->SaveAs(Form("/data2/users/ebadams/Eric_HF_RXN_Data_Sets/%d/PNG/%s/%s/%d_compare_all_cent__540.png", runnumber, Systematics.c_str(), Vertex.c_str(), runnumber));
		V1_Even_Histo[q-1]->Draw("HIST E0");
		V1_Even_Histo[q-1]->SetMarkerStyle(20);
		V1_Even_Histo[q-1]->SetMarkerSize(3);
		D->SaveAs(Form("/data2/users/ebadams/Eric_HF_RXN_Data_Sets/%d/PNG/%s/%s/%d_HF_V1_Even_Histo_5-40_%s.png", runnumber, Systematics.c_str(), Vertex.c_str(), runnumber, centralitystring[q-1].c_str()));
		V1_Even_Histo[q-1]->SaveAs(Form("/data2/users/ebadams/Eric_HF_RXN_Data_Sets/%d/ROOT/%s/%s/%d_HF_V1_Even_Histo_5-40_%s.root", runnumber, Systematics.c_str(), Vertex.c_str(), runnumber, centralitystring[q-1].c_str()));
		V1_Odd_Histo[q-1]->Draw("HIST E0"); 
		V1_Odd_Histo[q-1]->SetMarkerStyle(20);
		V1_Odd_Histo[q-1]->SetMarkerSize(3);
		D->SaveAs(Form("/data2/users/ebadams/Eric_HF_RXN_Data_Sets/%d/PNG/%s/%s/%d_HF_V1_Odd_Histo_5-40_%s.png", runnumber, Systematics.c_str(), Vertex.c_str(),runnumber, centralitystring[q-1].c_str()));
		V1_Odd_Histo[q-1]->SaveAs(Form("/data2/users/ebadams/Eric_HF_RXN_Data_Sets/%d/ROOT/%s/%s/%d_HF_V1_Odd_Histo_5-40_%s.root", runnumber, Systematics.c_str(), Vertex.c_str(), runnumber, centralitystring[q-1].c_str()));
 
		

	}
	myFile->Write();
}