 {
  TChain* chain = new TChain("T");
  chain->Add("abs_sizes/good_bar/h*.root");

  // Event* event = new Event();
  Double_t val;
  chain->SetBranchAddress("bin_w", &val);
  Int_t num = chain->GetEntries();
  for (int i = 0; i < num; i++) {
    chain->GetEvent(i);
    cout << num << endl;
  }

}