void Gaus()
{
	TH1F *hist = new TH1F("hist", "Gaus Fit", 100, 0, 11); // bins, start, end
	
	TRandom2 *rand = new TRandom2(3);
	
	//Als obs Daten von einem File waeren
	fstream file;
	file.open("data.txt", ios::out);
	
	for(int i = 0; i < 1000; i++)
	{
		double r = rand->Gaus(5,1);
		file << r << endl; //write into the file
	}
	
	file.close();
	
	file.open("data.txt", ios::in);
	
	double value;
	
	while(1)
	{
		file >> value;
		hist->Fill(value);
		if(file.eof()) break;
	}
	
	file.close();
	
	hist->SetFillColor(kGreen-9);
	
	hist->GetXaxis()->SetTitle("Distribution");
	hist->GetYaxis()->SetTitle("Entries");
	
	TF1 *fit = new TF1("fit", "gaus", 1, 9);
	fit->SetLineWidth(3);
	fit->SetLineStyle(2);
	
	TCanvas *c1 = new TCanvas();
	c1->SetTickx();
	c1->SetTicky();
	c1->SetGridx();
	c1->SetGridy();
	hist->SetStats(0);
	hist->Draw();
	hist->Fit("fit", "R"); //R for region or range
	
	TLegend *leg = new TLegend(0.6,0.7,0.9,0.9);
	leg->AddEntry(hist, "Measured Data", "f");
	leg->AddEntry(fit, "Fit Function", "l");
	leg->Draw();
}
