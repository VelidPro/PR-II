// Zadatak 003 - Obaveze na predmetu.cpp : Defines the entry point for the console application.
// UKOLIKO BUDE GREŠAKA, PREDLOŽI IZMJENU. https://github.com/VelidPro/Integralno

#include "stdafx.h"
#include <fstream>
#include <iostream>

using namespace std;

const char * crt = "\n---------------------------------------------------------\n";
enum VrstaObaveze { Seminarski, Parcijalni1, Parcijalni2, Integralni, Prakticni };
int BROJ_NEGATIVNIH = 0;
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
		cout << *_vrstaObaveze << " " << _ocjena << " " << _napomena<<"  ";
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
		int Ocjena;

		if(_trenutnoIzvrsenihObaveza > 0)
			if (_listaIzvrsenihObaveza[_trenutnoIzvrsenihObaveza-1]->_datumIzvrsenja->RazlikaDatuma(datum) < 7 && vrsta != Seminarski)
				return false;

		for (size_t i = 0; i < _trenutnoIzvrsenihObaveza; i++)
		{
			if (*_listaIzvrsenihObaveza[i]->_vrstaObaveze == vrsta)
			{
				identicna = true;
				Ocjena = _listaIzvrsenihObaveza[i]->_ocjena;
			}
			if (*_listaIzvrsenihObaveza[i]->_vrstaObaveze == vrsta && (vrsta != Seminarski) && _listaIzvrsenihObaveza[i]->_ocjena != 5)
			{
				return false;
			}
		}	
		if (identicna == true)
		{
			if ((vrsta != Seminarski) && Ocjena > 5)
				return false;
		}

		_listaIzvrsenihObaveza[_trenutnoIzvrsenihObaveza] = new ObavezeNaPredmetu;
		_listaIzvrsenihObaveza[_trenutnoIzvrsenihObaveza]->Unos(vrsta, &datum, ocjena, komentar);
		_trenutnoIzvrsenihObaveza++;
		return true;
	}

	int FormirajKonacnuOcjenu()
	{
		ObavezeNaPredmetu *IzvrsenaObaveza[5];
		ObavezeNaPredmetu *seminarskiPok[10];
		int brojseminarskih = 0;
		int brojIzvrsenihObaveza = 0;
		for (size_t i = 0; i < 10; i++)
		{
			if(i<5)
				IzvrsenaObaveza[i] = nullptr;

			seminarskiPok[i] = nullptr;
		}


		for (size_t i = 0; i < _trenutnoIzvrsenihObaveza; i++)
		{
			if (i < 5)
			{
				IzvrsenaObaveza[*_listaIzvrsenihObaveza[i]->_vrstaObaveze] = _listaIzvrsenihObaveza[i];
			}

			if (*_listaIzvrsenihObaveza[i]->_vrstaObaveze == Seminarski && _listaIzvrsenihObaveza[i]->_ocjena > 5)
			{
				seminarskiPok[brojseminarskih] = _listaIzvrsenihObaveza[i];
				brojseminarskih++;
			}
		}


		if (IzvrsenaObaveza[3] != nullptr)
		{
			_konacnaOcjena = IzvrsenaObaveza[3]->_ocjena;
			_datumPolaganja.Unos(*IzvrsenaObaveza[3]->_datumIzvrsenja->_dan, *IzvrsenaObaveza[3]->_datumIzvrsenja->_mjesec, *IzvrsenaObaveza[3]->_datumIzvrsenja->_godina);
			return  _konacnaOcjena;
		}
		else if (IzvrsenaObaveza[1] != nullptr && IzvrsenaObaveza[2] != nullptr && brojseminarskih >= 2)
		{
			if (IzvrsenaObaveza[1]->_ocjena > 5 && IzvrsenaObaveza[2]->_ocjena > 5 
				&& strstr(IzvrsenaObaveza[1]->_napomena, "prepisivao") == nullptr 
				&& strstr(IzvrsenaObaveza[1]->_napomena, "izbacen") == nullptr 
				&& strstr(IzvrsenaObaveza[2]->_napomena, "preipisivao") == nullptr
				&& strstr(IzvrsenaObaveza[2]->_napomena, "izbacen") == nullptr)
			{
				float sumaOcjena = 0;
				sumaOcjena += IzvrsenaObaveza[1]->_ocjena;
				sumaOcjena += IzvrsenaObaveza[2]->_ocjena;
				for (size_t i = 0; i < brojseminarskih; i++)
				{
					sumaOcjena += seminarskiPok[i]->_ocjena;
				}

				sumaOcjena /= (brojseminarskih + 2); // broj seminarskih + 2 parcijalna ispita
				if (sumaOcjena + 0.5 >= (int)sumaOcjena + 1)
				{
					_konacnaOcjena = (int)sumaOcjena + 1;
				}
				else
				{
					_konacnaOcjena = (int)sumaOcjena;
				}

				Datum *d = nullptr;
				for (size_t i = 0; i < brojseminarskih - 1; i++)
				{
					if (seminarskiPok[i]->_datumIzvrsenja->RazlikaDatuma(*seminarskiPok[i + 1]->_datumIzvrsenja) < 0)
						d = seminarskiPok[i]->_datumIzvrsenja;
					d = seminarskiPok[i + 1]->_datumIzvrsenja;
				}
				if (d->RazlikaDatuma(*IzvrsenaObaveza[1]->_datumIzvrsenja) > 0)
					d = _listaIzvrsenihObaveza[1]->_datumIzvrsenja;

				if (d->RazlikaDatuma(*IzvrsenaObaveza[2]->_datumIzvrsenja) > 0)
					d = _listaIzvrsenihObaveza[2]->_datumIzvrsenja;

				_datumPolaganja.Unos(*d->_dan, *d->_mjesec, *d->_godina);
				return  _konacnaOcjena;
			}
		}
		return -1;
	}

	void Ispis()
	{
		cout << crt;
		cout << "Naziv predmeta: " << _nazivPredmeta << "\t Ocjena: " << _konacnaOcjena << "\t Datum polaganja: "; _datumPolaganja.Ispis();
		cout << "OBAVEZE NA PREDMETU: \n\n";
		for (size_t i = 0; i < _trenutnoIzvrsenihObaveza; i++)
		{
			_listaIzvrsenihObaveza[i]->Ispis();
		}
	}

};

