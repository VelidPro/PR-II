// Zadatak 003 - Obaveze na predmetu.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

using namespace std;

enum VrstaObaveze { Seminarski, Parcijalni1, Parcijalni2, Integralni, Prakticni };
struct Datum {
	int * _dan, *_mjesec, *_godina;
	void Unos(int d, int m, int g) {
		_dan = new int(d);
		_mjesec = new int(m);
		_godina = new int(g);
	}
	void Ispis() { cout << *_dan << "/" << *_mjesec << "/" << *_godina << endl; }
	void Dealociraj() { delete _dan; delete _mjesec; delete _godina; }
	int RazlikaDatuma(Datum d)
	{
		int suma2 = *d._dan * 30 + *d._mjesec * 12 + *d._godina;
		int suma = *_dan * 30 + *_mjesec * 12 + *_godina;

		return suma2 - suma;
	}
};
struct ObavezeNaPredmetu {
	VrstaObaveze * _vrstaObaveze;
	Datum * _datumIzvrsenja;
	char * _napomena;
	int _ocjena; // 5 - 10 

	void Unos(VrstaObaveze vrsta, Datum * datum, int ocjena, const char * napomena) {
		_vrstaObaveze = new VrstaObaveze(vrsta);
		_datumIzvrsenja = new Datum;
		_datumIzvrsenja->Unos(*datum->_dan, *datum->_mjesec, *datum->_godina);
		_ocjena = ocjena;
		int size = strlen(napomena) + 1;
		_napomena = new char[size];
		strcpy_s(_napomena, size, napomena);
	}
	void Ispis() {
		cout << *_vrstaObaveze << " " << _ocjena << " " << _napomena;
		_datumIzvrsenja->Ispis();
		cout << endl;
	}
	void Dealociraj() {
		_datumIzvrsenja->Dealociraj();
		delete _vrstaObaveze; _vrstaObaveze = nullptr;
		delete _datumIzvrsenja; _datumIzvrsenja = nullptr;
		delete[] _napomena; _napomena = nullptr;
	}
};
struct PolozeniPredmet {
	Datum _datumPolaganja;//datum koji ce se evidentirati kao datum kada je predmet polozen tj. kada je formirana konacna ocjena
	char * _nazivPredmeta;
	ObavezeNaPredmetu * _listaIzvrsenihObaveza[10];
	int _trenutnoIzvrsenihObaveza;
	int _konacnaOcjena; //formira se na osnovu ocjena izvrsenih obaveza

						/*
						1. Unos - na osnovu vrijednosti primljenog parametra izvrsiti inicijalizaciju odgovarajucih atributa
						2. DodajIzvrsenuObavezu – na osnovu vrijednosti primljenih parametara osigurati dodavanje novoizvrsene obaveze na predmetu. 
						Potrebno je onemoguciti dodavanje identicnih obaveza, a izmedju izvrsenja pojedinih obaveza mora proci najmanje 7 dana. 
						Identicna vrsta obaveze se moze dodati samo u slucaju da se radi o Seminarskom ili je prethodno dodana obaveza (identicne vrste)
						imala ocjenu 5. Ukoliko je moguce, osigurati prosirenje niza na nacin da prihvati vise od 10 obaveza.
						3. FormirajKonacnuOcjenu - konacna ocjene predstavlja prosjecnu ocjenu na predmetu, 
						a za njeno formiranje student mora posjedovati polozen integralni ili dva parijcalna ispita. 
						Ukoliko je ispit polozen putem parcijalnih ispita, student takodjer mora imati pozitivno (ocjenom vecom od 5)
						ocijenjena najmanje dva seminarska rada. U slucaju da bilo koji od navedenih uslova 
						nije zadovoljen konacna ocjena treba biti postavljena na vrijednost 5. Konacna ocjena, takodjer, ne smije biti formirana u slucaju
						da u napomeni od dvije obaveze stoji rijec 'prepisivao' ili 'izbacen'. 
						Ukoliko su ispunjeni svi uslovi, osim formiranja konacne ocjene, kao datum polaganja je potrebno postaviti 
						na vrijednost datuma posljednje izvrsene obaveze koja je usla u formiranje ocjene.
						4. Ispis – ispsuje vrijednosti svih atributa strukture
						*/

