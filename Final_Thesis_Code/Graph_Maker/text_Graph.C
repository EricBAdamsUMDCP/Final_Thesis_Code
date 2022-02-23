// pre v1 plotter no resolution!

#include "TCanvas.h"
#include "TDirectory.h"
#include "TFile.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TGraphMultiErrors.h"
#include "TH1.h"
#include "TH2.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TMath.h"
#include "TPaveText.h"
#include <iostream>

void test_Graph(){// w sys
   auto c47 = new TCanvas("c47","c47",200,10,600,400); //10-60 RPD SPM
   double ax[7]      = {-1.071, -0.713, -0.356, 0, 0.356, 0.713, 1.071};
   double ay[7]      = {0.0149684,   0.00985934,  0.00427526,  0.000267031, -0.00571449, -0.0123885,  -0.0164157};
   double aexl[7]    = {0.357, 0.357, 0.357, 0.357, 0.357, 0.357, 0.357};
   double aexh[7]    = {0.0202636, 0.0110084, 0.0589274, 0.121075, 0.142514, 0.0417976, 0.0837603};
   double* aeylstat = new double[7]  {0.357, 0.357, 0.357, 0.357, 0.357, 0.357, 0.357};
   double* aeyhstat = new double[7]  {0.357, 0.357, 0.357, 0.357, 0.357, 0.357, 0.357};
   double* aeylsys  = new double[7]  {0.357, 0.357, 0.357, 0.357, 0.357, 0.357, 0.357};
   double* aeyhsys  = new double[7]  {0.357, 0.357, 0.357, 0.357, 0.357, 0.357, 0.357};
 
   auto* gme = new TGraphMultiErrors("gme", "TGraphMultiErrors Example", 5, ax, ay, aexl, aexh, aeylstat, aeyhstat);
   gme->AddYError(7, aeylsys, aeyhsys);
   gme->SetMarkerStyle(21);
   gme->SetLineColor(kRed);
   gme->GetAttLine(0)->SetLineColor(kRed);
   gme->GetAttLine(1)->SetLineColor(kBlue);
   gme->GetAttFill(1)->SetFillStyle(0);
 
   gme->Draw("a p s ; ; 5 s=0.5");


   //c->Clear()
}
/*
-1.25, -0.892857, -0.535714, -0.178571, 0.178571, 0.535714, 0.892857, 1.25

0
0.5
1
1.5
2
2.5
3
3.5
4
4.5
5
5.5
6
6.5
7
7.5
8
*/