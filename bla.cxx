pair <vector<vector<Double_t>>, vector<vector<Double_t>>> read_sizes ( const char *fName) {
  
  TChain *fCh = new TChain("T");
  fCh->Add(fName);

  TObjArray *fileElements = fCh->GetListOfFiles();
  Int_t nFiles = fileElements->GetEntries(); 

  vector<vector<Double_t>> vWidths;
  vWidths.resize (nFiles);
  vector<vector<Double_t>> vLenghs;
  vLenghs.resize (nFiles);

  TIter next(fileElements);
  TChainElement *chEl=0;

  Int_t k = 0;

   while ((chEl = (TChainElement*)next())) {
    TFile f(chEl->GetTitle());
    TTree* tree = (TTree*)f.Get("T");
    Double_t content, bin_w;
    tree->SetBranchAddress("content", &content);
    tree->SetBranchAddress("bin_w", &bin_w);
    for (int i = 0; i < tree->GetEntries(); i++) {
      tree->GetEntry(i);
      vWidths.at(k).push_back(content);
      vLenghs.at(k).push_back(bin_w);
    }
    delete tree;
    k++;
  }

  return make_pair (vWidths, vLenghs);
  
  }


void bla (){
  
    TString fName = "abs_sizes/good_bar/h*.root";

    vector <vector <Double_t>> vWidths_good = read_sizes(fName).first;
    vector <vector <Double_t>> vLenghs_good = read_sizes(fName).second;

    cout << vWidths_good.size() << endl;

   
}

