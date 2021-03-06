// Zadatak 001 - Artikli (Integralno).cpp : Defines the entry point for the console application.
// UKOLIKO BUDE GREŠAKA, PREDLOŽI IZMJENU. https://github.com/VelidPro/Integralno

#include "stdafx.h"
#include <iostream>
#include <fstream>
using namespace std;

int BROJ_RACUNA = 1;
const char * crt = "\n----------------------------------------------------\n";
struct Datum {
	int * _dan, *_mjesec, *_godina;
	void Unos(int d, int m, int g) {
		_dan = new int(d);
		_mjesec = new int(m);
		_godina = new int(g);
	}
	void Ispis() { cout << *_dan << "/" << *_mjesec << "/" << *_godina << endl; }
	void Dealociraj() {
		delete _dan; delete _mjesec; delete _godina;
		_dan = _mjesec = _godina = nullptr;
	}
};
struct Artikal {
	char * _naziv;
	float _cijena;
	void Unos(const char * naziv, float cijena) {
		int vel = strlen(naziv) + 1;
		_naziv = new char[vel];
		strcpy_s(_naziv, vel, naziv);
		_cijena = cijena;
	}
	void Dealociraj() { delete[]_naziv; _naziv = nullptr; }
	void Ispis() { cout << _naziv << " " << _cijena << " KM"; }
};
struct Racun {
	//format broja racuna: 006005491
	//osigurati automatsko generisanje broja racuna pocevsi od 000000001
	char _brojRacuna[10];
	Datum _datumKreiranja;
	Artikal * _artikli;
	int * _kolicine;//cuva informaciju o kolicini svakog artikla na racunu
	int _brojArtikala;
	//Potpisi funkcija trebaju odgovarati onima u main-u
	//1. Unos
	void Unos(char * brojRacuna, Datum &datum)
	{
		strcpy_s(_brojRacuna, brojRacuna);
		_datumKreiranja.Unos(*datum._dan, *datum._mjesec, *datum._godina);
		_artikli = nullptr;
		_kolicine = nullptr;
		_brojArtikala = 0;
	}
	//2. Dealociraj
	void Dealociraj()
	{
		_datumKreiranja.Dealociraj();
		for (size_t i = 0; i < _brojArtikala; i++)
		{
			_artikli[i].Dealociraj();
		}
		delete[] _artikli;
		_artikli = nullptr;
		delete[] _kolicine;
		_brojArtikala = 0;
	}
	//3. DodajArtikal - dodaje novi artikal u listu artikala zajedno sa njegovom kolicinom. 
	//Onemoguciti ponavljanje artikala na nacin da se uvecava samo kolicina ukoliko
	//	korisnik vise puta pokusa dodati isti artikal.
	void DodajArtikal(Artikal artikal, int kolicina)
	{
		for (size_t i = 0; i < _brojArtikala; i++)
		{
			if (strcmp(_artikli[i]._naziv, artikal._naziv) == 0)
			{
				_kolicine[i] += kolicina;
				return;
			}
		}
		
		Artikal * tempArtikal = new Artikal[_brojArtikala + 1];
		int * tempKolicine = new int[_brojArtikala + 1];

		for (size_t i = 0; i < _brojArtikala; i++)
		{
			tempArtikal[i].Unos(_artikli[i]._naziv, _artikli[i]._cijena);
			tempKolicine[i] = _kolicine[i];
		}

		delete[] _kolicine;
		delete[] _artikli; 

		_artikli = tempArtikal;
		_kolicine = tempKolicine;

		_artikli[_brojArtikala].Unos(artikal._naziv, artikal._cijena);
		_kolicine[_brojArtikala] = kolicina;
		_brojArtikala++;
	}
		//4. Ispis - ispisuje racun u formatu prikazanom na slici (nije obavezno da bude 
	//	identican, ali je svakako pozeljno).Prilikom svakog ispisa, racun
	//	je potrebno spasiti u tekstualni fajl sa istim nazivom kao i broj racuna.
	void Ispis()
	{
		ofstream unos;
		char nazivFajla[100];
		strcpy_s(nazivFajla,100, _brojRacuna);
		strcat_s(nazivFajla, ".txt");

		unos.open(nazivFajla, ios::app); // otvaramo fajl
		unos << "Broj racuna: " << _brojRacuna << endl;
		unos<< "Datum kreiranja: ";
		unos << *_datumKreiranja._dan << "." << *_datumKreiranja._mjesec << "." << *_datumKreiranja._godina<<endl;

		cout <<"Broj racuna: " << _brojRacuna << endl;
		cout << "Datum kreiranja: ";
		_datumKreiranja.Ispis();
		cout << crt;
		for (size_t i = 0; i < _brojArtikala; i++)
		{
			// Ispis na konzolu
			_artikli[i].Ispis();
			cout << "\nKolicina: " << _kolicine[i]<<crt;
			// Unos u fajl
			unos << crt;
			unos << _artikli[i]._naziv << "  "<<_artikli[i]._cijena<<endl;
			unos << "\nKolicina: "<<_kolicine[i];
			unos << crt;
		}
		unos << endl << endl;
		cout << endl << endl;
		unos.close(); // zatvaramo fajl

	}
		//5. BinarnaPretraga - koristeci binarnu pretragu, na osnovu naziva, pronalazi i vraca 
	//	pokazivac na artikal.Ukoliko trazeni artikal ne postoji funkcija vraca nullptr.

