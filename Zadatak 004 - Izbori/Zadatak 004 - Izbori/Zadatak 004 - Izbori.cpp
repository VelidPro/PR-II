// Zadatak 004 - Izbori.cpp : Defines the entry point for the console application.
// UKOLIKO BUDE GREŠAKA, PREDLOŽI IZMJENU. https://github.com/VelidPro/Integralno

#include "stdafx.h"
#include <fstream>
#include <iostream>

using namespace std;

//narednu liniju code-a ignorisite, osim u slucaju da vam bude predstavljala smetnje u radu
#pragma warning(disable:4996)

const char *crt = "\n---------------------------------------------------------------\n";


enum Opstina { Opstina1, Opstina2, Opstina3, Opstina4 };
const char * OpstineChar[] = { "Opstina1", "Opstina2", "Opstina3", "Opstina4" };
enum Kandidati { Kandidat1, Kandidat2, Kandidat3, Kandidat4 };
const char * KandidatiChar[4] = { "Kandidat1", "Kandidat2", "Kandidat3", "Kandidat4" };


void Sortiraj(int brojGlasova[])
{
	for (size_t i = 1; i < 4; i++)
	{
		int j = i;

		while (j != 0)
		{
			if (brojGlasova[j] > brojGlasova[j - 1])
			{
				int tempBroj = brojGlasova[j];
				brojGlasova[j] = brojGlasova[j - 1];
				brojGlasova[j - 1] = tempBroj;

				const char * tempChar = KandidatiChar[j];
				KandidatiChar[j] = KandidatiChar[j - 1];
				KandidatiChar[j - 1] = tempChar;
			}
			j--;
		}

	}
}


struct Datum {
	int _dan, _mjesec, _godina;
	void Unos(int d, int m, int g) { _dan = d; _mjesec = m; _godina = g; }
	void Ispis() { cout << _dan << "/" << _mjesec << "/" << _godina << endl; }
};
struct Osoba {
	Datum _datumRodjenja;
	char * _imePrezime;
	char _JMBG[14];
	Opstina _Prebivaliste;
	void unos(Datum d, const char * ip, const char jmbg[], Opstina p) {
		_datumRodjenja = d;
		_imePrezime = new char[strlen(ip) + 1];
		strcpy(_imePrezime, ip);
		strcpy(_JMBG, jmbg);
		_Prebivaliste = p;
	}
	void Dealociraj() { delete[] _imePrezime; _imePrezime = nullptr; }
	void Info() {
		_datumRodjenja.Ispis();
		cout << _imePrezime << " " << _JMBG << " " <<
			OpstineChar[_Prebivaliste] << endl;
	}
};
struct Glas {
	Osoba * _glasac;
	Kandidati * _glasZa;
	void Unos(Osoba o, Kandidati k) {
		_glasac = new Osoba;
		_glasac->unos(o._datumRodjenja, o._imePrezime, o._JMBG, o._Prebivaliste);
		_glasZa = new Kandidati(k);
	}
	void Ispis() {
		_glasac->Info();
		cout << KandidatiChar[*_glasZa] << endl;
	}
	void Dealociraj() { _glasac->Dealociraj(); }
};
struct Izbori2016 {
	Opstina * _glasackoMjesto;
	Glas * _glasovi[1000];
	int _doSadaGlasalo;
	/*
	1. Unos - na osnovu vrijednosti primljenih parametara, inicijalizovati vrijednosti atributa strukture.
	2. DodajGlas - funkcija treba da omoguci dodavanje informacija o novom glasu i tom prilikom onemoguciti: 
	da ista osoba glasa vise puta, glasanje osobama mladjim od 18 godina (uzeti u obzir samo mjesec i godinu rodjenja), 
	glasanje osobama iz drugih opstina. U zavisnosti od uspjesnosti operacije funkcija vraca true ili false

	3. Ispis - u zavisnosti od postignutog uspjeha funkcija treba ispisati listu kandidata sortiranu opadajucim redoslijedom, 
	a uz svakog kandidata je potrebno ispisati i osvojeni procenat glasova.
	4. BrojGlasacaPoRodjenju - funkcija vraca broj glasaca koji su do tog momenta glasali, 
	a koji su rodjeni u parametrima definisanom periodu (parametri su tipa datum, period OD i DO)
	*/