	void Unos(const char * nazivPredmeta)
	{
		_nazivPredmeta = new char[strlen(nazivPredmeta) + 1];
		strcpy_s(_nazivPredmeta, strlen(nazivPredmeta) + 1, nazivPredmeta);
		for (size_t i = 0; i < 10; i++)
		{
			_listaIzvrsenihObaveza[i] = nullptr;
		}
		_trenutnoIzvrsenihObaveza = 0;
		_konacnaOcjena;
	}

	bool DodajIzvrsenuObavezu(VrstaObaveze vrsta, Datum datum, int ocjena, const char * komentar)
	{
		if (_trenutnoIzvrsenihObaveza >= 10)
			return false;

		bool identicna = false;
		int ocjena;

		if (_listaIzvrsenihObaveza[_trenutnoIzvrsenihObaveza - 1]->_datumIzvrsenja->RazlikaDatuma(datum) < 7)
			return false;

		for (size_t i = 0; i < _trenutnoIzvrsenihObaveza; i++)
		{
			if (*_listaIzvrsenihObaveza[i]->_vrstaObaveze == vrsta)
			{
				identicna = true;
				ocjena = _listaIzvrsenihObaveza[i]->_ocjena;
			}
			if (*_listaIzvrsenihObaveza[i]->_vrstaObaveze == vrsta && (vrsta != Seminarski))
			{
				return false;
			}
		}	
		if (identicna == true)
		{
			if (ocjena > 5)
				return false;
		}

		_listaIzvrsenihObaveza[_trenutnoIzvrsenihObaveza] = new ObavezeNaPredmetu;
		_listaIzvrsenihObaveza[_trenutnoIzvrsenihObaveza]->Unos(vrsta, &datum, ocjena, komentar);
		_trenutnoIzvrsenihObaveza++;
		return true;
	}

	int FormirajKonacnuOcjenu()
	{
		



	}

};

/*
1. UpisiUBinarniFajl - objekat koji je primljen kao parametar upisuje u binarni fajl. Naziv fajla treba biti sastavljen od naziva predmeta i konacne ocjena npr. "Programiranje II_6"
2. UcitajIzBinarnogFajla - u objekat koji je primljen kao parametar ucitava sadrzaj binarnog fajla (naziv fajla je takodjer proslijedjen kao parametar)
3. PretragaRekurzivno - rekurzivna funkcija koja treba da vrati broj obaveza kod koji je student ostvario negativnu ocjenu
*/
int main() {

	Datum datumSeminarski1, datumSeminarski2, datumParcijalni1, datumParcijalni2,
		datumIntegralni;
	datumSeminarski1.Unos(10, 6, 2016);
	datumSeminarski2.Unos(18, 6, 2016);
	datumParcijalni1.Unos(22, 3, 2016);
	datumParcijalni2.Unos(22, 6, 2016);

	//polozeni predmet
	PolozeniPredmet prII, prII_FromFile;
	prII.Unos("Programiranje II");
	if (prII.DodajIzvrsenuObavezu(Parcijalni1, datumParcijalni1, 6, "uslovno polozen"))
		cout << "Parcijalni 1...dodan" << endl;
	if (prII.DodajIzvrsenuObavezu(Parcijalni2, datumParcijalni2, 5, "previse gresaka"))
		cout << "Parcijalni 2...dodan" << endl;
	if (prII.DodajIzvrsenuObavezu(Seminarski, datumSeminarski1, 8, "implementirani svi algoritmi"))
		cout << "Seminarski1...dodan" << endl;
	if (prII.DodajIzvrsenuObavezu(Seminarski, datumSeminarski2, 6, "rad slican kao kod studenta IB150388"))
		cout << "Seminarski2...dodan" << endl;

	cout << "Konacna ocjena iz predmeta PRII je: " << prII.FormirajKonacnuOcjenu() << endl;

	datumParcijalni2.Unos(22, 7, 2016);
	if (prII.DodajIzvrsenuObavezu(Parcijalni2, datumParcijalni2, 6, ""))
		cout << "Parcijalni 2...dodan" << endl;
	cout << "Konacna ocjena iz predmeta PRII je: " << prII.FormirajKonacnuOcjenu() << endl;
	//rekurzija
	cout << "Broj negativnih ocjena je: " << PretragaRekurzivno(/*parametri*/) << endl;
	//manipulacija fajlovima
	UpisiUBinarniFajl(prII);
	prII.Ispis();
	UcitajIzBinarnogFajla(prII_FromFile, "Programiranje II_6");
	prII_FromFile.Ispis();

	return 0;
	system("pause>0");
}
