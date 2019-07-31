#include "TFile.h"
#include "TMath.h"
#include "TH1D.h"
#include "TH2D.h"
#include <iostream>
#include <sstream>
#include <iomanip>      // std::setprecision

using namespace std;

Double_t Nevents(TString sample, TFile* file, Int_t nbin);
void GetBin(TString sample, TFile* file, Int_t nbin, TString year, TString loose);

//main function
void ttScaleFactors(TString year="2017", TString loose="eTmuL", TString tight="eTmuT", TString var= "muon_pt", Int_t nbin=3){
  //**************************
  //********ingredients*******
  //**************************
  //opposite-sign
  TString SS   = "_SS";
  TString ttname        = var+"_TT";
  TString bkgname       = var+"_background";
  TString dataname      = var+"_data";
  //same-sign
  TString ttname_SS        = var+"_TT"+SS;
  TString stname_SS        = var+"_SingleTop"+SS;
  TString wjetsname_SS     = var+"_WJets"+SS;
  TString dyname_SS        = var+"_DY"+SS;
  TString wwname_SS        = var+"_WW"+SS;
  TString ttwname_SS       = var+"_TTW"+SS;
  TString ttzname_SS       = var+"_TTZ"+SS;
  TString dataname_SS      = var+"_data"+SS;

  //ttbar fakes
  TString fakesttname    = var+"_Fakes_TT";
  TString fakesttname_SS = var+"_Fakes_TT"+SS;

  //Files loose selection and tight selection
  TFile *fileLoose = new TFile(loose+"_data/"+year+"/"+loose+"_data/testplots.root");
  TFile *fileTight = new TFile(tight+"_data/"+year+"/"+tight+"_data/testplots.root");
  //*****************************
  //data  + stat. errors
  Double_t den_data =  Nevents(dataname, fileLoose, nbin);       Double_t err_den_data =  TMath::Sqrt(den_data); 
  Double_t num_data =  Nevents(dataname, fileTight, nbin);       Double_t err_num_data =  TMath::Sqrt(num_data); 
  //data SS + stat. errors
  Double_t den_data_SS =  Nevents(dataname_SS, fileLoose, nbin); Double_t err_den_data_SS =  TMath::Sqrt(den_data_SS); 
  Double_t num_data_SS =  Nevents(dataname_SS, fileTight, nbin); Double_t err_num_data_SS =  TMath::Sqrt(num_data_SS);

  //*****************************
  //ttbar 
  Double_t den_tt = Nevents(ttname, fileLoose, nbin);            Double_t err_den_tt = TMath::Sqrt(den_tt); 
  Double_t num_tt = Nevents(ttname, fileTight, nbin);            Double_t err_num_tt = TMath::Sqrt(num_tt); 

  //*****************************
  //ttbar fakes
  Double_t den_fakes_tt = Nevents(fakesttname, fileLoose, nbin); Double_t err_den_fakes_tt = TMath::Sqrt(den_fakes_tt);
  Double_t num_fakes_tt = Nevents(fakesttname, fileTight, nbin); Double_t err_num_fakes_tt = TMath::Sqrt(num_fakes_tt);
  //ttbar fakes SS
  Double_t den_fakes_tt_SS = Nevents(fakesttname_SS, fileLoose, nbin);Double_t err_den_fakes_tt_SS = TMath::Sqrt(den_fakes_tt_SS);
  Double_t num_fakes_tt_SS = Nevents(fakesttname_SS, fileTight, nbin);Double_t err_num_fakes_tt_SS = TMath::Sqrt(num_fakes_tt_SS);

  //fakes tt scale factor
  Double_t den_fakes_tt_SF = den_fakes_tt/den_fakes_tt_SS;
  Double_t err_den_fakes_tt_SF = den_fakes_tt_SF*((err_den_fakes_tt/den_fakes_tt)+(err_den_fakes_tt_SS/den_fakes_tt_SS));
  Double_t num_fakes_tt_SF = num_fakes_tt/num_fakes_tt_SS;
  Double_t err_num_fakes_tt_SF = num_fakes_tt_SF*((err_num_fakes_tt/num_fakes_tt)+(err_num_fakes_tt_SS/num_fakes_tt_SS));

  //prompt SS, TT, DY, WJets, SingleTop, WW
  Double_t den_tt_SS    = Nevents(ttname_SS, fileLoose, nbin);    Double_t err_den_tt_SS = TMath::Sqrt(den_tt_SS);
  Double_t num_tt_SS    = Nevents(ttname_SS, fileTight, nbin);    Double_t err_num_tt_SS = TMath::Sqrt(num_tt_SS);
  Double_t den_st_SS    = Nevents(stname_SS, fileLoose, nbin);    Double_t err_den_st_SS = TMath::Sqrt(den_st_SS);
  Double_t num_st_SS    = Nevents(stname_SS, fileTight, nbin);    Double_t err_num_st_SS = TMath::Sqrt(num_st_SS);
  Double_t den_wjets_SS = Nevents(wjetsname_SS, fileLoose, nbin); Double_t err_den_wjets_SS = TMath::Sqrt(den_wjets_SS);
  Double_t num_wjets_SS = Nevents(wjetsname_SS, fileTight, nbin); Double_t err_num_wjets_SS = TMath::Sqrt(num_wjets_SS);
  Double_t den_dy_SS    = Nevents(dyname_SS, fileLoose, nbin);    Double_t err_den_dy_SS = TMath::Sqrt(den_dy_SS);
  Double_t num_dy_SS    = Nevents(dyname_SS, fileTight, nbin);    Double_t err_num_dy_SS = TMath::Sqrt(num_dy_SS);
  Double_t den_ww_SS    = Nevents(wwname_SS, fileLoose, nbin);    Double_t err_den_ww_SS = TMath::Sqrt(den_ww_SS);
  Double_t num_ww_SS    = Nevents(wwname_SS, fileTight, nbin);    Double_t err_num_ww_SS = TMath::Sqrt(num_ww_SS);
  Double_t den_ttw_SS   = Nevents(ttwname_SS, fileLoose, nbin);    Double_t err_den_ttw_SS = TMath::Sqrt(den_ttw_SS);
  Double_t num_ttw_SS   = Nevents(ttwname_SS, fileTight, nbin);    Double_t err_num_ttw_SS = TMath::Sqrt(num_ttw_SS);
  Double_t den_ttz_SS   = Nevents(ttzname_SS, fileLoose, nbin);    Double_t err_den_ttz_SS = TMath::Sqrt(den_ttz_SS);
  Double_t num_ttz_SS   = Nevents(ttzname_SS, fileTight, nbin);    Double_t err_num_ttz_SS = TMath::Sqrt(num_ttz_SS);

  //total prompt SS
  Double_t den_prompt_SS=den_tt_SS+den_st_SS+den_wjets_SS+den_dy_SS+den_ww_SS+den_ttw_SS+den_ttz_SS;
  Double_t num_prompt_SS=num_tt_SS+num_st_SS+num_wjets_SS+num_dy_SS+num_ww_SS+num_ttw_SS+num_ttz_SS;
  
  //*****************************
  //corrections to data data_corr = data(OS)- ((data SS - prompt SS)*SF(tt fakes))
  Double_t den_data_corr     =  den_data -((den_data_SS - den_prompt_SS)*den_fakes_tt_SF);
  Double_t num_data_corr     =  num_data -((num_data_SS - num_prompt_SS)*num_fakes_tt_SF);
  //to be fixed
  Double_t err_den_data_corr = TMath::Sqrt(TMath::Power(err_den_data,2)+TMath::Power(err_den_data_SS,2)+TMath::Power(err_den_fakes_tt_SF,2));
  Double_t err_num_data_corr = TMath::Sqrt(TMath::Power(err_num_data,2)+TMath::Power(err_num_data_SS,2)+TMath::Power(err_num_fakes_tt_SF,2)); 

  //Calculate MC efficiency
  //efficiencies
  Double_t eff_tt   = num_tt/den_tt;
  //Calculate data efficiency
  //data corrected
  Double_t eff_data_corr = num_data_corr/den_data_corr;
  
  //stat err
  Double_t err_eff_tt   = eff_tt*((err_num_tt/num_tt)+(err_den_tt/den_tt));
  Double_t err_eff_data_corr = eff_data_corr*((err_num_data_corr/num_data_corr)+(err_den_data_corr/den_data_corr));

  //Calculate scale factors and errors
  Double_t SF     = eff_data_corr/eff_tt;
  Double_t err_SF = SF*((err_eff_data_corr/eff_data_corr)+(err_eff_tt/eff_tt));
  
  //output, checks
  /*
  cout << "ttbar den = " << den_tt  << " +/- " << err_den_tt << ", num  "  << num_tt  << " +/- " << err_num_tt<< endl;
  cout << "fakes den = " << den_fakes_tt  << " +/- " << err_den_fakes_tt << ", num  "  << num_fakes_tt  << " +/- " << err_num_fakes_tt<< endl;
  cout << "fakes den = " << den_fakes_tt_SS  << " +/- " << err_den_fakes_tt_SS << ", num  "  << num_fakes_tt_SS  << " +/- " << err_num_fakes_tt_SS<< endl;
  cout << "fakes den SF = " << den_fakes_tt_SF  << " +/- " << err_den_fakes_tt_SF << ", num SF "  << num_fakes_tt_SF  << " +/- " << err_num_fakes_tt_SF<< endl;
  cout << "data den = " << den_data  << " +/- " << err_den_data << ", num  "  << num_data  << " +/- " << err_num_data<< endl;
  cout << "data SS den = " << den_data_SS  << " +/- " << err_den_data_SS << ", num  "  << num_data_SS  << " +/- " << err_num_data_SS<< endl;
  cout << "data corrected den = " << den_data_corr  << " +/- " << err_den_data_corr << ", num  "  << num_data_corr  << " +/- " << err_num_data_corr<< endl;
  */
     
  //print out
  GetBin(dataname, fileLoose, nbin, year, loose);
  cout <<fixed << showpoint << setprecision(3) << "Resuts: tt eff = " << eff_tt  << " +/- " << err_eff_tt 
       << ", data eff " << eff_data_corr << " +/- " << err_eff_data_corr 
       << ", SF = " << SF << " +/- " << err_SF << endl;

  

}
//get events
Double_t Nevents(TString sample, TFile* file, Int_t nbin){
  TH1D *h = (TH1D*)file->Get(sample);
  h->Draw();
  Double_t nevents = h->GetBinContent(nbin);
  //Double_t nevents = h->Integral();
  //cout << "Sample: " << sample << ", number of events: " << nevents << endl;
  return nevents;
}
void GetBin(TString sample, TFile* file, Int_t nbin, TString year, TString loose){
  TH1D *h = (TH1D*)file->Get(sample);
  h->Draw();
  Double_t nevents = h->GetBinContent(nbin);
  Double_t binCenter =  h->GetXaxis()->GetBinCenter(nbin);
  Double_t binDown   =  h->GetXaxis()->GetBinLowEdge(nbin);
  Double_t binUp     =  h->GetXaxis()->GetBinUpEdge(nbin);
  cout << ">>> Binning down = " <<  binDown << ", center " << binCenter << ", up " << binUp << endl;

  /*
  //compare with loose to tight SFs
  TFile *file2D;
  if(loose.Contains("mu")) 
    file2D= new TFile("../../data/leptonSF/looseToTight_"+year+"_m_2lss.root");
  else
    file2D= new TFile("../../data/leptonSF/looseToTight_"+year+"_e_2lss.root");

  TH2D * histo = (TH2D*)file2D->Get("EGamma_SF2D"); 
  
  Int_t binx = histo->GetXaxis()->FindBin(binCenter);
  Int_t biny = histo->GetYaxis()->FindBin(binCenter);

  Double_t xnbins  =  histo->GetXaxis()->GetNbins();
  Double_t ynbins  =  histo->GetYaxis()->GetNbins();

  //printout
  if(sample.Contains("eta")){
    cout << "2D SF "<< binx << endl;    
    for(Int_t j=1; j<ynbins+1;j++){
      //x axis
      cout << "Compare with BinContent[" << binx << "][" << j << "] = " << histo->GetBinContent(binx,j) << endl;
    }//loop j
    
  }else if(sample.Contains("pt")){
    cout << "2D SF "<< biny << endl;
    for(Int_t i=1; i<xnbins+1;i++){
      //x axis
      cout << "Compare with BinContent[" << i << "][" << biny << "] = " << histo->GetBinContent(i,biny) << endl;
    }//loop j
    
  }
  */
}
