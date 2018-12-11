//#ifdef __CLING__
#ifdef __CINT__
R__LOAD_LIBRARY(libDelphes)
#endif
#include "classes/DelphesClasses.h"
#include "external/ExRootAnalysis/ExRootTreeReader.h"
#include "TTree.h"
#include "TFile.h"
#include "TClonesArray.h"
#include "TSystem.h"
#include <iostream>

//------------------------------------------------------------------------------
int getLast(TClonesArray* branch, const int iGen)
{
  const GenParticle* p = (const GenParticle*)branch->At(iGen);
  if ( p->D1 == -1 or p->D2 == -1 ) return iGen;

  for ( int i=p->D1; i<=p->D2; ++i ) {
    const GenParticle* dau = (const GenParticle*)branch->At(i);
    if ( p->PID == dau->PID ) return getLast(branch, i);
  }
  return iGen;
}

void makeFlatTuple(const std::string finName, const std::string foutName)
{
  // Prepare output tree
  TFile* fout = TFile::Open(foutName.c_str(), "recreate");
  TTree* tree = new TTree("tree", "tree");

  unsigned short b_run = 1;
  unsigned int b_event = 0;
  float b_weight = 0;

  float b_MET_pt, b_MET_phi;

  const unsigned short Muon_N = 100;
  unsigned short b_nMuon;
  float b_Muon_pt[Muon_N], b_Muon_eta[Muon_N], b_Muon_phi[Muon_N], b_Muon_m[Muon_N];
  short b_Muon_q[Muon_N];
  float b_Muon_relIso[Muon_N];

  const unsigned short Electron_N = 100;
  unsigned short b_nElectron;
  float b_Electron_pt[Electron_N], b_Electron_eta[Electron_N], b_Electron_phi[Electron_N], b_Electron_m[Electron_N];
  short b_Electron_q[Electron_N];
  float b_Electron_relIso[Electron_N];

  const unsigned short Jet_N = 100;
  unsigned short b_nJet;
  float b_Jet_pt[Jet_N], b_Jet_eta[Jet_N], b_Jet_phi[Jet_N], b_Jet_m[Jet_N];
  short b_Jet_flav[Jet_N];
  float b_Jet_bTag[Jet_N];

  const unsigned short SubJet_N = 10000;
  unsigned short b_nSubJet;
  float b_SubJet_pt[SubJet_N], b_SubJet_eta[SubJet_N], b_SubJet_phi[SubJet_N];
  short b_SubJet_q[SubJet_N], b_SubJet_pdgId[SubJet_N];
  unsigned short b_SubJet_jetIdx[SubJet_N];

  const unsigned short GenParticle_N = 1000;
  unsigned short b_nGenParticle;
  float b_GenParticle_pt[GenParticle_N], b_GenParticle_eta[GenParticle_N], b_GenParticle_phi[GenParticle_N], b_GenParticle_m[GenParticle_N];
  short b_GenParticle_pdgId[GenParticle_N], b_GenParticle_q3[GenParticle_N];
  short b_GenParticle_mother[GenParticle_N], b_GenParticle_dau1[GenParticle_N], b_GenParticle_dau2[GenParticle_N];

  const unsigned short GenJet_N = 100;
  unsigned short b_nGenJet;
  float b_GenJet_pt[GenJet_N], b_GenJet_eta[GenJet_N], b_GenJet_phi[GenJet_N], b_GenJet_m[GenJet_N];
  short b_GenJet_flav[GenJet_N];
  float b_GenJet_bTag[GenJet_N];

  tree->Branch("run", &b_run, "run/s");
  tree->Branch("event", &b_event, "event/i");
  tree->Branch("weight", &b_weight, "weight/F");

  tree->Branch("MET_pt", &b_MET_pt, "MET_pt/F");
  tree->Branch("MET_phi", &b_MET_phi, "MET_phi/F");

  tree->Branch("nMuon", &b_nMuon, "nMuon/s");
  tree->Branch("Muon_pt", b_Muon_pt, "Muon_pt[nMuon]/F");
  tree->Branch("Muon_eta", b_Muon_eta, "Muon_eta[nMuon]/F");
  tree->Branch("Muon_phi", b_Muon_phi, "Muon_phi[nMuon]/F");
  tree->Branch("Muon_m", b_Muon_m, "Muon_m[nMuon]/F");
  tree->Branch("Muon_q", b_Muon_q, "Muon_q[nMuon]/S");
  tree->Branch("Muon_relIso", b_Muon_relIso, "Muon_relIso[nMuon]/F");

  tree->Branch("nElectron", &b_nElectron, "nElectron/s");
  tree->Branch("Electron_pt", b_Electron_pt, "Electron_pt[nElectron]/F");
  tree->Branch("Electron_eta", b_Electron_eta, "Electron_eta[nElectron]/F");
  tree->Branch("Electron_phi", b_Electron_phi, "Electron_phi[nElectron]/F");
  tree->Branch("Electron_m", b_Electron_m, "Electron_m[nElectron]/F");
  tree->Branch("Electron_q", b_Electron_q, "Electron_q[nElectron]/S");
  tree->Branch("Electron_relIso", b_Electron_relIso, "Electron_relIso[nElectron]/F");

  tree->Branch("nJet", &b_nJet, "nJet/s");
  tree->Branch("Jet_pt", b_Jet_pt, "Jet_pt[nJet]/F");
  tree->Branch("Jet_eta", b_Jet_eta, "Jet_eta[nJet]/F");
  tree->Branch("Jet_phi", b_Jet_phi, "Jet_phi[nJet]/F");
  tree->Branch("Jet_m", b_Jet_m, "Jet_m[nJet]/F");
  tree->Branch("Jet_flav", b_Jet_flav, "Jet_flav[nJet]/S");
  tree->Branch("Jet_bTag", b_Jet_bTag, "Jet_bTag[nJet]/F");

  tree->Branch("nSubJet", &b_nSubJet, "nSubJet/s");
  tree->Branch("SubJet_pt", b_SubJet_pt, "SubJet_pt[nSubJet]/F");
  tree->Branch("SubJet_eta", b_SubJet_eta, "SubJet_eta[nSubJet]/F");
  tree->Branch("SubJet_phi", b_SubJet_phi, "SubJet_phi[nSubJet]/F");
  tree->Branch("SubJet_q", b_SubJet_q, "SubJet_q[nSubJet]/S");
  tree->Branch("SubJet_pdgId", b_SubJet_pdgId, "SubJet_pdgId[nSubJet]/S");
  tree->Branch("SubJet_jetIdx", b_SubJet_jetIdx, "SubJet_jetIdx[nSubJet]/S");

  tree->Branch("nGenParticle", &b_nGenParticle, "nGenParticle/s");
  tree->Branch("GenParticle_pt", b_GenParticle_pt, "GenParticle_pt[nGenParticle]/F");
  tree->Branch("GenParticle_eta", b_GenParticle_eta, "GenParticle_eta[nGenParticle]/F");
  tree->Branch("GenParticle_phi", b_GenParticle_phi, "GenParticle_phi[nGenParticle]/F");
  tree->Branch("GenParticle_m", b_GenParticle_m, "GenParticle_m[nGenParticle]/F");
  tree->Branch("GenParticle_pdgId", b_GenParticle_pdgId, "GenParticle_pdgId[nGenParticle]/S");
  tree->Branch("GenParticle_q3", b_GenParticle_q3, "GenParticle_q3[nGenParticle]/S");
  tree->Branch("GenParticle_mother", b_GenParticle_mother, "GenParticle_mother[nGenParticle]/S");
  tree->Branch("GenParticle_dau1", b_GenParticle_dau1, "GenParticle_dau1[nGenParticle]/S");
  tree->Branch("GenParticle_dau2", b_GenParticle_dau2, "GenParticle_dau2[nGenParticle]/S");

  tree->Branch("nGenJet", &b_nGenJet, "nGenJet/s");
  tree->Branch("GenJet_pt", b_GenJet_pt, "GenJet_pt[nGenJet]/F");
  tree->Branch("GenJet_eta", b_GenJet_eta, "GenJet_eta[nGenJet]/F");
  tree->Branch("GenJet_phi", b_GenJet_phi, "GenJet_phi[nGenJet]/F");
  tree->Branch("GenJet_m", b_GenJet_m, "GenJet_m[nGenJet]/F");
  tree->Branch("GenJet_flav", b_GenJet_flav, "GenJet_flav[nGenJet]/S");
  tree->Branch("GenJet_bTag", b_GenJet_bTag, "GenJet_bTag[nGenJet]/F");

  // Create chain of root trees
  TChain chain("Delphes");
  chain.Add(finName.c_str());

  // Create object of class ExRootTreeReader
  ExRootTreeReader *treeReader = new ExRootTreeReader(&chain);
  Long64_t numberOfEntries = treeReader->GetEntries();

  // Get pointers to branches used in this analysis
  TClonesArray *branchGen = treeReader->UseBranch("Particle");
  TClonesArray *branchEvent = treeReader->UseBranch("Event");
  TClonesArray *branchMET = treeReader->UseBranch("MissingET");
  TClonesArray *branchMuon = treeReader->UseBranch("Muon");
  TClonesArray *branchElectron = treeReader->UseBranch("Electron");
  TClonesArray *branchJet = treeReader->UseBranch("Jet");
  TClonesArray *branchGenJet = treeReader->UseBranch("GenJet");
  TClonesArray *branchEFlowTrack = treeReader->UseBranch("EFlowTrack");
  TClonesArray *branchEFlowPhoton = treeReader->UseBranch("EFlowPhoton");
  TClonesArray *branchEFlowNeutralHadron = treeReader->UseBranch("EFlowNeutralHadron");

  // Loop over all events
  for(Int_t entry = 0; entry < numberOfEntries; ++entry) {
    // Load selected branches with data from specified event
    treeReader->ReadEntry(entry);
    std::cout << entry << '/' << numberOfEntries << '\r';

    const HepMCEvent* event = (const HepMCEvent*)branchEvent->At(0);
    b_event = event->Number;
    b_weight = event->Weight;

    // Build gen particle collection, for the ttbar decays
    std::vector<std::vector<int> > GenParticle_topDaus;
    b_nGenParticle = 0;
    for ( int i=0; i<branchGen->GetEntries(); ++i ) {
      const GenParticle* p = (const GenParticle*)branchGen->At(i);
      const int pid = p->PID, absId = abs(p->PID);
      if ( absId != 6 ) continue; // top only.
      if ( p->D1 == -1 or p->D2 == -1 ) continue; // should have valid daughters

      bool isDupl = false;
      for ( int j=p->D1; j<=p->D2; ++j ) {
        const GenParticle* dau  = (const GenParticle*)branchGen->At(j);
        if ( pid == dau->PID ) { isDupl = true; break; }
      }
      if ( isDupl ) continue;

      GenParticle_topDaus.emplace_back();
      for ( int j=p->D1; j<=p->D2; ++j ) GenParticle_topDaus.back().push_back(j);

      // Fill top quarks
      b_GenParticle_pt[b_nGenParticle] = p->PT;
      b_GenParticle_eta[b_nGenParticle] = p->Eta;
      b_GenParticle_phi[b_nGenParticle] = p->Phi;
      b_GenParticle_m[b_nGenParticle] = p->Mass;
      b_GenParticle_pdgId[b_nGenParticle] = p->PID;
      b_GenParticle_q3[b_nGenParticle] = p->Charge*3;
      b_GenParticle_dau1[b_nGenParticle] = b_GenParticle_dau2[b_nGenParticle] = -1;
      b_GenParticle_mother[b_nGenParticle] = -1;

      ++b_nGenParticle;
      if ( b_nGenParticle >= GenParticle_N ) break;
    }
    
    std::vector<int> dauIdx;
    for ( int i=0, n=GenParticle_topDaus.size(); i<n; ++i ) {
      const auto& dauIdxs = GenParticle_topDaus.at(i);
      if ( dauIdxs.empty() ) continue; // no daughters

      b_GenParticle_dau1[i] = b_nGenParticle;
      b_GenParticle_dau2[i] = b_nGenParticle+dauIdxs.size()-1;

      for ( auto j : dauIdxs ) {
        const GenParticle* dau = (const GenParticle*)branchGen->At(j);

        // Fill top quark daughters
        b_GenParticle_pt[b_nGenParticle] = dau->PT;
        b_GenParticle_eta[b_nGenParticle] = dau->Eta;
        b_GenParticle_phi[b_nGenParticle] = dau->Phi;
        b_GenParticle_m[b_nGenParticle] = dau->Mass;
        b_GenParticle_pdgId[b_nGenParticle] = dau->PID;
        b_GenParticle_q3[b_nGenParticle] = dau->Charge*3;
        b_GenParticle_dau1[b_nGenParticle] = b_GenParticle_dau2[b_nGenParticle] = -1;
        b_GenParticle_mother[b_nGenParticle] = i;

        const int iDau = b_nGenParticle;
        ++b_nGenParticle;
        if ( b_nGenParticle >= GenParticle_N ) break;

        if ( abs(dau->PID) < 23 or abs(dau->PID) > 25 ) continue;
        if ( dau->D1 == -1 or dau->D2 == -1 ) continue;
        dau = (const GenParticle*)branchGen->At(getLast(branchGen, j));

        int ngdau = 0;
        for ( int k=dau->D1; k<=dau->D2; ++k ) {
          const GenParticle* gdau = (const GenParticle*)branchGen->At(k);

          // Fill W/Z/H daughters
          b_GenParticle_pt[b_nGenParticle] = gdau->PT;
          b_GenParticle_eta[b_nGenParticle] = gdau->Eta;
          b_GenParticle_phi[b_nGenParticle] = gdau->Phi;
          b_GenParticle_m[b_nGenParticle] = gdau->Mass;
          b_GenParticle_pdgId[b_nGenParticle] = gdau->PID;
          b_GenParticle_q3[b_nGenParticle] = gdau->Charge*3;
          b_GenParticle_dau1[b_nGenParticle] = b_GenParticle_dau2[b_nGenParticle] = -1;
          b_GenParticle_mother[b_nGenParticle] = iDau;

          ++ngdau;
          ++b_nGenParticle;
          if ( b_nGenParticle >= GenParticle_N ) break;

          const int iGdau = b_nGenParticle;
          // For the tau decays
          if ( abs(gdau->PID) != 15 ) continue;
          if ( gdau->D1 == -1 or gdau->D2 == -1 ) continue;
          gdau = (const GenParticle*)branchGen->At(getLast(branchGen, k));

          int nggdau = 0;
          for ( int l=gdau->D1; l<=gdau->D2; ++l ) {
            const GenParticle* ggdau = (const GenParticle*)branchGen->At(l);
            const int absId = abs(ggdau->PID);
            if ( absId < 11 or absId >= 15 ) continue;

            // Fill W/Z/H daughters
            b_GenParticle_pt[b_nGenParticle] = ggdau->PT;
            b_GenParticle_eta[b_nGenParticle] = ggdau->Eta;
            b_GenParticle_phi[b_nGenParticle] = ggdau->Phi;
            b_GenParticle_m[b_nGenParticle] = ggdau->Mass;
            b_GenParticle_pdgId[b_nGenParticle] = ggdau->PID;
            b_GenParticle_q3[b_nGenParticle] = ggdau->Charge*3;
            b_GenParticle_dau1[b_nGenParticle] = b_GenParticle_dau2[b_nGenParticle] = -1;
            b_GenParticle_mother[b_nGenParticle] = iGdau;

            ++nggdau;
            ++b_nGenParticle;
            if ( b_nGenParticle >= GenParticle_N ) break;
          }
          b_GenParticle_dau1[iGdau] = iGdau+1;
          b_GenParticle_dau2[iGdau] = iGdau+nggdau;
          if ( b_nGenParticle >= GenParticle_N ) break;
        }
        b_GenParticle_dau1[iDau] = iDau+1;
        b_GenParticle_dau2[iDau] = iDau+ngdau;
        
        if ( b_nGenParticle >= GenParticle_N ) break;
      }
      if ( b_nGenParticle >= GenParticle_N ) break;
    }

    if ( branchMET->GetEntries() > 0 ) {
      const MissingET* met = (const MissingET*)branchMET->At(0);
      b_MET_pt = met->MET;
      b_MET_phi = met->Phi;
    }
    else {
      b_MET_pt = b_MET_phi = 0;
    }

    b_nMuon = 0;
    for ( int i=0; i<branchMuon->GetEntries(); ++i ) {
      const Muon* muon = (const Muon*) branchMuon->At(i);
      const TLorentzVector p4 = muon->P4();

      b_Muon_pt[b_nMuon] = muon->PT;
      b_Muon_eta[b_nMuon] = muon->Eta;
      b_Muon_phi[b_nMuon] = muon->Phi;
      b_Muon_m[b_nMuon] = p4.M();
      b_Muon_q[b_nMuon] = muon->Charge;

      b_Muon_relIso[b_nMuon] = muon->IsolationVar;///muon->PT;

      ++b_nMuon;
      if ( b_nMuon >= Muon_N ) break;
    }

    b_nElectron = 0;
    for ( int i=0; i<branchElectron->GetEntries(); ++i ) {
      const Electron* electron = (const Electron*) branchElectron->At(i);
      const TLorentzVector p4 = electron->P4();

      b_Electron_pt[b_nElectron] = electron->PT;
      b_Electron_eta[b_nElectron] = electron->Eta;
      b_Electron_phi[b_nElectron] = electron->Phi;
      b_Electron_m[b_nElectron] = p4.M();
      b_Electron_q[b_nElectron] = electron->Charge;

      b_Electron_relIso[b_nElectron] = electron->IsolationVarRhoCorr;///electron->PT;

      ++b_nElectron;
      if ( b_nElectron >= Electron_N ) break;
    }

    b_nJet = b_nSubJet = 0;
    for ( int i=0; i<branchJet->GetEntries(); ++i ) {
      const Jet* jet = (const Jet*) branchJet->At(i);
      //const TLorentzVector p4 = jet->P4();

      b_Jet_pt[b_nJet] = jet->PT;
      b_Jet_eta[b_nJet] = jet->Eta;
      b_Jet_phi[b_nJet] = jet->Phi;
      b_Jet_m[b_nJet] = jet->Mass;
      b_Jet_flav[b_nJet] = jet->Flavor;
      b_Jet_bTag[b_nJet] = jet->BTag;

      // Keep the subjet particles
      TRefArray cons = jet->Constituents;
      for ( int j=0; j<cons.GetEntriesFast(); ++j ) {
        if ( b_nSubJet > SubJet_N ) break;

        const TObject* obj = cons.At(j);
        if ( !obj ) continue;

        //const GenParticle* p = dynamic_cast<const GenParticle*>(obj);
        const Track* track = dynamic_cast<const Track*>(obj);
        const Tower* tower = dynamic_cast<const Tower*>(obj);
        if ( track ) {
          b_SubJet_pt[b_nSubJet] = track->PT;
          b_SubJet_eta[b_nSubJet] = track->Eta;
          b_SubJet_phi[b_nSubJet] = track->Phi;
          b_SubJet_q[b_nSubJet] = track->Charge;
          //b_SubJet_pdgId[b_nSubJet] = track->Charge*211;
          const GenParticle* p = dynamic_cast<const GenParticle*>(track->Particle.GetObject());
          b_SubJet_pdgId[b_nSubJet] = p->PID;
        }
        else if ( tower ) {
          b_SubJet_pt[b_nSubJet] = tower->ET;
          b_SubJet_eta[b_nSubJet] = tower->Eta;
          b_SubJet_phi[b_nSubJet] = tower->Phi;
          b_SubJet_q[b_nSubJet] = 0;
          //const bool isPhoton = ( tower->Eem > tower->Ehad ); // Crude estimation
          //if ( isPhoton ) b_SubJet_pdgId[b_nSubJet] = 22; // photons
          //else b_SubJet_pdgId[b_nSubJet] = 2112; // set as neutron
          int nPhoton = 0;
          TRefArray ps = tower->Particles;
          for ( int k=0; k<ps.GetEntries(); ++k ) {
            const GenParticle* p = dynamic_cast<const GenParticle*>(ps.At(k));
            if ( p->PID == 22 ) ++nPhoton;
          }
          b_SubJet_pdgId[b_nSubJet] = nPhoton > 0 ? 22 : 2112; // set as neutron if no photon found in this tower
        }
        else {
          std::cout << obj->IsA()->GetName() << endl;
          continue;
        }
        b_SubJet_jetIdx[b_nSubJet] = b_nJet;
        ++b_nSubJet;
      }

      ++b_nJet;
      if ( b_nJet >= Jet_N ) break;
    }

    b_nGenJet = 0; //b_nSubGenJet = 0;
    for ( int i=0; i<branchGenJet->GetEntries(); ++i ) {
      const Jet* jet = (const Jet*) branchGenJet->At(i);
      //const TLorentzVector p4 = jet->P4();

      b_GenJet_pt[b_nGenJet] = jet->PT;
      b_GenJet_eta[b_nGenJet] = jet->Eta;
      b_GenJet_phi[b_nGenJet] = jet->Phi;
      b_GenJet_m[b_nGenJet] = jet->Mass;
      b_GenJet_flav[b_nGenJet] = jet->Flavor;
      b_GenJet_bTag[b_nGenJet] = jet->BTag;

      // Keep the subjet particles
      TRefArray cons = jet->Constituents;
      for ( int j=0; j<cons.GetEntriesFast(); ++j ) {
        //if ( b_nSubGenJet > SubGenJet_N ) break;

        const TObject* obj = cons.At(j);
        if ( !obj ) continue;

        const GenParticle* p = dynamic_cast<const GenParticle*>(obj);
        if ( p ) {
          //cout << p << ' ' << p->PID << endl;
          /*b_SubGenJet_pt[b_nSubGenJet] = track->PT;
          b_SubGenJet_eta[b_nSubGenJet] = track->Eta;
          b_SubGenJet_phi[b_nSubGenJet] = track->Phi;
          b_SubGenJet_q[b_nSubGenJet] = track->Charge;
          b_SubGenJet_pdgId[b_nSubGenJet] = track->Charge*211;*/
        }
        else {
          std::cout << obj->IsA()->GetName() << endl;
          continue;
        }
        //b_SubGenJet_jetIdx[b_nSubGenJet] = b_nGenJet;
        //++b_nSubGenJet;
      }

      ++b_nGenJet;
      if ( b_nGenJet >= GenJet_N ) break;
    }

    tree->Fill();
  }

  tree->Write();
  fout->Close();

}

