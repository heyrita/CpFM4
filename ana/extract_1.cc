// ROOT
#include "TROOT.h"
#include "TChain.h"
#include "TTree.h"
#include "TFile.h"
#include "TMath.h"
#include "TCanvas.h"
#include "TMultiGraph.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TH1.h"
#include "TRandom3.h"

//C++
#include "iostream"
#include "assert.h"
#include "math.h"
#include "time.h"

using namespace std;

void function_1(TString _fileName, Int_t _nFiles, Int_t _nThreads);
int function_2(Double_t polishingQuality1, Double_t polishingQuality2);

TChain* fChain = new TChain("T");
Int_t Nhits, src_confID;
Double_t src_posZ, abs_width1, abs_width2;
Double_t Wavelength[20000];
Int_t reflectionNum1[20000];
Int_t reflectionNum2[20000];
TRandom3* randomGenerator = new TRandom3();

int main()
{
    time_t start_time, stop_time;
    start_time = time(NULL);

    const Int_t nThreads    = 1;
    const Int_t nFiles      = 271;
    const Int_t nPosZ       = 271;
    const Int_t nPolish     = 21;
    const Int_t nWidth      = 12;
    const Double_t src_posZ_init = 160.0;
    const Double_t src_posZ_step = 1.0;
    const Double_t abs_width_init = 0.00;
    const Double_t abs_width_step = 0.05;

    Double_t *****nHits_1 = new Double_t****[nWidth];
    Double_t *****nHits_err_1 = new Double_t****[nWidth];

    Double_t *****nHits_2 = new Double_t****[nWidth];
    Double_t *****nHits_err_2 = new Double_t****[nWidth];

    for(Int_t i1 = 0; i1 < nWidth; i1++)
    {
        nHits_1[i1]     = new Double_t***[nWidth];
        nHits_err_1[i1] = new Double_t***[nWidth];

        nHits_2[i1]     = new Double_t***[nWidth];
        nHits_err_2[i1] = new Double_t***[nWidth];

        for(Int_t i2 = 0; i2 < nWidth; i2++)
        {
            nHits_1[i1][i2]     = new Double_t**[nPolish];
            nHits_err_1[i1][i2] = new Double_t**[nPolish];

            nHits_2[i1][i2]     = new Double_t**[nPolish];
            nHits_err_2[i1][i2] = new Double_t**[nPolish];

            for(Int_t j1 = 0; j1 < nPolish; j1++)
            {
                nHits_1[i1][i2][j1]     = new Double_t*[nPolish];
                nHits_err_1[i1][i2][j1] = new Double_t*[nPolish];

                nHits_2[i1][i2][j1]     = new Double_t*[nPolish];
                nHits_err_2[i1][i2][j1] = new Double_t*[nPolish];

                for(Int_t j2 = 0; j2 < nPolish; j2++)
                {
                    nHits_1[i1][i2][j1][j2]     = new Double_t[nPosZ];
                    nHits_err_1[i1][i2][j1][j2] = new Double_t[nPosZ];

                    nHits_2[i1][i2][j1][j2]     = new Double_t[nPosZ];
                    nHits_err_2[i1][i2][j1][j2] = new Double_t[nPosZ];

                    for(Int_t k = 0; k < nPosZ; k++)
                    {
                        nHits_1[i1][i2][j1][j2][k]      = 0.0;
                        nHits_err_1[i1][i2][j1][j2][k]  = 0.0;

                        nHits_2[i1][i2][j1][j2][k]      = 0.0;
                        nHits_err_2[i1][i2][j1][j2][k]  = 0.0;
                    }
                }
            }
        }
    }

    Double_t* posZ          = new Double_t[nPosZ];
    Double_t* posZ_err      = new Double_t[nPosZ];
    Double_t* polish1        = new Double_t[nPolish];
    Double_t* width1         = new Double_t[nWidth];
    Double_t* polish2        = new Double_t[nPolish];
    Double_t* width2         = new Double_t[nWidth];

    function_1("../dataSim/cherenkovUA9SixthZscanTH_",nFiles,nThreads);

    fChain->SetBranchAddress("Nhits",           &Nhits);
    fChain->SetBranchAddress("src_confID",      &src_confID);
    fChain->SetBranchAddress("src_posZ",        &src_posZ);
    fChain->SetBranchAddress("abs_width1",      &abs_width1);
    fChain->SetBranchAddress("abs_width2",      &abs_width2);
    fChain->SetBranchAddress("Wavelength",      Wavelength);
    fChain->SetBranchAddress("reflectionNum1",  reflectionNum1);
    fChain->SetBranchAddress("reflectionNum2",  reflectionNum2);

    Long64_t nEntries = fChain->GetEntries();
    cout<<"--> nEntries: "<<nEntries<<endl;

    Int_t Nhits_detected;

    for(Long64_t eventID = 0; eventID < nEntries; eventID++)
    {
        fChain->GetEntry(eventID);

        Int_t widthID1  = round((abs_width1-abs_width_init)/abs_width_step);
        Int_t widthID2  = round((abs_width2-abs_width_init)/abs_width_step);
        Int_t poszID   = round((src_posZ-src_posZ_init)/src_posZ_step);

        posZ[poszID] = src_posZ;
        width1[widthID1] = abs_width1;
        width2[widthID2] = abs_width2;

        for(Int_t polishID1 = 0; polishID1 < nPolish; polishID1++)
        {
            polish1[polishID1] = 0.900 + 0.005*polishID1;

            for(Int_t polishID2 = 0; polishID2 < nPolish; polishID2++)
            {
                polish2[polishID2] = 0.900 + 0.005*polishID2;

                Nhits_detected = function_2(polish1[polishID1],polish2[polishID2]);

                if(src_confID == 1)
                {
                    if(Nhits_detected > 0)
                    {
                        nHits_1[widthID1][widthID2][polishID1][polishID2][poszID]++;
                    }
                }
                else if(src_confID == 2)
                {
                    if(Nhits_detected > 0)
                    {
                        nHits_2[widthID1][widthID2][polishID1][polishID2][poszID]++;
                    }
                }
                else
                {
                    cout<<"--> ERROR:: Not expected value of the 'src_confID': "<<src_confID<<endl;
                    assert(0);
                }
            }
        }

        if(eventID%1000000 == 0)
	{
            cout<<"--> "<<(double)100.0*eventID/nEntries<<" %"<<endl;
	    stop_time = time(NULL);
	    cout<<"--> Running time is : "<<stop_time - start_time<<" seconds"<<endl;
	}
    }

    for(Int_t i1 = 0; i1 < nWidth; i1++)
    {
        for(Int_t i2 = 0; i2 < nWidth; i2++)
        {
            for(Int_t j1 = 0; j1 < nPolish; j1++)
            {
                for(Int_t j2 = 0; j2 < nPolish; j2++)
                {
                    for(Int_t k = 0; k < nPosZ; k++)
                    {
                        posZ_err[k] = 0.1;
                        nHits_err_1[i1][i2][j1][j2][k] = TMath::Sqrt(nHits_1[i1][i2][j1][j2][k]);
                        nHits_err_2[i1][i2][j1][j2][k] = TMath::Sqrt(nHits_2[i1][i2][j1][j2][k]);
                    }
                }
            }
        }
    }

    TFile* outputFile = new TFile("OUTPUT_1_BAR.root","RECREATE");

    //TGraphErrors* gr_1[nWidth*nWidth*nPolish];
    //TGraphErrors* gr_2[nWidth*nWidth*nPolish];

    //Int_t k = 0;
    for(Int_t i1 = 0; i1 < nWidth; i1++)
    {
        for(Int_t i2 = 0; i2 < nWidth; i2++)
        {
            for(Int_t j1 = 0; j1 < nPolish; j1++)
            {
                for(Int_t j2 = 0; j2 < nPolish; j2++)
                {
		    TString name1 = "gr_1["; name1 += i1; name1 += "]["; name1 += i2;
                    name1 += "]["; name1 += j1; name1 += "]["; name1 += j2; name1 += "]";
                    TString name2 = "gr_2["; name2 += i1; name2 += "]["; name2 += i2;
                    name2 += "]["; name2 += j1; name2 += "]["; name2 += j2; name2 += "]";

                    TString title = "width1="; title += (int)round(width1[i1]*1000); title += " mkm | width2="; title += (int)round(width2[i2]*1000);
                    title += " mkm | polishing1="; title += (int)polish1[j1]; title += "."; title += (int)round(polish1[j1]*1000)%1000;
                    title += " | polishing2="; title += (int)polish2[j2]; title += "."; title += (int)round(polish2[j2]*1000)%1000;
                    TGraphErrors* gr_1 = new TGraphErrors(nPosZ,posZ,nHits_1[i1][i2][j1][j2],posZ_err,nHits_err_1[i1][i2][j1][j2]);
                    TGraphErrors* gr_2 = new TGraphErrors(nPosZ,posZ,nHits_2[i1][i2][j1][j2],posZ_err,nHits_err_2[i1][i2][j1][j2]);

                    gr_1->SetName(name1.Data());
                    gr_1->SetTitle(title.Data());
                    gr_1->Write();

                    gr_2->SetName(name2.Data());
                    gr_2->SetTitle(title.Data());
		    gr_2->Write();

		    delete gr_1;
		    delete gr_2;

                    //k++;
                }
            }
        }
    }

    outputFile->Close();

    fChain->Delete();
    delete nHits_1;
    delete nHits_err_1;
    delete nHits_2;
    delete nHits_err_2;
    delete polish1;
    delete polish2;
    delete width1;
    delete width2;
    delete posZ;
    delete posZ_err;

    stop_time = time(NULL);
    cout<<"--> Total time is : "<<stop_time - start_time<<" seconds"<<endl;

    return 0;
}

