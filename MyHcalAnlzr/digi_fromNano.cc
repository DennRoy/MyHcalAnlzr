#include "TROOT.h"
#include "TFile.h"
#include "TNtuple.h"
#include "TCanvas.h"
#include "TH2.h"
#include "TF1.h"
#include "TProfile.h"
#include "TLegend.h"
#include "TGraph.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <math.h>
#include <assert.h> 
#include <iomanip>

#include <stdlib.h>

using namespace std;

int main(int argc, char *argv[])
{
  /*if(argc!=6){
    cerr << "Invalid arguments provided, correct format is: ./main kind(Full/Ped) days lumi floatday runid\n";
    exit(0);
  }*/

  string runid = argv[1]; // Integer
  string floatday = argv[2]; // String, e.g. "22.02"
  bool RemovePED = true;

  // There is a DISGUSTING amount of hardcoding here; unfortunately I wasn't able to loop over subdets or time slices.

  TFile *f = new TFile(("/eos/user/d/dmroy/HCAL/MyHcalAnlzr_Nano/output_CalibRuns_Nano_Run"+runid+".root").c_str(), "read");
  TNtuple* qiedigi = (TNtuple*)f->Get("Events");
  int ntot = qiedigi->GetEntries();
  std::cout << "Reading in input file, total " << ntot << " Entries." << std::endl;
  //float RunNum, LumiNum, EvtNum;
  //float ieta, iphi, depth, sumADC, type, shunt;
  uint nDigiHB, nDigiHE, nDigiHF, nDigiHO;
  qiedigi->SetBranchAddress("nDigiHB", &nDigiHB); // always 9072 ?
  qiedigi->SetBranchAddress("nDigiHE", &nDigiHE); // always 6768 ?
  qiedigi->SetBranchAddress("nDigiHF", &nDigiHF); // always 3456 ?
  qiedigi->SetBranchAddress("nDigiHO", &nDigiHO); // always 2160 ?
  int DigiHB_ieta[9072], DigiHE_ieta[6768], DigiHF_ieta[3456], DigiHO_ieta[2160];
  qiedigi->SetBranchAddress("DigiHB_ieta", &DigiHB_ieta);
  qiedigi->SetBranchAddress("DigiHE_ieta", &DigiHE_ieta);
  qiedigi->SetBranchAddress("DigiHF_ieta", &DigiHF_ieta);
  qiedigi->SetBranchAddress("DigiHO_ieta", &DigiHO_ieta);
  int DigiHB_iphi[9072], DigiHE_iphi[6768], DigiHF_iphi[3456], DigiHO_iphi[2160];
  qiedigi->SetBranchAddress("DigiHB_iphi", &DigiHB_iphi);
  qiedigi->SetBranchAddress("DigiHE_iphi", &DigiHE_iphi);
  qiedigi->SetBranchAddress("DigiHF_iphi", &DigiHF_iphi);
  qiedigi->SetBranchAddress("DigiHO_iphi", &DigiHO_iphi);
  int DigiHB_depth[9072], DigiHE_depth[6768], DigiHF_depth[3456], DigiHO_depth[2160];
  qiedigi->SetBranchAddress("DigiHB_depth", &DigiHB_depth);
  qiedigi->SetBranchAddress("DigiHE_depth", &DigiHE_depth);
  qiedigi->SetBranchAddress("DigiHF_depth", &DigiHF_depth);
  qiedigi->SetBranchAddress("DigiHO_depth", &DigiHO_depth);
  float DigiHB_fc0[9072], DigiHB_fc1[9072], DigiHB_fc2[9072], DigiHB_fc3[9072];
  float DigiHB_fc4[9072], DigiHB_fc5[9072], DigiHB_fc6[9072], DigiHB_fc7[9072];
  qiedigi->SetBranchAddress("DigiHB_fc0", &DigiHB_fc0);
  qiedigi->SetBranchAddress("DigiHB_fc1", &DigiHB_fc1);
  qiedigi->SetBranchAddress("DigiHB_fc2", &DigiHB_fc2);
  qiedigi->SetBranchAddress("DigiHB_fc3", &DigiHB_fc3);
  qiedigi->SetBranchAddress("DigiHB_fc4", &DigiHB_fc4);
  qiedigi->SetBranchAddress("DigiHB_fc5", &DigiHB_fc5);
  qiedigi->SetBranchAddress("DigiHB_fc6", &DigiHB_fc6);
  qiedigi->SetBranchAddress("DigiHB_fc7", &DigiHB_fc7);
  float DigiHB_pedestalfc0[9072], DigiHB_pedestalfc1[9072], DigiHB_pedestalfc2[9072], DigiHB_pedestalfc3[9072];
  float DigiHB_pedestalfc4[9072], DigiHB_pedestalfc5[9072], DigiHB_pedestalfc6[9072], DigiHB_pedestalfc7[9072];
  qiedigi->SetBranchAddress("DigiHB_pedestalfc0", &DigiHB_pedestalfc0);
  qiedigi->SetBranchAddress("DigiHB_pedestalfc1", &DigiHB_pedestalfc1);
  qiedigi->SetBranchAddress("DigiHB_pedestalfc2", &DigiHB_pedestalfc2);
  qiedigi->SetBranchAddress("DigiHB_pedestalfc3", &DigiHB_pedestalfc3);
  qiedigi->SetBranchAddress("DigiHB_pedestalfc4", &DigiHB_pedestalfc4);
  qiedigi->SetBranchAddress("DigiHB_pedestalfc5", &DigiHB_pedestalfc5);
  qiedigi->SetBranchAddress("DigiHB_pedestalfc6", &DigiHB_pedestalfc6);
  qiedigi->SetBranchAddress("DigiHB_pedestalfc7", &DigiHB_pedestalfc7);
  int DigiHB_adc0[9072], DigiHB_adc1[9072], DigiHB_adc2[9072], DigiHB_adc3[9072];
  int DigiHB_adc4[9072], DigiHB_adc5[9072], DigiHB_adc6[9072], DigiHB_adc7[9072];
  qiedigi->SetBranchAddress("DigiHB_adc0", &DigiHB_adc0);
  qiedigi->SetBranchAddress("DigiHB_adc1", &DigiHB_adc1);
  qiedigi->SetBranchAddress("DigiHB_adc2", &DigiHB_adc2);
  qiedigi->SetBranchAddress("DigiHB_adc3", &DigiHB_adc3);
  qiedigi->SetBranchAddress("DigiHB_adc4", &DigiHB_adc4);
  qiedigi->SetBranchAddress("DigiHB_adc5", &DigiHB_adc5);
  qiedigi->SetBranchAddress("DigiHB_adc6", &DigiHB_adc6);
  qiedigi->SetBranchAddress("DigiHB_adc7", &DigiHB_adc7);
  float DigiHE_fc0[6768], DigiHE_fc1[6768], DigiHE_fc2[6768], DigiHE_fc3[6768];
  float DigiHE_fc4[6768], DigiHE_fc5[6768], DigiHE_fc6[6768], DigiHE_fc7[6768];
  qiedigi->SetBranchAddress("DigiHE_fc0", &DigiHE_fc0);
  qiedigi->SetBranchAddress("DigiHE_fc1", &DigiHE_fc1);
  qiedigi->SetBranchAddress("DigiHE_fc2", &DigiHE_fc2);
  qiedigi->SetBranchAddress("DigiHE_fc3", &DigiHE_fc3);
  qiedigi->SetBranchAddress("DigiHE_fc4", &DigiHE_fc4);
  qiedigi->SetBranchAddress("DigiHE_fc5", &DigiHE_fc5);
  qiedigi->SetBranchAddress("DigiHE_fc6", &DigiHE_fc6);
  qiedigi->SetBranchAddress("DigiHE_fc7", &DigiHE_fc7);
  float DigiHE_pedestalfc0[6768], DigiHE_pedestalfc1[6768], DigiHE_pedestalfc2[6768], DigiHE_pedestalfc3[6768];
  float DigiHE_pedestalfc4[6768], DigiHE_pedestalfc5[6768], DigiHE_pedestalfc6[6768], DigiHE_pedestalfc7[6768];
  qiedigi->SetBranchAddress("DigiHE_pedestalfc0", &DigiHE_pedestalfc0);
  qiedigi->SetBranchAddress("DigiHE_pedestalfc1", &DigiHE_pedestalfc1);
  qiedigi->SetBranchAddress("DigiHE_pedestalfc2", &DigiHE_pedestalfc2);
  qiedigi->SetBranchAddress("DigiHE_pedestalfc3", &DigiHE_pedestalfc3);
  qiedigi->SetBranchAddress("DigiHE_pedestalfc4", &DigiHE_pedestalfc4);
  qiedigi->SetBranchAddress("DigiHE_pedestalfc5", &DigiHE_pedestalfc5);
  qiedigi->SetBranchAddress("DigiHE_pedestalfc6", &DigiHE_pedestalfc6);
  qiedigi->SetBranchAddress("DigiHE_pedestalfc7", &DigiHE_pedestalfc7);
  int DigiHE_adc0[6768], DigiHE_adc1[6768], DigiHE_adc2[6768], DigiHE_adc3[6768];
  int DigiHE_adc4[6768], DigiHE_adc5[6768], DigiHE_adc6[6768], DigiHE_adc7[6768];
  qiedigi->SetBranchAddress("DigiHE_adc0", &DigiHE_adc0);
  qiedigi->SetBranchAddress("DigiHE_adc1", &DigiHE_adc1);
  qiedigi->SetBranchAddress("DigiHE_adc2", &DigiHE_adc2);
  qiedigi->SetBranchAddress("DigiHE_adc3", &DigiHE_adc3);
  qiedigi->SetBranchAddress("DigiHE_adc4", &DigiHE_adc4);
  qiedigi->SetBranchAddress("DigiHE_adc5", &DigiHE_adc5);
  qiedigi->SetBranchAddress("DigiHE_adc6", &DigiHE_adc6);
  qiedigi->SetBranchAddress("DigiHE_adc7", &DigiHE_adc7);
  float DigiHF_fc0[3456], DigiHF_fc1[3456], DigiHF_fc2[3456];
  qiedigi->SetBranchAddress("DigiHF_fc0", &DigiHF_fc0);
  qiedigi->SetBranchAddress("DigiHF_fc1", &DigiHF_fc1);
  qiedigi->SetBranchAddress("DigiHF_fc2", &DigiHF_fc2);
  float DigiHF_pedestalfc0[3456], DigiHF_pedestalfc1[3456], DigiHF_pedestalfc2[3456];
  qiedigi->SetBranchAddress("DigiHF_pedestalfc0", &DigiHF_pedestalfc0);
  qiedigi->SetBranchAddress("DigiHF_pedestalfc1", &DigiHF_pedestalfc1);
  qiedigi->SetBranchAddress("DigiHF_pedestalfc2", &DigiHF_pedestalfc2);
  int DigiHF_adc0[3456], DigiHF_adc1[3456], DigiHF_adc2[3456];
  qiedigi->SetBranchAddress("DigiHF_adc0", &DigiHF_adc0);
  qiedigi->SetBranchAddress("DigiHF_adc1", &DigiHF_adc1);
  qiedigi->SetBranchAddress("DigiHF_adc2", &DigiHF_adc2);
  float DigiHO_fc0[2160], DigiHO_fc1[2160], DigiHO_fc2[2160], DigiHO_fc3[2160], DigiHO_fc4[2160];
  float DigiHO_fc5[2160], DigiHO_fc6[2160], DigiHO_fc7[2160], DigiHO_fc8[2160], DigiHO_fc9[2160];
  qiedigi->SetBranchAddress("DigiHO_fc0", &DigiHO_fc0);
  qiedigi->SetBranchAddress("DigiHO_fc1", &DigiHO_fc1);
  qiedigi->SetBranchAddress("DigiHO_fc2", &DigiHO_fc2);
  qiedigi->SetBranchAddress("DigiHO_fc3", &DigiHO_fc3);
  qiedigi->SetBranchAddress("DigiHO_fc4", &DigiHO_fc4);
  qiedigi->SetBranchAddress("DigiHO_fc5", &DigiHO_fc5);
  qiedigi->SetBranchAddress("DigiHO_fc6", &DigiHO_fc6);
  qiedigi->SetBranchAddress("DigiHO_fc7", &DigiHO_fc7);
  qiedigi->SetBranchAddress("DigiHO_fc8", &DigiHO_fc8);
  qiedigi->SetBranchAddress("DigiHO_fc9", &DigiHO_fc9);
  float DigiHO_pedestalfc0[2160], DigiHO_pedestalfc1[2160], DigiHO_pedestalfc2[2160], DigiHO_pedestalfc3[2160], DigiHO_pedestalfc4[2160];
  float DigiHO_pedestalfc5[2160], DigiHO_pedestalfc6[2160], DigiHO_pedestalfc7[2160], DigiHO_pedestalfc8[2160], DigiHO_pedestalfc9[2160];
  qiedigi->SetBranchAddress("DigiHO_pedestalfc0", &DigiHO_pedestalfc0);
  qiedigi->SetBranchAddress("DigiHO_pedestalfc1", &DigiHO_pedestalfc1);
  qiedigi->SetBranchAddress("DigiHO_pedestalfc2", &DigiHO_pedestalfc2);
  qiedigi->SetBranchAddress("DigiHO_pedestalfc3", &DigiHO_pedestalfc3);
  qiedigi->SetBranchAddress("DigiHO_pedestalfc4", &DigiHO_pedestalfc4);
  qiedigi->SetBranchAddress("DigiHO_pedestalfc5", &DigiHO_pedestalfc5);
  qiedigi->SetBranchAddress("DigiHO_pedestalfc6", &DigiHO_pedestalfc6);
  qiedigi->SetBranchAddress("DigiHO_pedestalfc7", &DigiHO_pedestalfc7);
  qiedigi->SetBranchAddress("DigiHO_pedestalfc8", &DigiHO_pedestalfc8);
  qiedigi->SetBranchAddress("DigiHO_pedestalfc9", &DigiHO_pedestalfc9);
  int DigiHO_adc0[2160], DigiHO_adc1[2160], DigiHO_adc2[2160], DigiHO_adc3[2160], DigiHO_adc4[2160];
  int DigiHO_adc5[2160], DigiHO_adc6[2160], DigiHO_adc7[2160], DigiHO_adc8[2160], DigiHO_adc9[2160];
  qiedigi->SetBranchAddress("DigiHO_adc0", &DigiHO_adc0);
  qiedigi->SetBranchAddress("DigiHO_adc1", &DigiHO_adc1);
  qiedigi->SetBranchAddress("DigiHO_adc2", &DigiHO_adc2);
  qiedigi->SetBranchAddress("DigiHO_adc3", &DigiHO_adc3);
  qiedigi->SetBranchAddress("DigiHO_adc4", &DigiHO_adc4);
  qiedigi->SetBranchAddress("DigiHO_adc5", &DigiHO_adc5);
  qiedigi->SetBranchAddress("DigiHO_adc6", &DigiHO_adc6);
  qiedigi->SetBranchAddress("DigiHO_adc7", &DigiHO_adc7);
  qiedigi->SetBranchAddress("DigiHO_adc8", &DigiHO_adc8);
  qiedigi->SetBranchAddress("DigiHO_adc9", &DigiHO_adc9);
  UChar_t eventtype, DigiHB_sipmTypes[9072], DigiHE_sipmTypes[6768];
  qiedigi->SetBranchAddress("uMNio_EventType", &eventtype);
  qiedigi->SetBranchAddress("DigiHB_sipmTypes", &DigiHB_sipmTypes);
  qiedigi->SetBranchAddress("DigiHE_sipmTypes", &DigiHE_sipmTypes);


  TFile *ofile = new TFile(("hist_CalibOutput_run"+runid+".root").c_str(), "recreate");

  std::cout << "Creating base histograms..." << std::endl;

  //string det;
  //TH1F ***** histarray;
  //histarray = new TH1F****[3];
  vector<string> subdets{"HB", "HE", "HF", "HO"};
  map<string, map<string, map<int, map<int, map<int, TH1F*>>>>> histarrayFC, histarrayADC; // Subdet, SiPM size, ieta, iphi, depth
  /*for(int t=0; t<3; t++){ // 0=Small SiPM, 1=Large SiPM, 2=HF&HO
    histarray[t] = new TH1F***[82];
    if (t==0) size = "_sipmSmall";
    else if (t==1) size = "_sipmLarge";
    else size = "";
    for(int i=0; i<82; i++){ // Number of ieta bins -41..41, excluding 0
      histarray[t][i] = new TH1F**[72];
      for(int j=0; j<72; j++){ // Number of iphi bins 1..72
        histarray[t][i][j] = new TH1F*[7];
        for(int k=0; k<7; k++){ // Number of depths 1..7
          if (t==2 && (i<13 || i>68)) det = "HF";
          else if (t==2 && (i>25 && i<56)) det = "HO";
          else if (t<2 && ((i>=26 && i<=55) || (i==25 && k<3) || (i==56 && k<3))) det = "HB";
          else if (t<2 && ((i>=12 && i<=24) || (i>=57 && i<=69) || (i==25 && k>=3) || (i==56 && k>=3))) det = "HE";
          else det = "";
          histarray[t][i][j][k] = new TH1F(("hist_run"+runid+"_subdet"+det+size+"_ieta"+to_string(i<=41?i-41:i-40)+"_iphi"+to_string(j+1)+"_depth"+to_string(k+1)).c_str(), "Pedestal per Channel; ADC; Entries", 1600, 0, 40);
        }
      }
    }
  }*/

  std::cout << "Looping over input events..." << std::endl;

  string size;
  int N, ieta, iphi, depth, evtype, sipmtype;
  float fcsum, adcsum;
  for(int i=0; i<ntot; i++){
    if(i%100==0) std::cout << i << "-th event." << std::endl;
    qiedigi->GetEntry(i);
    evtype = eventtype; // UChar_t -> Int conversion
    if(evtype!=1) continue; // PED events only

    for (auto const& subdet : subdets){
      if(subdet=="HB") N = nDigiHB;
      else if(subdet=="HE") N = nDigiHE;
      else if(subdet=="HF") N = nDigiHF;
      else if(subdet=="HO") N = nDigiHO;
      for(int j=0; j<N; j++){
        size = "";
        if(subdet=="HB"){
          ieta = DigiHB_ieta[j];
          iphi = DigiHB_iphi[j];
          depth = DigiHB_depth[j];
          sipmtype = DigiHB_sipmTypes[j]; // UChar_t -> Int conversion
          if(sipmtype==5) size = "_sipmSmall";
          if(sipmtype==6) size = "_sipmLarge";
        }else if (subdet=="HE"){
          ieta = DigiHE_ieta[j];
          iphi = DigiHE_iphi[j];
          depth = DigiHE_depth[j];
          sipmtype = DigiHE_sipmTypes[j]; // UChar_t -> Int conversion
          if(sipmtype==3) size = "_sipmSmall";
          if(sipmtype==4) size = "_sipmLarge";
        }else if (subdet=="HF"){
          ieta = DigiHF_ieta[j];
          iphi = DigiHF_iphi[j];
          depth = DigiHF_depth[j];
        }else if (subdet=="HO"){
          ieta = DigiHO_ieta[j];
          iphi = DigiHO_iphi[j];
          depth = DigiHO_depth[j];
        }
        //if (histarrayFC.find(subdet.first) == histarrayFC.end()) histarrayFC[subdet.first];
        //if (histarrayFC[subdet.first].find(size) == histarrayFC[subdet.first].end()) histarrayFC[subdet.first][size];
        //if (histarrayFC[subdet.first][size].find(ieta) == histarrayFC[subdet.first][size].end()) histarrayFC[subdet.first][size][ieta];
        //if (histarrayFC[subdet.first][size][ieta].find(iphi) == histarrayFC[subdet.first][size][ieta].end()) histarrayFC[subdet.first][size][ieta][iphi];
        if (histarrayFC[subdet][size][ieta][iphi].find(depth) == histarrayFC[subdet][size][ieta][iphi].end()){
          histarrayFC[subdet][size][ieta][iphi][depth] = new TH1F(("hist_run"+runid+"_subdet"+subdet+size+"_ieta"+to_string(ieta)+"_iphi"+to_string(iphi)+"_depth"+to_string(depth)+"_FC").c_str(), "Pedestal per Channel; fC; Entries", 10000, 0, 1000); // TODO
          histarrayADC[subdet][size][ieta][iphi][depth] = new TH1F(("hist_run"+runid+"_subdet"+subdet+size+"_ieta"+to_string(ieta)+"_iphi"+to_string(iphi)+"_depth"+to_string(depth)+"_ADC").c_str(), "Pedestal per Channel; ADC; Entries", 960, 0, 32);
        }
        /*if(subdet=="HB"){
          histarrayFC[subdet][size][ieta][iphi][depth]->Fill(DigiHB_fc0[j]);
          histarrayFC[subdet][size][ieta][iphi][depth]->Fill(DigiHB_fc1[j]);
          histarrayFC[subdet][size][ieta][iphi][depth]->Fill(DigiHB_fc2[j]);
          histarrayFC[subdet][size][ieta][iphi][depth]->Fill(DigiHB_fc3[j]);
          histarrayFC[subdet][size][ieta][iphi][depth]->Fill(DigiHB_fc4[j]);
          histarrayFC[subdet][size][ieta][iphi][depth]->Fill(DigiHB_fc5[j]);
          histarrayFC[subdet][size][ieta][iphi][depth]->Fill(DigiHB_fc6[j]);
          histarrayFC[subdet][size][ieta][iphi][depth]->Fill(DigiHB_fc7[j]);
          histarrayADC[subdet][size][ieta][iphi][depth]->Fill(DigiHB_adc0[j]);
          histarrayADC[subdet][size][ieta][iphi][depth]->Fill(DigiHB_adc1[j]);
          histarrayADC[subdet][size][ieta][iphi][depth]->Fill(DigiHB_adc2[j]);
          histarrayADC[subdet][size][ieta][iphi][depth]->Fill(DigiHB_adc3[j]);
          histarrayADC[subdet][size][ieta][iphi][depth]->Fill(DigiHB_adc4[j]);
          histarrayADC[subdet][size][ieta][iphi][depth]->Fill(DigiHB_adc5[j]);
          histarrayADC[subdet][size][ieta][iphi][depth]->Fill(DigiHB_adc6[j]);
          histarrayADC[subdet][size][ieta][iphi][depth]->Fill(DigiHB_adc7[j]);
        }else if (subdet=="HE"){
          histarrayFC[subdet][size][ieta][iphi][depth]->Fill(DigiHE_fc0[j]);
          histarrayFC[subdet][size][ieta][iphi][depth]->Fill(DigiHE_fc1[j]);
          histarrayFC[subdet][size][ieta][iphi][depth]->Fill(DigiHE_fc2[j]);
          histarrayFC[subdet][size][ieta][iphi][depth]->Fill(DigiHE_fc3[j]);
          histarrayFC[subdet][size][ieta][iphi][depth]->Fill(DigiHE_fc4[j]);
          histarrayFC[subdet][size][ieta][iphi][depth]->Fill(DigiHE_fc5[j]);
          histarrayFC[subdet][size][ieta][iphi][depth]->Fill(DigiHE_fc6[j]);
          histarrayFC[subdet][size][ieta][iphi][depth]->Fill(DigiHE_fc7[j]);
          histarrayADC[subdet][size][ieta][iphi][depth]->Fill(DigiHE_adc0[j]);
          histarrayADC[subdet][size][ieta][iphi][depth]->Fill(DigiHE_adc1[j]);
          histarrayADC[subdet][size][ieta][iphi][depth]->Fill(DigiHE_adc2[j]);
          histarrayADC[subdet][size][ieta][iphi][depth]->Fill(DigiHE_adc3[j]);
          histarrayADC[subdet][size][ieta][iphi][depth]->Fill(DigiHE_adc4[j]);
          histarrayADC[subdet][size][ieta][iphi][depth]->Fill(DigiHE_adc5[j]);
          histarrayADC[subdet][size][ieta][iphi][depth]->Fill(DigiHE_adc6[j]);
          histarrayADC[subdet][size][ieta][iphi][depth]->Fill(DigiHE_adc7[j]);
        }else if (subdet=="HF"){
          histarrayFC[subdet][size][ieta][iphi][depth]->Fill(DigiHF_fc0[j]);
          histarrayFC[subdet][size][ieta][iphi][depth]->Fill(DigiHF_fc1[j]);
          histarrayFC[subdet][size][ieta][iphi][depth]->Fill(DigiHF_fc2[j]);
          histarrayADC[subdet][size][ieta][iphi][depth]->Fill(DigiHF_adc0[j]);
          histarrayADC[subdet][size][ieta][iphi][depth]->Fill(DigiHF_adc1[j]);
          histarrayADC[subdet][size][ieta][iphi][depth]->Fill(DigiHF_adc2[j]);
        }else if (subdet=="HO"){
          histarrayFC[subdet][size][ieta][iphi][depth]->Fill(DigiHO_fc0[j]);
          histarrayFC[subdet][size][ieta][iphi][depth]->Fill(DigiHO_fc1[j]);
          histarrayFC[subdet][size][ieta][iphi][depth]->Fill(DigiHO_fc2[j]);
          histarrayFC[subdet][size][ieta][iphi][depth]->Fill(DigiHO_fc3[j]);
          histarrayFC[subdet][size][ieta][iphi][depth]->Fill(DigiHO_fc4[j]);
          histarrayFC[subdet][size][ieta][iphi][depth]->Fill(DigiHO_fc5[j]);
          histarrayFC[subdet][size][ieta][iphi][depth]->Fill(DigiHO_fc6[j]);
          histarrayFC[subdet][size][ieta][iphi][depth]->Fill(DigiHO_fc7[j]);
          histarrayFC[subdet][size][ieta][iphi][depth]->Fill(DigiHO_fc8[j]);
          histarrayFC[subdet][size][ieta][iphi][depth]->Fill(DigiHO_fc9[j]);
          histarrayADC[subdet][size][ieta][iphi][depth]->Fill(DigiHO_adc0[j]);
          histarrayADC[subdet][size][ieta][iphi][depth]->Fill(DigiHO_adc1[j]);
          histarrayADC[subdet][size][ieta][iphi][depth]->Fill(DigiHO_adc2[j]);
          histarrayADC[subdet][size][ieta][iphi][depth]->Fill(DigiHO_adc3[j]);
          histarrayADC[subdet][size][ieta][iphi][depth]->Fill(DigiHO_adc4[j]);
          histarrayADC[subdet][size][ieta][iphi][depth]->Fill(DigiHO_adc5[j]);
          histarrayADC[subdet][size][ieta][iphi][depth]->Fill(DigiHO_adc6[j]);
          histarrayADC[subdet][size][ieta][iphi][depth]->Fill(DigiHO_adc7[j]);
          histarrayADC[subdet][size][ieta][iphi][depth]->Fill(DigiHO_adc8[j]);
          histarrayADC[subdet][size][ieta][iphi][depth]->Fill(DigiHO_adc9[j]);
        }*/
        if(subdet=="HB"){
          if(RemovePED){
            fcsum = (DigiHB_fc0[j]+DigiHB_fc1[j]+DigiHB_fc2[j]+DigiHB_fc3[j]+DigiHB_fc4[j]+DigiHB_fc5[j]+DigiHB_fc6[j]+DigiHB_fc7[j] - DigiHB_pedestalfc0[j]-DigiHB_pedestalfc1[j]-DigiHB_pedestalfc2[j]-DigiHB_pedestalfc3[j]-DigiHB_pedestalfc4[j]-DigiHB_pedestalfc5[j]-DigiHB_pedestalfc6[j]-DigiHB_pedestalfc7[j]) / 8.0;
          }else{
            fcsum = (DigiHB_fc0[j]+DigiHB_fc1[j]+DigiHB_fc2[j]+DigiHB_fc3[j]+DigiHB_fc4[j]+DigiHB_fc5[j]+DigiHB_fc6[j]+DigiHB_fc7[j]) / 8.0;
          }
          adcsum = (DigiHB_adc0[j]+DigiHB_adc1[j]+DigiHB_adc2[j]+DigiHB_adc3[j]+DigiHB_adc4[j]+DigiHB_adc5[j]+DigiHB_adc6[j]+DigiHB_adc7[j]) / 8.0;
        }else if (subdet=="HE"){
          if(RemovePED){
            fcsum = (DigiHE_fc0[j]+DigiHE_fc1[j]+DigiHE_fc2[j]+DigiHE_fc3[j]+DigiHE_fc4[j]+DigiHE_fc5[j]+DigiHE_fc6[j]+DigiHE_fc7[j] - DigiHE_pedestalfc0[j]-DigiHE_pedestalfc1[j]-DigiHE_pedestalfc2[j]-DigiHE_pedestalfc3[j]-DigiHE_pedestalfc4[j]-DigiHE_pedestalfc5[j]-DigiHE_pedestalfc6[j]-DigiHE_pedestalfc7[j]) / 8.0;
          }else{
            fcsum = (DigiHE_fc0[j]+DigiHE_fc1[j]+DigiHE_fc2[j]+DigiHE_fc3[j]+DigiHE_fc4[j]+DigiHE_fc5[j]+DigiHE_fc6[j]+DigiHE_fc7[j]) / 8.0;
          }
          adcsum = (DigiHE_adc0[j]+DigiHE_adc1[j]+DigiHE_adc2[j]+DigiHE_adc3[j]+DigiHE_adc4[j]+DigiHE_adc5[j]+DigiHE_adc6[j]+DigiHE_adc7[j]) / 8.0;
        }else if (subdet=="HF"){
          if(RemovePED){
            fcsum = (DigiHF_fc0[j]+DigiHF_fc1[j]+DigiHF_fc2[j] - DigiHF_pedestalfc0[j]-DigiHF_pedestalfc1[j]-DigiHF_pedestalfc2[j]) / 3.0;
          }else{
            fcsum = (DigiHF_fc0[j]+DigiHF_fc1[j]+DigiHF_fc2[j]) / 3.0;
          }
          adcsum = (DigiHF_adc0[j]+DigiHF_adc1[j]+DigiHF_adc2[j]) / 3.0;
        }else if (subdet=="HO"){
          if(RemovePED){
            fcsum = (DigiHO_fc0[j]+DigiHO_fc1[j]+DigiHO_fc2[j]+DigiHO_fc3[j]+DigiHO_fc4[j]+DigiHO_fc5[j]+DigiHO_fc6[j]+DigiHO_fc7[j]+DigiHO_fc8[j]+DigiHO_fc9[j] - DigiHO_pedestalfc0[j]-DigiHO_pedestalfc1[j]-DigiHO_pedestalfc2[j]-DigiHO_pedestalfc3[j]-DigiHO_pedestalfc4[j]-DigiHO_pedestalfc5[j]-DigiHO_pedestalfc6[j]-DigiHO_pedestalfc7[j]-DigiHO_pedestalfc8[j]-DigiHO_fc9[j]) / 10.0;
          }else{
            fcsum = (DigiHO_fc0[j]+DigiHO_fc1[j]+DigiHO_fc2[j]+DigiHO_fc3[j]+DigiHO_fc4[j]+DigiHO_fc5[j]+DigiHO_fc6[j]+DigiHO_fc7[j]+DigiHO_fc8[j]+DigiHO_fc9[j]) / 10.0;
          }
          adcsum = (DigiHO_adc0[j]+DigiHO_adc1[j]+DigiHO_adc2[j]+DigiHO_adc3[j]+DigiHO_adc4[j]+DigiHO_adc5[j]+DigiHO_adc6[j]+DigiHO_adc7[j]+DigiHO_adc8[j]+DigiHO_adc9[j]) / 10.0;
        }
        histarrayFC[subdet][size][ieta][iphi][depth]->Fill(fcsum);
        histarrayADC[subdet][size][ieta][iphi][depth]->Fill(adcsum);
      }
    }

    //if(shunt!=-1.0 and shunt!=6.0) continue; //-1.0 for global, 6.0 for local

    //int ietaidx = ieta<0?ieta+41:ieta+40;
    //int iphiidx = iphi-1;
    //int depthidx = depth-1;
    //int sipmidx = -1;
    //if(type==3||type==5) sipmidx=0; // Small SiPM
    //else if(type==4||type==6) sipmidx=1; // Large SiPM
    //else if(type==0||type==2) sipmidx=2; // HF/HO

    //std::cout << sipmidx << ", " << ietaidx << ", " << iphiidx << ", " << depthidx << std::endl;

    /*if (type==3 || type==4 || type==5 || type==6){
      histarray[sipmidx][ietaidx][iphiidx][depthidx]->Fill(sumADC/8.0); // Average ADC of all 8 TSs, HBHE
    }else if (!(ietaidx>=26 && ietaidx<=55)){
      histarray[sipmidx][ietaidx][iphiidx][depthidx]->Fill(sumADC/6.0); // Average ADC of all 6 TSs, HF
    }else{
      histarray[sipmidx][ietaidx][iphiidx][depthidx]->Fill(sumADC/10.0); // Average ADC of all 10 TSs, HO
    }*/

  }


  std::cout << "Writing table..." << std::endl;

  ofstream tablefile;
  tablefile.open("Table_Run"+runid+"_"+floatday+".2022.txt");
  tablefile << setw(8) << "SubDet" << setw(8) << "SiPM" << setw(8) << "ieta" << setw(8) << "iphi" << setw(8) << "depth" << setw(12) << "ADC Mean" << setw(12) << "ADC RMS" << setw(12) << "fC Mean" << setw(12) << "fC RMS" << "\n";
  for (auto const& subdet : subdets){
    for (auto const& siz : histarrayFC[subdet]){
      if(siz.first=="_sipmSmall") size = "Small";
      if(siz.first=="_sipmLarge") size = "Large";
      for (auto const& eta : histarrayFC[subdet][siz.first]){
        for (auto const& phi : histarrayFC[subdet][siz.first][eta.first]){
          for (auto const& dep : histarrayFC[subdet][siz.first][eta.first][phi.first]){
            if (histarrayFC[subdet][siz.first][eta.first][phi.first][dep.first]->GetEntries() > 0){
              tablefile << setw(8) << subdet << setw(8) << size << setw(8) << eta.first << setw(8) << phi.first << setw(8) << dep.first;
              tablefile << setw(12) << histarrayADC[subdet][siz.first][eta.first][phi.first][dep.first]->GetMean() << setw(12) << histarrayADC[subdet][siz.first][eta.first][phi.first][dep.first]->GetRMS();
              tablefile << setw(12) << histarrayFC[subdet][siz.first][eta.first][phi.first][dep.first]->GetMean() << setw(12) << histarrayFC[subdet][siz.first][eta.first][phi.first][dep.first]->GetRMS() << "\n";
            }
          }
        }
      }
    }
  }
  tablefile.close();
  /*for(int t=0; t<3; t++){ // 0=Small SiPM, 1=Large SiPM, 2=HF&HO
    for(int i=0; i<82; i++){ // Number of ieta bins -41..41, excluding 0
      for(int j=0; j<72; j++){ // Number of iphi bins 1..72
        for(int k=0; k<7; k++){ // Number of depths 1..7
          if (histarray[t][i][j][k]->GetEntries() > 0){
            det = "";
            if (t==2 && (i<13 || i>68)) det = "HF";
            else if (t==2 && (i>25 && i<56)) det = "HO";
            else if (t<2 && ((i>=26 && i<=55) || (i==25 && k<3) || (i==56 && k<3))) det = "HB";
            else if (t<2 && ((i>=12 && i<=24) || (i>=57 && i<=69) || (i==25 && k>=3) || (i==56 && k>=3))) det = "HE";
            if (t==2) size = "";
            else size = t==0?"Small":"Large";
            tablefile << setw(8) << det << setw(8) << size << setw(8) << (i<41?i-41:i-40) << setw(8) << (j+1) << setw(8) << (k+1) << setw(12) << histarray[t][i][j][k]->GetMean() << setw(12) << histarray[t][i][j][k]->GetRMS() << "\n";
          }
        }
      }
    }
  }
  tablefile.close();*/



  ////
  std::cout << "Preparing matching map..." << std::endl; // Find eta/phi/depth matching to crate/slot/fiber/channel, for HBHE only (Reminder that some eta/phi/depth overlap between HBHE and HFHO)
  map<string, string> matching;
  ifstream infile("lmap_complete.txt"); 
  string line, sumone, sumtwo, strgarbage;
  int side, eta, phi, dep, crate, slot, fiber, channel, garbage;
  vector<string> ishbhe{ "HB", "HBX", "HE", "HEX" };
  while (getline(infile, line))
  {
    istringstream iss(line);
    iss >> side;
    iss >> eta;
    iss >> phi;
    iss >> garbage; // dPhi
    iss >> dep;
    iss >> garbage; // CH_TYPE
    iss >> strgarbage; // Det
    if(find(ishbhe.begin(), ishbhe.end(), strgarbage) == ishbhe.end()) continue;
    iss >> strgarbage; // RBX
    iss >> garbage; // Wedge
    iss >> garbage; // MB_NO
    iss >> garbage; // BV
    iss >> garbage; // QIE
    iss >> garbage; // QIECH
    iss >> garbage; // RM
    iss >> garbage; // RM_FI
    iss >> channel; // FI_CH
    iss >> garbage; // Trunk
    iss >> garbage; // cpCol
    iss >> garbage; // cpRow
    iss >> garbage; // cpCpl
    iss >> garbage; // cpLC
    iss >> garbage; // cpOctFib
    iss >> garbage; // ribbon
    iss >> garbage; // ppCol
    iss >> garbage; // ppRow
    iss >> strgarbage; // ppCpl
    iss >> garbage; // ppLC
    iss >> garbage; // dodec
    iss >> crate; // Crate
    iss >> slot; // uHTR
    iss >> fiber; // uHTR_FI

    eta = eta * side;
    sumone = to_string(crate)+"_"+to_string(slot)+"_"+to_string(fiber)+"_"+to_string(channel);
    sumtwo = to_string(eta)+" "+to_string(phi)+" "+to_string(dep);
    matching[sumone] = sumtwo;
  }

  std::cout << "Writing xml..." << std::endl;

  ofstream xmlfile;
  xmlfile.open("TEST.xml");

  auto MakeNewBrick = [&xmlfile](int crate, int slot, int elements){
    xmlfile << "  <CFGBrick>" << "\n";
    xmlfile << "    <Parameter name=\"INFOTYPE\" type=\"string\">ZS</Parameter>" << "\n";
    xmlfile << "    <Parameter name=\"CREATIONTAG\" type=\"string\">hb12-13_he8_hf0_ho11_v14</Parameter>" << "\n";
    xmlfile << "    <Parameter name=\"CRATE\" type=\"int\">" << dec << crate << "</Parameter>" << "\n";
    xmlfile << "    <Parameter name=\"SLOT\" type=\"int\">" << dec << slot << "</Parameter>" << "\n";
    xmlfile << "    <Parameter name=\"TOPBOTTOM\" type=\"string\">utca</Parameter>" << "\n";
    xmlfile << "    <Parameter name=\"ZS_TYPE\" type=\"string\">standard</Parameter>" << "\n";
    xmlfile << "    <Data elements=\"" << dec << elements << "\" encoding=\"hex\">";
  };
  auto EndBrick = [&xmlfile](){
    xmlfile << "</Data>" << "\n";
    xmlfile << "  </CFGBrick>" << "\n";
  };

  xmlfile << "<?xml version=\"1.0\" ?>" << "\n";
  xmlfile << "<CFGBrickSet>" << "\n";

  // HBHE
  vector<int> HBHEcrateVec{ 20, 21, 24, 25, 30, 31, 34, 35, 37 };
  vector<int> FibNotInMod2{ 0, 1, 10, 11 };
  vector<int> FibNotInMod0{ 0, 11, 12, 23 };
  bool isHE;
  for(int crate: HBHEcrateVec){
    for(int slot=1; slot<13; slot++){
      int modulo = slot%3;
      int elements, maxchannel, ZS;
      string mystring;
      int filled = 0;
      if(modulo==0){
        maxchannel = 6;
      }else{
        maxchannel = 8;
      }
      elements = 24 * maxchannel; // 192 for HB, 144 for HE
      MakeNewBrick(crate, slot, elements);
      for(int fiber=0; fiber<24; fiber++){
        bool FiberNotInUHTR = (modulo==2 and find(FibNotInMod2.begin(), FibNotInMod2.end(), fiber) != FibNotInMod2.end()) or (modulo==0 and find(FibNotInMod0.begin(), FibNotInMod0.end(), fiber) != FibNotInMod0.end());
        for(int channel=0; channel<maxchannel; channel++){
          if((modulo==2 and fiber==10 and (channel==4 || channel==5)) || (modulo==0 and fiber==11 and (channel==0 || channel==1))){ // Nonexistent HB channels (for some reason have ZS 0 instead of 255)
            ZS = 0;
          }else if(modulo==2 and ((fiber==14 and channel==4) || (fiber==15 and channel==0) || (fiber==20 and channel==4) || (fiber==21 and channel==0))){ // Masked HE channels, RM1 RMFI6 CH4, RM2 RMFI4 CH0, RM3 RMFI6 CH4, RM4 RMFI4 CH0 (Note on ranges: RM 1-4, RMFI 1-8, CH 0-5 HE, CH 0-7 HB)
            ZS = 255;
          //}else if((modulo==2 and (fiber==2 || fiber==6) and channel==2) || (modulo==1 and (fiber==21 || fiber==23) and channel==0)){ // Masked HB channels, uHTR2 RM1 RMFI2 CH2, uHTR1 RM2 RMFI8 CH0, uHTR2 RM3 RMFI2 CH2, uHTR1 RM4 RMFI8 CH0
          //  ZS = 255;
          }else if(FiberNotInUHTR){ // Nonexistent channels
            ZS = 255;
          }else if(modulo==2 and (fiber==12 || fiber==13 || fiber==22 || fiber==23) and (channel==6 || channel==7)){ // Placeholders for HE fibers, should be 255, but were regular HE ZS 8 for some reason (keep like this for now, to test consistency)
            ZS = 8;
          }else{
            mystring = matching[to_string(crate)+"_"+to_string(slot)+"_"+to_string(fiber)+"_"+to_string(channel)];
            //if(crate==20 && slot==2) cout << to_string(crate)+"_"+to_string(slot)+"_"+to_string(fiber)+"_"+to_string(channel) << " -> " << mystring << endl;
            istringstream iss(mystring);
            int ietaidx, iphiidx, depthidx=-1000;
            while( iss >> eta >> phi >> dep ){
              ietaidx = eta<0?eta+41:eta+40;
              iphiidx = phi-1;
              depthidx = dep-1;
            }
            if(depthidx==-1000){ // No valid depth: Also Masked? HE: RM2 RMFI5 CH5, RM2 RMFI7 CH1, RM3 RMFI2 CH1, RM3 RMFI4 CH0
              ZS = 255;
            }else{
              // Example configuration: HE 8, HB small 9, HB large 10
              isHE = (((ietaidx>=12 && ietaidx<=24) || (ietaidx>=57 && ietaidx<=69) || (ietaidx==25 && depthidx>=3) || (ietaidx==56 && depthidx>=3)));
              if (((ietaidx>=26 && ietaidx<=55) || (ietaidx==25 && depthidx<3) || (ietaidx==56 && depthidx<3)) && (depthidx>1 && depthidx<4)) ZS = 13;
              else if (((ietaidx>=26 && ietaidx<=55) || (ietaidx==25 && depthidx<3) || (ietaidx==56 && depthidx<3)) && (depthidx>-1 && depthidx<2)) ZS = 12;
              else if (isHE) ZS = 8;
              // Horizontal HB:
              if ((((crate==30) and (slot==4 || slot==5 || slot==10 || slot==11)) || ((crate==31) and (slot==1 || slot==2 || slot==7 || slot==8))) and ZS>8) ZS++;
              // ZS is Mean+RMS of each channel, rounded up
              // SiPM: Large or Small? Check both
              //histarrayFC[subdet][siz.first][eta.first][phi.first][dep.first]
              /*
              if ((histarrayADC[0][ietaidx][iphiidx][depthidx]->GetEntries() > 0) and (histarrayADC[1][ietaidx][iphiidx][depthidx]->GetEntries() == 0)){
                ZS = ceil(histarrayADC[0][ietaidx][iphiidx][depthidx]->GetMean() + histarrayADC[0][ietaidx][iphiidx][depthidx]->GetRMS());
              }else if ((histarrayADC[0][ietaidx][iphiidx][depthidx]->GetEntries() == 0) and (histarrayADC[1][ietaidx][iphiidx][depthidx]->GetEntries() > 0)){
                ZS = ceil(histarrayADC[1][ietaidx][iphiidx][depthidx]->GetMean() + histarrayADC[1][ietaidx][iphiidx][depthidx]->GetRMS());
              }else if ((histarrayADC[0][ietaidx][iphiidx][depthidx]->GetEntries() == 0) and (histarrayADC[1][ietaidx][iphiidx][depthidx]->GetEntries() == 0)){
                cout << "WARNING! NONE! " << ietaidx << " " << iphiidx << " " << depthidx << endl;
              }else if ((histarrayADC[0][ietaidx][iphiidx][depthidx]->GetEntries() > 0) and (histarrayADC[1][ietaidx][iphiidx][depthidx]->GetEntries() > 0)){
                cout << "WARNING! BOTH! " << ietaidx << " " << iphiidx << " " << depthidx << endl;
              }
              */
            }
          }
          xmlfile << hex << ZS;
          filled++;
          if(elements != filled) xmlfile << " ";
        }
      }
      EndBrick();
    }
  }

  // HF
  vector<int> HFcrateVec{ 22, 29, 32 };
  for(int crate: HFcrateVec){
    for(int slot=1; slot<13; slot++){
      MakeNewBrick(crate, slot, 96);
      xmlfile << "0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0";
      EndBrick();
    }
  }
  MakeNewBrick(38, 9, 96);
  xmlfile << "0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff";
  EndBrick();

  // HO
  vector<int> HOcrateVec{ 23, 26, 27, 38 };
  for(int crate: HOcrateVec){
    for(int slot=1; slot<10; slot++){
      if(crate!=26 and slot==9) continue;
      MakeNewBrick(crate, slot, 72);
      xmlfile << "b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b";
      EndBrick();
    }
  }

  xmlfile << "</CFGBrickSet>" << "\n";
  xmlfile.close();
  ////



  std::cout << "Saving results..." << std::endl;

  ofile->cd();

  for (auto const& subdet : subdets){
    for (auto const& siz : histarrayFC[subdet]){
      for (auto const& eta : histarrayFC[subdet][siz.first]){
        for (auto const& phi : histarrayFC[subdet][siz.first][eta.first]){
          for (auto const& dep : histarrayFC[subdet][siz.first][eta.first][phi.first]){
            histarrayFC[subdet][siz.first][eta.first][phi.first][dep.first]->Write(); // dep.second->Write()
            histarrayADC[subdet][siz.first][eta.first][phi.first][dep.first]->Write();
          }
        }
      }
    }
  }
  /*for(int t=0; t<3; t++){ // 0=Small SiPM, 1=Large SiPM, 2=HF&HO
    for(int i=0; i<82; i++){ // Number of ieta bins -41..41, excluding 0
      for(int j=0; j<72; j++){ // Number of iphi bins 1..72
        for(int k=0; k<7; k++){ // Number of depths 1..7
          if (histarray[t][i][j][k]->GetEntries() > 0){
            histarray[t][i][j][k]->Write();
          }
        }
      }
    }
  }*/

  std::cout << "Finished writing." << std::endl;
  ofile->Close();

  std::cout << "End Job." << std::endl;
  return 0;

}