	void Unos(Opstina o)
	{
		_glasackoMjesto = new Opstina(o);
		for (size_t i = 0; i < 1000; i++)
		{
			_glasovi[i] = nullptr;
		}
		_doSadaGlasalo = 0;
	}
	bool DodajGlas(Glas g)
	{
		if (_doSadaGlasalo >= 1000)
			return false;

		for (size_t i = 0; i < _doSadaGlasalo; i++)
		{
			if (strcmp(_glasovi[i]->_glasac->_JMBG, g._glasac->_JMBG) == 0)
				return false;
		}
		if ((2016 - g._glasac->_datumRodjenja._godina) == 18)
			if (g._glasac->_datumRodjenja._mjesec > 7) // Izbori u 7 mjesecu
				return false;
		if ((2016 - g._glasac->_datumRodjenja._godina) < 18)
			return false;

		if (g._glasac->_Prebivaliste != *_glasackoMjesto)
			return false;

		_glasovi[_doSadaGlasalo] = new Glas;
		_glasovi[_doSadaGlasalo]->Unos(*g._glasac, *g._glasZa);
		_doSadaGlasalo++;
		return true;
	} 
	void Ispis()
	{
		int suma = 0;
		int BrojGlasova[4] = {0};
		for (size_t i = 0; i < _doSadaGlasalo; i++)
		{
			BrojGlasova[*(_glasovi[i]->_glasZa)] ++;
			suma++;
		}

		Sortiraj(BrojGlasova);
		float procenat;
		for (int i = 0; i < 4; i++)
		{
			int a = BrojGlasova[i];
			procenat = ((float)a / (float)suma); // Koefcijent
			procenat *= 100; // Pocenat
			cout << KandidatiChar[i] << " -> " << BrojGlasova[i] << " -> " << procenat<<"%"<<endl;
		}

	}
	int BrojGlasacaPoRodjenju(Datum gornjaGranica, Datum donjaGranica )
	{
		int brojGlasacaUnutarDatuma = 0;
		for (int i = 0; i < _doSadaGlasalo; i++)
		{
			bool dodaj = false;
			if (_glasovi[i]->_glasac->_datumRodjenja._godina > donjaGranica._godina && _glasovi[i]->_glasac->_datumRodjenja._godina < gornjaGranica._godina)
			{
				brojGlasacaUnutarDatuma++;
				continue;
			}
			else if (_glasovi[i]->_glasac->_datumRodjenja._godina == donjaGranica._godina)
			{
				if (_glasovi[i]->_glasac->_datumRodjenja._mjesec > donjaGranica._mjesec)
				{
					dodaj = true;
				}
				else if (_glasovi[i]->_glasac->_datumRodjenja._mjesec == donjaGranica._mjesec)
				{
					if (_glasovi[i]->_glasac->_datumRodjenja._dan >= donjaGranica._dan)
					{
						dodaj = true;
					}
				}
			}
			if (_glasovi[i]->_glasac->_datumRodjenja._godina == gornjaGranica._godina)
			{
				if (_glasovi[i]->_glasac->_datumRodjenja._mjesec < gornjaGranica._mjesec)
				{
					if (dodaj == true)
						brojGlasacaUnutarDatuma++;
				}
				else if (_glasovi[i]->_glasac->_datumRodjenja._mjesec == gornjaGranica._mjesec)
				{
					if (_glasovi[i]->_glasac->_datumRodjenja._dan <= gornjaGranica._dan)
					{
						if (dodaj == true)
							brojGlasacaUnutarDatuma++;
					}
				}
			}



		}
		return brojGlasacaUnutarDatuma;
	}
};

/*
1. UpisiUBinarniFajl - objekat koji je primljen kao parametar upisuje u binarni fajl. 
		Naziv fajla treba biti identican nazivu opstine u kojoj se odrzavaju izbori
2. UcitajIzBinarnogFajla - u objekat koji je primljen kao parametar ucitava sadrzaj binarnog fajla (naziv fajla je takodjer proslijedjen kao parametar)
3. PretragaRekurzivno - rekurzivna funkcija koja treba da vrati broj glasova koje je na izborima ostvario odredjeni kandidat 
		(kandidat se proslijedjuje kao parametar)
*/

void UpisiUBinarniFajl(Izbori2016 izbori)
{
	int vel = strlen("Opstinax") + strlen(".bin") + 1;
	char * nazivFajla = new char[vel];
	for (size_t i = 0; i < 4; i++)
	{
		if (*izbori._glasackoMjesto == (Opstina)i)
		{
			strcpy_s(nazivFajla, vel, OpstineChar[i]);
		}
	}
	strcat_s(nazivFajla, vel, ".bin");
	nazivFajla[vel - 1] = '\0';

	ofstream upis;
	upis.open(nazivFajla, ios::binary);
	upis.write((char *)(&izbori), sizeof(izbori));
	upis.close();
}
void UcitajIzBinarnogFajla(Izbori2016 &izbori, const char * naziv)
{
	int vel = strlen("opstinax") + strlen(".bin") + 1;
	char * nazivFajla = new char[vel];
	strcpy_s(nazivFajla, vel, naziv);
	strcat_s(nazivFajla, vel, ".bin");
	nazivFajla[vel - 1] = '\0';

	ifstream ispis;
	ispis.open(nazivFajla, ios::binary);
	ispis.read((char *)(&izbori), sizeof(izbori));
	ispis.close();
}

