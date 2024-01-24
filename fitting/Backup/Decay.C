void Decay()
{
	TCanvas *c1 = new TCanvas();
	
	//Defining Decay
	TF1 *func = new TF1("func", "[0] * exp(-[1]*x)", 0, 10);
	func->SetParameter(0, 100);
	func->SetParameter(1, 0.5);
	
	TH1F *hist = new TH1F("hist", "Histogram", 100, 0, 10); // bins, start, end
	
	TRandom2 *rand = new TRandom2(3);
	
	//Als obs Daten von einem File waeren
	fstream file;
	file.open("DataDec.txt", ios::out);
	
	for(int i = 0; i < 1000; i++)
	{
		double r = func->GetRandom();
		file << r << endl; //write into the file
	}
	
	file.close();
	
	file.open("DataDec.txt", ios::in);
	
	double value;
	
	while(1)
	{
		file >> value;
		hist->Fill(value);
		if(file.eof()) break;
	}
	
	file.close();
	
	hist->GetXaxis()->SetTitle("Distribution");
	hist->GetYaxis()->SetTitle("Entries");
	
	hist->Draw();
	hist->Fit("func", "R"); //R for region or range
}
