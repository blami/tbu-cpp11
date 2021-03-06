/*
 * test.cpp: test implementace tridy AsciiKontext
 */

#include <iostream>
#include <string>
#include <sstream>
#include <memory>
#include "asciikontext.h"
#include "bod.h"
#include "kruh.h"
#include "polygony.h"
#include "ctverec.h"
#include "trojuhelnik.h"
#include "platno.h"

using namespace std;


// Pomocna funkce pro zadani bodu
obrazce::Bod ZadejBod(const string jmeno) {
	obrazce::Bod bod;

	cout << jmeno << " x: "; cin >> bod.x;
	cout << jmeno << " y: "; cin >> bod.y;

	return bod;
}

// Pomocna funkce pro zadani spolecnych parametru, ktere pouze ovlivnuji
// vykreslovani (posunuti a zvetseni)
void ZadejPosunutiAZvetseni(shared_ptr<obrazce::IObrazec> obrazec) {
	// Pokud neni nastaven obrazec neni co zadavat
	if(!obrazec)
		return;

	obrazce::Bod posunuti(0,0);
	double zvetseni = 1;

	posunuti = ZadejBod("Posunuti");
	cout << "Zvetseni: "; cin >> zvetseni;

	obrazec->ZadejPosunuti(posunuti);
	obrazec->ZadejZvetseni(zvetseni);
}

// Pomocna funkce pro vypis teziste a obsahu obrazce
void VypisTezisteAPlochu(shared_ptr<obrazce::IObrazec> obrazec) {
	if(!obrazec)
		return;

	obrazce::Bod t = obrazec->VypoctiTeziste();
	double s = obrazec->VypoctiObsah();

	cout << "Teziste: (" << t.x << "," << t.y << ") Obsah: " << s << endl;
	cout << "Pozn.: vypocet teziste a obsahu neuvazuje posunuti ani zvetseni" << endl;
}

// Pomocna funkce pro zadani kruhu
shared_ptr<obrazce::Kruh> ZadejKruh() {
	double polomer;
	obrazce::Bod stred;
	shared_ptr<obrazce::Kruh> kruh;

	cout << "Zadejte parametry kruhu" << endl;
	cout << "Polomer: ";
	cin >> polomer;
	stred = ZadejBod("Stred");

	kruh = make_shared<obrazce::Kruh>(polomer, stred);
	ZadejPosunutiAZvetseni(kruh);

	VypisTezisteAPlochu(kruh);

	return kruh;
}

// Pomocna funkce pro zadani ctverce
shared_ptr<obrazce::Ctverec> ZadejCtverec() {
	double a;
	obrazce::Bod stred;
	shared_ptr<obrazce::Ctverec> ctverec;

	cout << "Zadejte parametry ctverce" << endl;
	cout << "Delka strany: ";
	cin >> a;
	stred = ZadejBod("Stred");

	ctverec = make_shared<obrazce::Ctverec>(a, stred);
	ZadejPosunutiAZvetseni(ctverec);

	VypisTezisteAPlochu(ctverec);

	return ctverec;
}

// Pomocna funkce pro zadani trojuhelniku
shared_ptr<obrazce::Trojuhelnik> ZadejTrojuhelnik() {
	obrazce::Bod a, b, c;
	shared_ptr<obrazce::Trojuhelnik> trojuhelnik;

	cout << "Zadejte parametry trojuhelniku" << endl;
	a = ZadejBod("Bod a");
	b = ZadejBod("Bod b");
	c = ZadejBod("Bod c");

	trojuhelnik = make_shared<obrazce::Trojuhelnik>(a, b, c);
	ZadejPosunutiAZvetseni(trojuhelnik);

	VypisTezisteAPlochu(trojuhelnik);

	return trojuhelnik;
}

// Pomocna funkce pro zadani polygonu
shared_ptr<obrazce::Polygon> ZadejPolygon() {
	vector<obrazce::Bod> body;
	int n;
	shared_ptr<obrazce::Polygon> polygon;

	cout << "Zadejte parametry polygonu (konvexni obal)" << endl;
	cout << "Pocet bodu: ";
	cin >> n;
	if(n < 3)
		return NULL;

	for(int i = 0; i < n; i++) {
		ostringstream s;
		s << "Bod c." << i + 1;
		body.push_back(ZadejBod(s.str()));
	}

	polygon = make_shared<obrazce::Polygon>(body);
	ZadejPosunutiAZvetseni(polygon);

	VypisTezisteAPlochu(polygon);

	return polygon;
}


// Pozn.: aplikace nijak neosetruje neplatny vstup.
int main()
{
	shared_ptr<obrazce::IKontext> kontext =
		make_shared<obrazce::AsciiKontext>(20,20);
	obrazce::Platno platno(kontext);

	bool konec = false;
	while(!konec) {
		char operace = 0;

		// Vypsat menu
		cout << "Menu:" << endl
			<< "k	pridat kruh" << endl
			<< "c	pridat ctverec" << endl
			<< "t	pridat trojuhelnik" << endl
			<< "p	pridat polygon" << endl
			<< "z	zobrazit platno" << endl
			<< "v	vymaz platno" << endl
			<< "x	konec" << endl
			<< "Volba: ";
		cin >> operace;

		switch(operace) {
			case 'k':
				platno.Pridej(ZadejKruh());
				break;
			case 'c':
				platno.Pridej(ZadejCtverec());
				break;
			case 't':
				platno.Pridej(ZadejTrojuhelnik());
				break;
			case 'p':
				platno.Pridej(ZadejPolygon());
				break;
			case 'z':
				platno.Zobraz();
				break;
			case 'v':
				platno.OdstranVsechnyObrazce();
				cout << "Platno bylo vymazano" << endl;
				break;
			case 'x':
				konec = true;
				continue;
			default:
				cout << "CHYBA: neplatna volba '" << operace << "'!" << endl;
				continue;
		}
		cin.clear();

		// Zobrazit platno
	}

	return 0;
}