	void Sortiranje() // Insertion sort - moze se uraditi i sortiranje samo na osnovu veličine riječi (niza karaktera).
	{
		
		for (size_t i = 1; i < _brojArtikala; i++)
		{
			int j = i;
			while (j != 0)
			{
				bool promjena = false;
				if (strlen(_artikli[j]._naziv) == strlen(_artikli[j - 1]._naziv))
				{
					for (size_t i = 0; i < strlen(_artikli[j]._naziv); i++)
					{
	
						if (_artikli[j]._naziv[i] == _artikli[j - 1]._naziv[i])
							promjena = false;
						else if (_artikli[j]._naziv[i] < _artikli[j - 1]._naziv[i])
						{
							promjena = true;
							break;
						}
						else {
							promjena == false;
							break;
						}
					}
				}
				else if (strlen(_artikli[j]._naziv) < strlen(_artikli[j - 1]._naziv))
				{
					promjena = true;
				}

				if (promjena)
				{
					Artikal temp;
					temp.Unos(_artikli[j]._naziv, _artikli[j]._cijena);
					_artikli[j].Dealociraj();
					_artikli[j].Unos(_artikli[j - 1]._naziv, _artikli[j - 1]._cijena);
					_artikli[j - 1].Dealociraj();
					_artikli[j - 1].Unos(temp._naziv, temp._cijena);
				}

				j--;
			}
		}
	}