/*
1. UpisiUBinarniFajl - objekat koji je primljen kao parametar upisuje u binarni fajl. Naziv fajla treba biti sastavljen od naziva predmeta i konacne ocjena npr. "Programiranje II_6"
2. UcitajIzBinarnogFajla - u objekat koji je primljen kao parametar ucitava sadrzaj binarnog fajla (naziv fajla je takodjer proslijedjen kao parametar)
3. PretragaRekurzivno - rekurzivna funkcija koja treba da vrati broj obaveza kod koji je student ostvario negativnu ocjenu
*/

void UpisiUBinarniFajl(PolozeniPredmet predmet)
{
	int vel = strlen(predmet._nazivPredmeta) + strlen(".bin") + 3 + 1; // 3 - jedan za '_' dva za ocjenu
	char * nazivFajla = new char[vel];
	strcpy_s(nazivFajla, vel, predmet._nazivPredmeta);
	strcat_s(nazivFajla, vel, "_");
	_itoa_s(predmet._konacnaOcjena, &nazivFajla[strlen(predmet._nazivPredmeta) + 1], vel,10);
	strcat_s(nazivFajla, vel, ".bin");
	nazivFajla[vel - 1] = '\0';

	ofstream upis(nazivFajla, ios::binary);
	upis.write((char*)(&predmet), sizeof(predmet));
	upis.close();
}
void UcitajIzBinarnogFajla(PolozeniPredmet &uPredmet, const char * nazivFajla)
{
	int vel = strlen(nazivFajla) + strlen(".bin") + 1;
	char * _nazivFajla = new char[vel];
	strcpy_s(_nazivFajla, vel, nazivFajla);
	strcat_s(_nazivFajla, vel, ".bin");
	_nazivFajla[vel - 1] = '\0';

	ifstream ispis;
	if (!ispis.fail())
	{
		ispis.open(_nazivFajla, ios::binary);
		ispis.read((char *)(&uPredmet), sizeof(uPredmet));
		ispis.close();
	}
}
int PretragaRekurzivno(PolozeniPredmet predmet, int brojObaveza)
{
	if (brojObaveza < 0)
		return BROJ_NEGATIVNIH;
	if (predmet._listaIzvrsenihObaveza[brojObaveza]->_ocjena == 5)
	{
		BROJ_NEGATIVNIH++;
	}
	return PretragaRekurzivno(predmet, brojObaveza - 1);
}

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
	cout << "Broj negativnih ocjena je: " << PretragaRekurzivno(prII, prII._trenutnoIzvrsenihObaveza-1) << endl;

	//manipulacija fajlovima
	UpisiUBinarniFajl(prII);
	prII.Ispis();
	UcitajIzBinarnogFajla(prII_FromFile, "Programiranje II_7");
	prII_FromFile.Ispis();

	system("pause>0");
	return 0;
}
