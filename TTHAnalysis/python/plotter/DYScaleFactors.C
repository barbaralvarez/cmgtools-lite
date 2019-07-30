#include "TFile.h"
#include "TMath.h"
#include "TH2D.h"
#include <iostream>
#include <sstream>
#include <iomanip>      // std::setprecision

using namespace std;

//main function
void DYScaleFactors(){
  TFile *file = new TFile("../../data/leptonSF/looseToTight_2017_m_2lss.root");
  TH2F * histo = (TH2F*)file->Get("EGamma_SF2D"); 
  
  //x axis eta
  Double_t xCenter =  -999;
  Double_t xDown   =  -999;
  Double_t xUp     =  -999;
  Double_t xnbins  =  -999;
  //y axis pt
  Double_t yCenter =  -999;
  Double_t yDown   =  -999;
  Double_t yUp     =  -999;
  Double_t ynbins  =  -999;
  
  Int_t binx = histo->GetXaxis()->FindBin(0.89);
  cout << "BinX " <<binx << endl;
  
  /*
  for(Int_t i=1; i<xnbins+1;i++){
    for(Int_t j=1; j<ynbins+1;j++){
      //x axis
      xCenter =  histo->GetXaxis()->GetBinCenter(i);
      xDown   =  histo->GetXaxis()->GetBinLowEdge(i);
      xUp     =  histo->GetXaxis()->GetBinUpEdge(i);
      //y axis
      yCenter =  histo->GetYaxis()->GetBinCenter(j);
      yDown   =  histo->GetYaxis()->GetBinLowEdge(j);
      yUp     =  histo->GetYaxis()->GetBinUpEdge(j);
      
      cout << ">>> X down = " <<  xDown << ", center " << xCenter << ", x up " << xUp << endl;
      cout << ">>> Y down = " <<  yDown << ", center " << yCenter << ", y up " << yUp << endl;
      cout << " BinContent[" << i << "][" << j << "] = " << histo->GetBinContent(i,j) << endl;
    }//loop j
  }//loop i
  */

}