	Artikal * BinarnaPretraga(const char * kljuc) // Binarna pretraga niza karaktera. Prilikom traženja riječi, poredi slovo po slovo a ne samo velicinu niza karaktera.
	{
		this->Sortiranje();

		Artikal * pok;
		int lijevo = 0;
		int desno = _brojArtikala - 1;
		int sredina = (lijevo + desno)/2;	

		bool IdiLijevo = false;
		while (lijevo <= desno)
		{
			if (strcmp(kljuc, _artikli[sredina]._naziv) == 0)
			{
				pok = new Artikal;
				pok->Unos(_artikli[sredina]._naziv, _artikli[sredina]._cijena);
				return pok;
			}

			if (strlen(kljuc) == strlen(_artikli[sredina]._naziv))
			{
				for (size_t i = 0; i < strlen(kljuc); i++)
				{
					if (kljuc[i] == _artikli[i]._naziv[i])
					{
						IdiLijevo = false;
					}
					else if (kljuc[i] < _artikli[sredina]._naziv[i])
					{
						IdiLijevo = true;
						break;
					}
					else if (kljuc[i] > _artikli[sredina]._naziv[i])
					{
						IdiLijevo = false;
						break;
					}
				}
			}
			else if (strlen(kljuc) < strlen(_artikli[sredina]._naziv))
				IdiLijevo = true;
			else if (strlen(kljuc) > strlen(_artikli[sredina]._naziv))
				IdiLijevo = false;
			
			if (IdiLijevo == true)
			{
				desno = sredina - 1;
			}
			else
			{
				lijevo = sredina + 1;
			}

			sredina = (lijevo + desno) / 2;
		}
		return nullptr;
	}

};
//1. PronadjiNajskupljiArtikal - rekurzivna funkcija koja vraca pokazivac na artikal sa najvecom ukupnom cijenom (cijena*kolicina)
//2. GenerisiSljedeciBrojRacuna - generise i vraca naredni broj racuna
char * GenerisiSljedeciBrojRacuna()
{
	const int max = 10;
	char *povratni = new char[strlen("000000000") + 1];
	strcpy_s(povratni, strlen("000000000") + 1, "000000000");
	int brojCifara = 0;
	
	int temp = BROJ_RACUNA;
	while (temp != 0)
	{
		temp /= 10;
		brojCifara++;
	}

	_itoa_s(BROJ_RACUNA, &povratni[max - brojCifara-1],10,10);
	povratni[max-1] = '\0';
	BROJ_RACUNA++;
	return povratni;
}
Artikal * PronadjiNajskupljiArtikal(Racun racun, int brojac)
{
	if (brojac <= 0)
		return nullptr;
	Artikal * pok = nullptr;
	pok = PronadjiNajskupljiArtikal(racun, brojac - 1);
	if(pok == nullptr)
		return &racun._artikli[brojac];
	if (racun._artikli[brojac]._cijena * racun._kolicine[brojac] > PronadjiNajskupljiArtikal(racun, brojac - 1)->_cijena * racun._kolicine[brojac - 1])
	{
		pok = new Artikal;
		pok->Unos(racun._artikli[brojac]._naziv, racun._artikli[brojac]._cijena);
		return pok;
	}
}


int main() {
	//za eventualne nejasnoce analizirati testni program i ispise na slici
	Datum danas; danas.Unos(13, 6, 2016);
	Artikal cigarete, cokolada, sok,a ,b ,c;
	cigarete.Unos("Cigarete", 2.23);
	cokolada.Unos("Cokolada", 1.23);
	sok.Unos("Cappy", 2.10);
	//----------------------------- // 3 artikla više dodano radi testiranja
	a.Unos("Cigareta", 3);
	b.Unos("Cigaretu", 3);
	c.Unos("Cigpretp", 3);
	

	Racun racun;
	racun.Unos(GenerisiSljedeciBrojRacuna(), danas);
	racun.DodajArtikal(cokolada, 5);
	racun.DodajArtikal(sok, 10);
	racun.DodajArtikal(cigarete, 2);
	racun.DodajArtikal(cigarete, 5);
	//------------------------------- // 3 artikla više dodano radi testiranja
	racun.DodajArtikal(a, 5);
	racun.DodajArtikal(b, 5);
	racun.DodajArtikal(c, 5);

	racun.Ispis();
	Artikal * p1 = racun.BinarnaPretraga("Cigarete");
	racun.Ispis();
	cout << "Binarnom pretragom pronadjen artikal -> ";
	p1->Ispis();
	cout << endl;
	racun.Ispis();


	Artikal * p2 = PronadjiNajskupljiArtikal(racun, racun._brojArtikala-1);
	cout << "Rekurzivno pronadjen najskuplji artikal -> ";
	p2->Ispis();
	cout << endl;

	//dealocirati zauzetu memoriju
	racun.Dealociraj();
	p1->Dealociraj();
	delete p1; p1 = nullptr;
	p2->Dealociraj();
	delete p2; p2 = nullptr;

	system("pause>0");
	return 0;
}
