//////////////////// FIT-Funktionen ////////////////////
// Uniforme Verteilung
Double_t uniform_pdf(Double_t *x, Double_t xmin, Double_t xmax){ 
    return 1./(xmax - xmin) + 0 * x[0];
}

// Radioaktiver Zerfall
Double_t decay_exp(Double_t *x, Double_t *par){ 
  return par[0] * exp(- par[1] * x[0]);
}

// Radioaktiver Zerfall
Double_t gaussianic(Double_t *x, Double_t mu, Double_t sig, Double_t a){ 
  Double_t gausso = TMath::Gaus(x[0],mu,sig);
  Double_t y = a*gausso;
  return y;
}


////////// Szintillator //////////
// 1 Komponente
Double_t EMG_PDF(Double_t *x, Double_t mu, Double_t sig, Double_t tau){ 
  Double_t big = (sig*sig + 2. * tau * mu - 2. * tau * x[0])/( 2. * tau*tau);
  Double_t expX = exp(big);
  Double_t erfcX = erfc( (sig*sig + tau * mu - tau * x[0]) / (sqrt(2.) * sig * tau));
  Double_t y = 1./(2. * tau) * expX * erfcX; //erfc complementary error function (1 - erf(x) )
  return y;
}
// 2 Komponenten /// TO-DO: AUFBAUEN AUS EMG_PDF
Double_t EMG_PDF_two_comps(Double_t *x, Double_t mu, Double_t sig, Double_t tau1, Double_t tau2, Double_t p){ 
  // tau1
  Double_t big2_1 = (sig*sig + 2. * tau1 * mu - 2. * tau1 * x[0])/( 2. * tau1*tau1);
  Double_t expX2_1 = exp(big2_1);
  Double_t erfcX2_1 = erfc( (sig*sig + tau1 * mu - tau1 * x[0]) / (sqrt(2.) * sig * tau1));
  Double_t emg2_1 = 1./(2. * tau1) * expX2_1 * erfcX2_1;
  // tau2
  Double_t big2_2 = (sig*sig + 2. * tau2 * mu - 2. * tau2 * x[0])/( 2. * tau2*tau2);
  Double_t expX2_2 = exp(big2_2);
  Double_t erfcX2_2 = erfc( (sig*sig + tau2 * mu - tau2 * x[0]) / (sqrt(2.) * sig * tau2));
  Double_t emg2_2 = 1./(2. * tau2) * expX2_2 * erfcX2_2;
  // Funktion
  Double_t y = p * emg2_1 + (1-p) * emg2_2;
  return y;
}
// 3 Komponenten /// TO-DO: AUFBAUEN AUS EMG_PDF
Double_t EMG_PDF_three_comps(Double_t *x, Double_t mu, Double_t sig, Double_t tau1, Double_t tau2, Double_t tau3, Double_t p, Double_t q){ 
  // tau1
  Double_t big3_1 = (sig*sig + 2. * tau1 * mu - 2. * tau1 * x[0])/( 2. * tau1*tau1);
  Double_t expX3_1 = exp(big3_1);
  Double_t erfcX3_1 = erfc( (sig*sig + tau1 * mu - tau1 * x[0]) / (sqrt(2.) * sig * tau1));
  Double_t emg3_1 = 1./(2. * tau1) * expX3_1 * erfcX3_1;
  // tau2
  Double_t big3_2 = (sig*sig + 2. * tau2 * mu - 2. * tau2 * x[0])/( 2. * tau2*tau2);
  Double_t expX3_2 = exp(big3_2);
  Double_t erfcX3_2 = erfc( (sig*sig + tau2 * mu - tau2 * x[0]) / (sqrt(2.) * sig * tau2));
  Double_t emg3_2 = 1./(2. * tau2) * expX3_2 * erfcX3_2;
  // tau3
  Double_t big3_3 = (sig*sig + 2. * tau3 * mu - 2. * tau3 * x[0])/( 2. * tau3*tau3);
  Double_t expX3_3 = exp(big3_3);
  Double_t erfcX3_3 = erfc( (sig*sig + tau3 * mu - tau3 * x[0]) / (sqrt(2.) * sig * tau3));
  Double_t emg3_3 = 1./(2. * tau3) * expX3_3 * erfcX3_3;
  // Funktion
  Double_t y = p * emg3_1 + q * emg3_2 + (1-p-q) * emg3_3;
  return y;
}
// 4 Komponenten /// TO-DO: AUFBAUEN AUS EMG_PDF
Double_t EMG_PDF_four_comps(Double_t *x, Double_t mu, Double_t sig, Double_t tau1, Double_t tau2, Double_t tau3, Double_t tau4, Double_t p, Double_t q, Double_t r, Double_t s){ 
  // tau1
  Double_t big4_1 = (sig*sig + 2. * tau1 * mu - 2. * tau1 * x[0])/( 2. * tau1*tau1);
  Double_t expX4_1 = exp(big4_1);
  Double_t erfcX4_1 = erfc( (sig*sig + tau1 * mu - tau1 * x[0]) / (sqrt(2.) * sig * tau1));
  Double_t emg4_1 = 1./(2. * tau1) * expX4_1 * erfcX4_1;
  // tau2
  Double_t big4_2 = (sig*sig + 2. * tau2 * mu - 2. * tau2 * x[0])/( 2. * tau2*tau2);
  Double_t expX4_2 = exp(big4_2);
  Double_t erfcX4_2 = erfc( (sig*sig + tau2 * mu - tau2 * x[0]) / (sqrt(2.) * sig * tau2));
  Double_t emg4_2 = 1./(2. * tau2) * expX4_2 * erfcX4_2;
  // tau3
  Double_t big4_3 = (sig*sig + 2. * tau3 * mu - 2. * tau3 * x[0])/( 2. * tau3*tau3);
  Double_t expX4_3 = exp(big4_3);
  Double_t erfcX4_3 = erfc( (sig*sig + tau3 * mu - tau3 * x[0]) / (sqrt(2.) * sig * tau3));
  Double_t emg4_3 = 1./(2. * tau3) * expX4_3 * erfcX4_3;
  // tau4
  Double_t big4_4 = (sig*sig + 2. * tau4 * mu - 2. * tau4 * x[0])/( 2. * tau4*tau4);
  Double_t expX4_4 = exp(big4_4);
  Double_t erfcX4_4 = erfc( (sig*sig + tau4 * mu - tau4 * x[0]) / (sqrt(2.) * sig * tau4));
  Double_t emg4_4 = 1./(2. * tau4) * expX4_4 * erfcX4_4;
  // Funktion
  Double_t y = p * emg4_1 + q * emg4_2 + r * emg4_3 + s * emg4_4;
  return y;
}
///// 1 PEAK zb muons /////
// 1 Komponente
Double_t Lifetime(Double_t *x, Double_t *par){ //par = [a, mu, sig, tau]
  return par[0]*EMG_PDF(x, par[1], par[2], par[3]);
}
// 2 Komponenten
Double_t single_peak_twoT(Double_t *x, Double_t *par){ //par = [a, mu, sig, tau1, tau2, p]
    return par[0]*EMG_PDF_two_comps(x, par[1], par[2], par[3], par[4], par[5]);
}
// 3 Komponenten
Double_t single_peak_threeT(Double_t *x, Double_t *par){ //par = [a, mu, sig, tau1, tau2, tau3, p, q]
    return par[0]*EMG_PDF_three_comps(x, par[1], par[2], par[3], par[4], par[5], par[6], par[7]);
}
// 3 Komponenten ALTERNATIVE
Double_t single_peak_threeT_alter(Double_t *x, Double_t *par){ //par = [a, mu, sig, tau1, tau2, tau3, p, q]
    return par[0]*(par[6]*EMG_PDF(x, par[1], par[2], par[3]) + par[7]*EMG_PDF(x, par[1], par[2], par[4]) + (1-par[6]-par[7])*EMG_PDF(x, par[1], par[2], par[5]));
}
// 4 Komponenten
Double_t single_peak_fourT(Double_t *x, Double_t *par){ //par = [a, mu, sig, tau1, tau2, tau3, tau4, p, q, r, s]
    return par[0]*EMG_PDF_four_comps(x, par[1], par[2], par[3], par[4], par[5], par[6], par[7], par[8], par[9], par[10]);
}
///// 2 PEAK zb kaon decaying to muon /////
// 1 Komponente
Double_t double_peak_singleT(Double_t *x, Double_t *par){ //par = [a, b, mu1, mu2, sig, tau]
  return par[0]*EMG_PDF(x, par[2], par[4], par[5]) + par[1]*EMG_PDF(x, par[3], par[4], par[5]);
}
// 2 Komponenten
Double_t double_peak_twoT(Double_t *x, Double_t *par){ //par = [a, b, mu1, mu2, sig, tau1, tau2, p]
  return par[0]*EMG_PDF_two_comps(x, par[2], par[4], par[5], par[6], par[7]) + par[1]*EMG_PDF_two_comps(x, par[3], par[4], par[5], par[6], par[7]);
}

Double_t double_peak_threeT(Double_t *x, Double_t *par){ //par = [a, b, mu1, mu2, sig, tau1, tau2, tau3, p, q]
  return par[0]*EMG_PDF_three_comps(x, par[2], par[4], par[5], par[6], par[7], par[8], par[9]) + par[1]*EMG_PDF_three_comps(x, par[3], par[4], par[5], par[6], par[7], par[8], par[9]);
}

/*Double_t Double_Peak(Double_t *x, Double_t *par){ //par = [a, b, mu1, mu2, sig, tau, bkg, xmin, xmax]
  return par[0]*EMG_PDF(x, par[2], par[4], par[5]) + par[1]*EMG_PDF(x, par[3], par[4], par[5]) + par[6]*uniform_pdf(x, par[7], par[8]);
}*/
