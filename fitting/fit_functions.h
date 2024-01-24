////////// DECLARATION //////////
Double_t uniform_pdf(Double_t *x, Double_t xmin, Double_t xmax);
Double_t decay_exp(Double_t *x, Double_t *par);
Double_t gaussianic(Double_t *x, Double_t mu, Double_t sig, Double_t a);
Double_t EMG_PDF(Double_t *x, Double_t mu, Double_t sig, Double_t tau);
Double_t EMG_PDF_two_comps(Double_t *x, Double_t mu, Double_t sig, Double_t tau1, Double_t tau2, Double_t p);
Double_t EMG_PDF_three_comps(Double_t *x, Double_t mu, Double_t sig, Double_t tau1, Double_t tau2, Double_t tau3, Double_t p, Double_t q);
Double_t EMG_PDF_four_comps(Double_t *x, Double_t mu, Double_t sig, Double_t tau1, Double_t tau2, Double_t tau3, Double_t tau4, Double_t p, Double_t q, Double_t r);
Double_t Lifetime(Double_t *x, Double_t *par);
Double_t single_peak_twoT(Double_t *x, Double_t *par);
Double_t single_peak_threeT(Double_t *x, Double_t *par);
Double_t single_peak_fourT(Double_t *x, Double_t *par);
Double_t double_peak_singleT(Double_t *x, Double_t *par);
Double_t double_peak_twoT(Double_t *x, Double_t *par);
Double_t double_peak_threeT(Double_t *x, Double_t *par);


//////////////////// FIT-Functions ////////////////////
// Uniform distribution
Double_t uniform_pdf(Double_t *x, Double_t xmin, Double_t xmax){ 
    return 1./(xmax - xmin) + 0 * x[0];
}

// Radioactive decay
Double_t decay_exp(Double_t *x, Double_t *par){ 
  return par[0] * exp(- par[1] * x[0]);
}

// TEST radioactive decay
Double_t gaussianic(Double_t *x, Double_t mu, Double_t sig, Double_t a){ 
  Double_t gausso = TMath::Gaus(x[0],mu,sig);
  Double_t y = a*gausso;
  return y;
}

////////// Scintillator //////////
// 1 fluorescence decay time; norm = 1
Double_t EMG_PDF(Double_t *x, Double_t mu, Double_t sig, Double_t tau){ 
  Double_t big = (sig*sig + 2. * tau * mu - 2. * tau * x[0])/( 2. * tau*tau);
  Double_t expX = exp(big);
  Double_t erfcX = erfc( (sig*sig + tau * mu - tau * x[0]) / (sqrt(2.) * sig * tau));
  Double_t y = 1./(2. * tau) * expX * erfcX; //erfc complementary error function (1 - erf(x) )
  return y;
}
// 2 fluorescence decay times 
Double_t EMG_PDF_two_comps(Double_t *x, Double_t mu, Double_t sig, Double_t tau1, Double_t tau2, Double_t p){ 
  Double_t emg2_1 = EMG_PDF(x, mu, sig, tau1);
  Double_t emg2_2 = EMG_PDF(x, mu, sig, tau2);
  Double_t y = p * emg2_1 + (1-p) * emg2_2;
  return y;
}
// 3 fluorescence decay times 
Double_t EMG_PDF_three_comps(Double_t *x, Double_t mu, Double_t sig, Double_t tau1, Double_t tau2, Double_t tau3, Double_t p, Double_t q){ 
  Double_t emg3_1 = EMG_PDF(x, mu, sig, tau1);
  Double_t emg3_2 = EMG_PDF(x, mu, sig, tau2);
  Double_t emg3_3 = EMG_PDF(x, mu, sig, tau3);
  Double_t y = p * emg3_1 + q * emg3_2 + (1-p-q) * emg3_3;
  return y;
}
// 4 fluorescence decay times 
Double_t EMG_PDF_four_comps(Double_t *x, Double_t mu, Double_t sig, Double_t tau1, Double_t tau2, Double_t tau3, Double_t tau4, Double_t p, Double_t q, Double_t r){
  Double_t emg4_1 = EMG_PDF(x, mu, sig, tau1);
  Double_t emg4_2 = EMG_PDF(x, mu, sig, tau2);
  Double_t emg4_3 = EMG_PDF(x, mu, sig, tau3);
  Double_t emg4_4 = EMG_PDF(x, mu, sig, tau3);
  Double_t y = p * emg4_1 + q * emg4_2 + r * emg4_3 + (1-p-q-r) * emg4_4;
  return y;
}

///// 1 PEAK eg muons /////
// 1 fluorescence decay time
Double_t Lifetime(Double_t *x, Double_t *par){ //par = [a, mu, sig, tau]
  return par[0]*EMG_PDF(x, par[1], par[2], par[3]);
}
// 2 fluorescence decay times
Double_t single_peak_twoT(Double_t *x, Double_t *par){ //par = [a, mu, sig, tau1, tau2, p]
    return par[0]*( par[5]*EMG_PDF(x, par[1], par[2], par[3])
    + (1-par[5])*EMG_PDF(x, par[1], par[2], par[4]) );
}
// 3 fluorescence decay times
Double_t single_peak_threeT(Double_t *x, Double_t *par){ //par = [a, mu, sig, tau1, tau2, tau3, p, q]
    return par[0]*EMG_PDF_three_comps(x, par[1], par[2], par[3], par[4], par[5], par[6], par[7]);
}
// 4 fluorescence decay times
Double_t single_peak_fourT(Double_t *x, Double_t *par){ //par = [a, mu, sig, tau1, tau2, tau3, tau4, p, q, r]
    return par[0]*EMG_PDF_four_comps(x, par[1], par[2], par[3], par[4], par[5], par[6], par[7], par[8], par[9]);
}

///// 2 PEAKs eg kaon decaying to muon /////
// 1 fluorescence decay time
Double_t double_peak_singleT(Double_t *x, Double_t *par){ //par = [a, b, mu1, mu2, sig, tau]
  return par[0]*EMG_PDF(x, par[2], par[4], par[5]) \
  + par[1]*EMG_PDF(x, par[3], par[4], par[5]);
}
// 2 fluorescence decay times
Double_t double_peak_twoT(Double_t *x, Double_t *par){ //par = [a, b, mu1, mu2, sig, tau1, tau2, p]
  return par[0]*EMG_PDF_two_comps(x, par[2], par[4], par[5], par[6], par[7]) \
  + par[1]*EMG_PDF_two_comps(x, par[3], par[4], par[5], par[6], par[7]);
}
// 3 fluorescence decay times
Double_t double_peak_threeT(Double_t *x, Double_t *par){ //par = [a, b, mu1, mu2, sig, tau1, tau2, tau3, p, q]
  return par[0]*EMG_PDF_three_comps(x, par[2], par[4], par[5], par[6], par[7], par[8], par[9]) \
  + par[1]*EMG_PDF_three_comps(x, par[3], par[4], par[5], par[6], par[7], par[8], par[9]);
}

/*Double_t Double_Peak(Double_t *x, Double_t *par){ //par = [a, b, mu1, mu2, sig, tau, bkg, xmin, xmax]
  return par[0]*EMG_PDF(x, par[2], par[4], par[5]) + par[1]*EMG_PDF(x, par[3], par[4], par[5]) + par[6]*uniform_pdf(x, par[7], par[8]);
}*/
