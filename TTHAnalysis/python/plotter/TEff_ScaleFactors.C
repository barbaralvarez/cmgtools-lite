#include "TLegend.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TMath.h"
#include "TROOT.h"
#include "TEfficiency.h"
#include "TFile.h"
#include <iostream>
#include <sstream>
#include <iomanip>      // std::setprecision
#include "Math/Math.h"
#include "Math/QuantFuncMathCore.h"


using namespace std;

Double_t normal(Double_t total,Double_t passed);
Double_t GetEfficiencyErrorLow(TH1D* htotal, TH1D* passed, Int_t bin);
TH1D *histo(TString sample, TFile *file);

//main
//void TEff_ScaleFactors(TString year="2017", TString loose="eTmuL", TString tight="eTmuT", TString var= "muon_pt"){
void TEff_ScaleFactors(TString year="2017", TString loose="muTeL", TString tight="muTeT", TString var= "ele_pt"){

  //Files loose selection and tight selection from ttH framework
  TFile *fileLoose = new TFile(loose+"_data/"+year+"/"+loose+"_data/testplots.root");
  TFile *fileTight = new TFile(tight+"_data/"+year+"/"+tight+"_data/testplots.root");
  //DY Scale factors
  TFile *file;
  if(tight=="eTmuT")     file = new TFile("../../data/leptonSF/looseToTight_"+year+"_m_2lss.root");
  else if(tight=="muTeT")file = new TFile("../../data/leptonSF/looseToTight_"+year+"_e_2lss.root");

  //Get the efficiency in data and ttbar from OS and SS events
  //**************************
  //********ingredients*******
  //**************************
  //opposite-sign
  TString ttname        = var+"_TT";
  TString bkgname       = var+"_background";
  TString dataname      = var+"_data";
  //same-sign
  TString SS               = "_SS";
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
  
  //*****************************************************
  //   get the histograms: estimate based on histograms
  //*****************************************************

  //data  
  TH1D* den_data =  histo(dataname, fileLoose); 
  TH1D* num_data =  histo(dataname, fileTight); 
  //data SS 
  TH1D* den_data_SS =  histo(dataname_SS, fileLoose);
  TH1D* num_data_SS =  histo(dataname_SS, fileTight);
  //prompt SS, TT, DY, WJets, SingleTop, WW
  TH1D* den_tt_SS    = histo(ttname_SS, fileLoose); 
  TH1D* num_tt_SS    = histo(ttname_SS, fileTight); 
  TH1D* den_st_SS    = histo(stname_SS, fileLoose); 
  TH1D* num_st_SS    = histo(stname_SS, fileTight); 
  TH1D* den_wjets_SS = histo(wjetsname_SS, fileLoose); 
  TH1D* num_wjets_SS = histo(wjetsname_SS, fileTight); 
  TH1D* den_dy_SS    = histo(dyname_SS, fileLoose);    
  TH1D* num_dy_SS    = histo(dyname_SS, fileTight);    
  TH1D* den_ww_SS    = histo(wwname_SS, fileLoose);    
  TH1D* num_ww_SS    = histo(wwname_SS, fileTight);    
  TH1D* den_ttw_SS   = histo(ttwname_SS, fileLoose);   
  TH1D* num_ttw_SS   = histo(ttwname_SS, fileTight);   
  TH1D* den_ttz_SS   = histo(ttzname_SS, fileLoose);   
  TH1D* num_ttz_SS   = histo(ttzname_SS, fileTight);   

  //sum histograms total prompt SS
  /*
  TH1D* den_prompt_SS = (TH1D*) den_tt_SS->Clone();
  den_prompt_SS->Add(den_st_SS);
  den_prompt_SS->Add(den_wjets_SS);
  den_prompt_SS->Add(den_dy_SS);
  den_prompt_SS->Add(den_ww_SS);
  den_prompt_SS->Add(den_ttw_SS);
  den_prompt_SS->Add(den_ttz_SS);
  TH1D* num_prompt_SS = (TH1D*) num_tt_SS->Clone();
  num_prompt_SS->Add(num_st_SS);
  num_prompt_SS->Add(num_wjets_SS);
  num_prompt_SS->Add(num_dy_SS);
  num_prompt_SS->Add(num_ww_SS);
  num_prompt_SS->Add(num_ttw_SS);
  num_prompt_SS->Add(num_ttz_SS);
  */
  TH1D* den_prompt_SS = (TH1D*) den_ttw_SS->Clone();
  den_prompt_SS->Add(den_ttz_SS);
  TH1D* num_prompt_SS = (TH1D*) num_ttw_SS->Clone();
  num_prompt_SS->Add(num_ttz_SS);

 //*****************************
  //ttbar fakes
  TH1D* den_fakes_tt = histo(fakesttname, fileLoose);
  TH1D* num_fakes_tt = histo(fakesttname, fileTight);
  //ttbar fakes SS
  TH1D* den_fakes_tt_SS = histo(fakesttname_SS, fileLoose);
  TH1D* num_fakes_tt_SS = histo(fakesttname_SS, fileTight);

  //fakes tt scale factor
  TH1D* den_fakes_tt_SF = (TH1D*) den_fakes_tt->Clone();
  den_fakes_tt_SF->Divide(den_fakes_tt_SS);
  TH1D* num_fakes_tt_SF = (TH1D*) num_fakes_tt->Clone();
  num_fakes_tt_SF->Divide(num_fakes_tt_SS);


  //*****************************
  //corrections to data data_corr = data(OS)- ((data SS - prompt SS)*SF(tt fakes))
  TH1D* den_data_SS_corr = (TH1D*) den_data_SS->Clone(); 
  den_data_SS_corr->Add(den_prompt_SS,-1); 
  den_data_SS_corr->Multiply(den_fakes_tt_SF); 
  TH1D* den_data_corr    = (TH1D*) den_data->Clone(); 
  den_data_corr->Add(den_data_SS_corr,-1); 
  TH1D* num_data_SS_corr = (TH1D*) num_data_SS->Clone(); 
  num_data_SS_corr->Add(num_prompt_SS,-1); 
  //num_data_SS_corr->Multiply(num_fakes_tt_SF); 
  //To be fixed: preliminary solution using the den SF to fix error issues, low stat.
  num_data_SS_corr->Multiply(den_fakes_tt_SF); 
  TH1D* num_data_corr    = (TH1D*) num_data->Clone(); 
  num_data_corr->Add(num_data_SS_corr,-1); 

  //draw checks
  //num_fakes_tt_SF->Draw();
  //den_data_SS->Draw("same");
  TEfficiency* eff_data = 0;
  if(TEfficiency::CheckConsistency(*num_data_corr, *den_data_corr)){
  eff_data = new TEfficiency(*num_data_corr,*den_data_corr);
  //if(TEfficiency::CheckConsistency(*num_data, *den_data)){
  //eff_data = new TEfficiency(*num_data,*den_data);
    eff_data->SetStatisticOption(TEfficiency::kFNormal);
    eff_data->SetLineColor(2);
    eff_data->SetLineWidth(2);
  //eff_data->Draw();
  }

  //ttbar 
  TH1D *den_tt = histo(ttname, fileLoose);           
  TH1D *num_tt = histo(ttname, fileTight);           
  TEfficiency* eff_tt = 0;
  if(TEfficiency::CheckConsistency(*num_tt, *den_tt)){
    eff_tt = new TEfficiency(*num_tt,*den_tt);
    eff_tt->SetStatisticOption(TEfficiency::kFNormal);
    eff_tt->SetLineWidth(2);
    //eff_tt->Draw("same");
    //eff_tt->Draw();
  }
  //pT array
  Double_t array[11] = {0,15,30,45,60,75,90,105,120,135,150};
  TH1D* histo_eff_data = new TH1D("histo_eff_data","histo_eff_data", 10, array);
  TH1D* histo_eff_tt   = new TH1D("histo_eff_tt","histo_eff_tt", 10, array);
  //bin by bin SF
  for(Int_t nbin = 1; nbin<11;nbin++){
    //for(Int_t nbin = 0; nbin<11;nbin++){
    double ratio = eff_data->GetEfficiency(nbin)/eff_tt->GetEfficiency(nbin);
    cout << "********Bin " << nbin << "  ********* " << endl;
    cout << "data eff " << eff_data->GetEfficiency(nbin) << ", ttbar eff " << eff_tt->GetEfficiency(nbin) << ", ratio " << ratio<< endl;
    cout << "data num "<< num_data_corr->GetBinContent(nbin)<< " err " << num_data_corr->GetBinError(nbin) <<  ", data den "<< den_data_corr->GetBinContent(nbin)<< " err " << den_data_corr->GetBinError(nbin) <<  endl;
    cout << "tt num "<< num_tt->GetBinContent(nbin)<< " err " << num_tt->GetBinError(nbin) <<  ", data den "<< den_tt->GetBinContent(nbin)<< " err " << den_tt->GetBinError(nbin) <<  endl;
    cout << "data err up " << eff_data->GetEfficiencyErrorUp(nbin) << ", ttbar err up " << eff_tt->GetEfficiencyErrorUp(nbin) <<  endl;
    cout << "data err down " << eff_data->GetEfficiencyErrorLow(nbin) << ", ttbar err up " << eff_tt->GetEfficiencyErrorLow(nbin) <<  endl;
    cout << endl;
    //cout << "data normal " << normal(den_data->GetBinContent(nbin),num_data->GetBinContent(nbin)) << ", tt " << normal(den_tt->GetBinContent(nbin), num_tt->GetBinContent(nbin)) << endl;
    cout << "data checks " << GetEfficiencyErrorLow(den_data, num_data, nbin) << endl;
    cout << "data corrected checks " << GetEfficiencyErrorLow(den_data_corr, num_data_corr, nbin) << endl;
    cout << "Fakes tt fakes: den " << den_fakes_tt->GetBinContent(nbin) << ", num " << num_fakes_tt->GetBinContent(nbin) << " SS den " << den_fakes_tt_SS->GetBinContent(nbin) << ", num " << num_fakes_tt_SS->GetBinContent(nbin) << ", SF " << den_fakes_tt_SF->GetBinContent(nbin) << ", num "<<num_fakes_tt_SF->GetBinContent(nbin) << endl;
    
    //get efficiecny
    histo_eff_data->SetBinContent(nbin,eff_data->GetEfficiency(nbin));
    histo_eff_data->SetBinError(nbin,eff_data->GetEfficiencyErrorLow(nbin));
    histo_eff_tt->SetBinContent(nbin,eff_tt->GetEfficiency(nbin));
    histo_eff_tt->SetBinError(nbin,eff_tt->GetEfficiencyErrorLow(nbin));


  }
  //Draw eff, and SF
  TCanvas* c = new TCanvas("c", "c", 1000, 800); 
  TH1D* ScaleFactors = (TH1D*) histo_eff_data->Clone();
  gStyle->SetOptStat(0);
  ScaleFactors->Divide(histo_eff_tt);
  ScaleFactors->SetMaximum(1.3);
  ScaleFactors->SetMinimum(0.0);
  ScaleFactors->SetLineWidth(2);
  if(var=="muon_pt") ScaleFactors->GetXaxis()->SetTitle("Muon p_{T} [GeV]");
  else ScaleFactors->GetXaxis()->SetTitle("Electron p_{T} [GeV]");
  ScaleFactors->SetTitle("ttbar/data efficiencies and SFs: "+year);
  ScaleFactors->SetMarkerColor(4);
  ScaleFactors->SetLineColor(4);
  ScaleFactors->SetMarkerStyle(22);
  ScaleFactors->Draw("*");
  eff_tt->Draw("same");
  eff_data->Draw("same");
  TLegend* leg = new TLegend(0.6,0.2,0.8,0.4);
  leg->AddEntry(ScaleFactors,"Scale Factors", "l");
  leg->AddEntry(eff_data,"data eff","l");
  leg->AddEntry(eff_tt,"tt MC eff","l");
  leg->Draw("same");
  c->SaveAs("plots/ttSF_"+year+"_"+var+".eps");
  c->SaveAs("plots/ttSF_"+year+"_"+var+".png");
  c->SaveAs("plots/ttSF_"+year+"_"+var+".C");

  TCanvas* cSF = new TCanvas("cSF", "cSF", 1000, 800); 

  //open file
  TH2F * histo = (TH2F*)file->Get("EGamma_SF2D"); 
  
  //x axis eta
  int xnbins  =  histo->GetXaxis()->GetNbins();
  int ynbins  =  histo->GetYaxis()->GetNbins();

  //pT array
  Double_t ptarray[11] = {0,15,20,25,30,35,40,45,60,80,120};

  TH1D* hpt[xnbins+1];
  TString hname[xnbins+1];
  for(int i=1; i<xnbins+1;i++){
    hname[i].Form("h_pt_index%d",i);
    cout << "Histo name: " << hname[i] << endl;
    hpt[i] =new TH1D(hname[i],hname[i],ynbins,ptarray);
    hpt[i]->SetLineWidth(2);
    if(i==4) hpt[i]->SetLineColor(i+2);
    else     hpt[i]->SetLineColor(i);
    for(Int_t j=1; j<ynbins+1;j++){
      hpt[i]->SetBinContent(j,histo->GetBinContent(i,j));
      hpt[i]->SetBinError(j,histo->GetBinError(i,j));
      cout << "Bin Content = " << histo->GetBinContent(i,j)<< " error " << histo->GetBinError(i,j)<<  endl;
    }//loop j
  }//loop i
  hpt[1]->SetTitle("DY vs ttbar SFs");
  if(var=="muon_pt") {
    hpt[1]->SetMaximum(1.15);
    hpt[1]->SetMinimum(0.75);
  }else{
    hpt[1]->SetMaximum(1.15);
    hpt[1]->SetMinimum(0.5);
  }
  hpt[1]->Draw();
  hpt[2]->Draw("same");
  hpt[3]->Draw("same");
  hpt[4]->Draw("same");
  ScaleFactors->Draw("same");

  TLegend* legSF = new TLegend(0.6,0.2,0.8,0.4);
  legSF->AddEntry(ScaleFactors,"ttbar SFs", "l");
  legSF->AddEntry(hpt[1],"DY SFs 0<|#eta|<0.9", "l");
  legSF->AddEntry(hpt[2],"DY SFs 0.9<|#eta|<1.2", "l");
  legSF->AddEntry(hpt[3],"DY SFs 1.2<|#eta|<2.1", "l");
  if(var=="muon_pt") legSF->AddEntry(hpt[4],"DY SFs 2.1<|#eta|<2.4", "l");
  else               legSF->AddEntry(hpt[4],"DY SFs 2.1<|#eta|<2.5", "l");
  legSF->Draw("same");
  cSF->SaveAs("plots/ScaleFactors_"+year+"_"+var+".eps");
  cSF->SaveAs("plots/ScaleFactors_"+year+"_"+var+".png");
  cSF->SaveAs("plots/ScaleFactors_"+year+"_"+var+".C");

}//the end