void function_1(TString _fileName, Int_t _nFiles, Int_t _nThreads)
{
    for(Int_t i = 0; i < _nThreads; i++)
    {
        for(Int_t j = 0; j < _nFiles; j++)
        {
            TString _fileName_temp = _fileName;
            _fileName_temp += i+1;
            _fileName_temp += "_";
            _fileName_temp += j;
            _fileName_temp += ".root";
            fChain->Add(_fileName_temp.Data());
            cout<<"--> "<<_fileName_temp<<endl;
        }
    }
}

int function_2(Double_t polishingQuality1, Double_t polishingQuality2)
{
    // Bialkali photocathode
    Double_t opticalPhotonWavelength[33] = {299.746185,309.898468,321.065979,330.203033,339.340088,349.492371,358.629456,369.796967,378.934021,
                                           390.101532,400.253815,407.360413,416.497467,428.680206,436.802032,450.000000,458.121826,469.289337,
                                            478.426392,489.593903,498.730957,508.883240,521.065979,529.187805,540.355347,550.507629,560.659912,
                                            569.796936,578.934021,590.101501,599.238586,609.390869,1000.0};

    Double_t opticalPhotonQuantumEff[33] = {0.352165,0.442561,0.502825,0.548023,0.653481,0.713745,0.789076,0.84934,0.92467,1.0,1.0,
						1.0,1.0,1.0,1.0,0.984934,0.92467,0.804142,0.698679,0.623353,0.548023,0.442561,0.337099,0.291901,
						0.216571,0.216571,0.186439,0.156311,0.126179,0.0809804,0.0508484,0.0357823,0};

/*	
    Double_t opticalPhotonQuantumEff[33] = {0.085388,0.107306,0.121918,0.132877,0.158447,0.173059,0.191324,0.205936,0.224201,0.242466,0.242466,
                                            0.242466,0.242466,0.242466,0.242466,0.238813,0.224201,0.194977,0.169406,0.151142,0.132877,0.107306,
                                            0.081735,0.070776,0.052511,0.052511,0.045205,0.037900,0.030594,0.019635,0.012329,0.008676,0.0};
*/

    Int_t _nPhotons = 0;

    for(Int_t j = 0; j < Nhits; j++)
    {
        Int_t i;
        for(i = 0; i < 33;i++)
        {
            if(Wavelength[j] < opticalPhotonWavelength[i]) {break;}
        }

        Double_t probability_1 = opticalPhotonQuantumEff[i-1]
                + (opticalPhotonQuantumEff[i]-opticalPhotonQuantumEff[i-1])
                *(Wavelength[j]-opticalPhotonWavelength[i-1])/(opticalPhotonWavelength[i]-opticalPhotonWavelength[i-1]);

        Double_t probability_2 = TMath::Power(polishingQuality1,reflectionNum1[j])*TMath::Power(polishingQuality2,reflectionNum2[j]);;
        //cout<<"--> j="<<j<<" | Wavelength[j]="<<Wavelength[j]<<" | reflectionNum[j]="<<reflectionNum[j]<<endl;

        //randomGenerator->SetSeed(j);
        Double_t _rndmNum = randomGenerator->Uniform(0,1);

        if(_rndmNum <= probability_1*probability_2) {_nPhotons++;}
    }

    return _nPhotons;
}