int BROJ_GLASOVA = 0;
int PretragaRekurzivno(Kandidati kandidat, Izbori2016 izbori, int DoSadaGlasalo)
{
	if (DoSadaGlasalo < 0)
		return BROJ_GLASOVA;
	if (*izbori._glasovi[DoSadaGlasalo]->_glasZa == kandidat)
	{
		BROJ_GLASOVA++;
	}

	PretragaRekurzivno( kandidat, izbori, DoSadaGlasalo - 1);
}


int main() {
	/*
	1. BROJ I VRSTA PARAMETARA MORAJU BITI IDENTICNI KAO U PRIMJERIMA. U SUPROTNOM SE RAD NEĆE BODOVATI
	2. STAVITE KOMENTAR NA DIJELOVE CODE-A KOJE NE BUDETE IMPLEMENTIRALI
	3. KREIRAJTE .DOC FAJL SA VAŠIM BROJEM INDEKSA ( NPR. IB130030.DOC BEZ IMENA I PREZIMENA), TE NA KRAJU ISPITA U NJEGA KOPIRAJTE RJEŠENJA VAŠIH ZADATAKA. NE PREDAVATI .TXT ILI .CPP FAJLOVE
	4. TOKOM IZRADE ISPITA NIJE DOZVOLJENO KORIŠTENJE HELP-A
	5. TOKOM IZRADE ISPITA MOGU BITI POKRENUTA SAMO TRI PROGRAMA: PDF READER (ISPITNI ZADACI), MS VISUAL STUDIO, MS WORD (U KOJI ĆETE KOPIRATI VAŠA RJEŠENJA)
	6. BEZ OBZIRA NA TO DA LI SU ISPITNI ZADACI URAĐENI, SVI STUDENTI KOJI SU PRISTUPILI ISPITU MORAJU PREDATI SVOJ RAD
	*/

	Datum datumGlasac1, datumGlasac2, datumGlasac3, datumGlasac4, datumIzbora;
	datumIzbora.Unos(14, 7, 2016);
	datumGlasac1.Unos(18, 5, 1990);
	datumGlasac2.Unos(5, 3, 1982);
	datumGlasac3.Unos(12, 8, 1958);
	datumGlasac4.Unos(22, 6, 2000);

	Osoba a, b, c, d, e;
	a.unos(datumGlasac1, "Denis Music", "1111111111111", Opstina1);
	b.unos(datumGlasac2, "Jasmin Azemovic", "1111111111112", Opstina1);
	c.unos(datumGlasac3, "Indira Hamulic", "1111111111113", Opstina1);
	d.unos(datumGlasac4, "Adel Handzic", "1111111111114", Opstina2);
	e.unos(datumGlasac4, "Zanin Vejzovic", "1111111111115", Opstina1);


	Glas g1, g2, g3, g4, g5;
	g1.Unos(a, Kandidat1);
	g2.Unos(b, Kandidat1);
	g3.Unos(c, Kandidat2);
	g4.Unos(d, Kandidat1);
	g5.Unos(e, Kandidat2);

	Izbori2016 izbori, izbori_FromFile;
	izbori.Unos(Opstina1);

	izbori.DodajGlas(g1);
	izbori.DodajGlas(g2);
	izbori.DodajGlas(g3);
	izbori.DodajGlas(g4);
	izbori.DodajGlas(g5);
	izbori.Ispis();
	cout << crt;
	Datum OD, DO;
	OD.Unos(17, 7, 1998);
	DO.Unos(17, 7, 1986);
	cout << "Glasaca u starost od 18 - 30 godina->" << izbori.BrojGlasacaPoRodjenju(OD, DO) << endl;
	cout << "Broj glasova za Kandidat1->" << PretragaRekurzivno(Kandidat1, izbori, izbori._doSadaGlasalo - 1) << endl;
	UpisiUBinarniFajl(izbori);
	UcitajIzBinarnogFajla(izbori_FromFile, "Opstina1");
	cout << crt;
	izbori_FromFile.Ispis();

	system("pause>0");
	return 0;
}