Double_t normal(Double_t total,Double_t passed)
{
  Double_t level = 0.682689492137; // 1 sigma
  Bool_t bUpper = true;
  Double_t alpha = (1.0 - level)/2;
  if (total == 0) return (bUpper) ? 1 : 0;
  Double_t average = passed / total;
  Double_t sigma = std::sqrt(average * (1 - average) / total);
  Double_t delta = ROOT::Math::normal_quantile(1 - alpha,sigma);
  
  if(bUpper)
    return ((average + delta) > 1) ? 1.0 : (average + delta);
  else
    return ((average - delta) < 0) ? 0.0 : (average - delta);
}

//get the histo function
TH1D *histo(TString sample, TFile *file){
  TH1D *h = (TH1D*)file->Get(sample);
  //h->Draw();
  return h;
}
//checking function
Double_t GetEfficiencyErrorLow(TH1D* htotal, TH1D* hpassed, Int_t bin)
{
  Double_t total  = htotal->GetBinContent(bin);
  Double_t passed = hpassed->GetBinContent(bin);
 
  Double_t eff = passed/total;
  cout <<"effiency " << eff << endl;
  Double_t tw  = htotal->GetBinContent(bin);
  Double_t tw2 = htotal->GetSumw2()->At(bin);
  Double_t pw  = hpassed->GetBinContent(bin);
  Double_t pw2 = hpassed->GetSumw2()->At(bin);
  cout << "tw " << tw << ", tw2 " << tw2 << ", pw " << pw << ", pw2 " << pw2 << endl;
  Double_t variance = ( pw2 * (1. - 2 * eff) + tw2 * eff *eff ) / ( tw * tw) ;
  Double_t sigma = sqrt(variance);
  Double_t level = 0.682689492137; // 1 sigma
  Double_t prob = 0.5 * (1.- level); 
  Double_t delta = ROOT::Math::normal_quantile_c(prob, sigma);
  cout << "variance " << variance << ", sigma " << sigma << ", delta " << delta << endl;
 
  // avoid to return errors which makes eff-err < 0
  return (eff - delta < 0) ? eff : delta;
}
