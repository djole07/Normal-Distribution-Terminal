
/**********************************************************
*
*       Crtanje Gausove krive pomocu nasumicnih brojeva
*
*
*       Autor: Djordje Mirosavic
*
*
************************************************************/

#include <iostream>
#include <random>
#include <functional>
#include <chrono>
#include <algorithm>

#define PI 3.14159

#define X_PODEOKA 100

#define Y_PODEOKA 70

#define BROJ_UZORAKA 73167

#define ZNAK_ZA_ISPIS '*'       // kojim znakom zelimo da iscrtamo krivu

#define OPSEG 3                 // odredjuje koliko devijacija zelimo da ispisemo levo i desno od srednje vrednosti

using namespace std;



typedef chrono::high_resolution_clock hrc_t;
hrc_t::time_point start = hrc_t::now();


void inicijalizuj(char Grafik[][X_PODEOKA]) {
    for(int i = 0 ; i < Y_PODEOKA ; ++i) {
        for(int j = 0 ; j < X_PODEOKA ; ++j) {
            Grafik[i][j] = ' ';
        }
    }

}

void stampaj(char Grafik[][X_PODEOKA], double maxValue, double srednjaVr, double devijacija) {
    cout << endl << endl;

    cout << "\t\t\t" << "Grafik Gausove raspodele  za " << BROJ_UZORAKA << " uzoraka" << endl << endl;

    for(int i = 0 ; i < Y_PODEOKA ; ++i) {
        printf("\t%.2f-|", maxValue - i * maxValue/Y_PODEOKA);
        for(int j = 0 ; j < X_PODEOKA ; ++j) {
            cout << Grafik[i][j];
        }
        cout << endl;
    }

    cout << endl <<  "            " << srednjaVr - OPSEG * devijacija << string(X_PODEOKA / 2 - 3,'-') << srednjaVr << string(X_PODEOKA / 2 - 3,'-')
    << srednjaVr + OPSEG * devijacija << endl;

}

int main()
{
    default_random_engine generator;
    hrc_t::duration d = hrc_t::now() - start;

    double srednjaVr;
    double devijacija;

    cout << "Unesite srednju vrednost: ";
    cin >> srednjaVr;

    cout << "Unesite standardnu devijaciju : ";
    cin >> devijacija;

    normal_distribution<double> rn(srednjaVr, devijacija);
    auto dn = bind(rn, generator);
    generator.seed(d.count());

    vector<double> randomNiz;           // tu cemo cuvati vrednosti koju su nasumicno generisane

    char Grafik[Y_PODEOKA][X_PODEOKA];  // tu cuvamo grafik

    inicijalizuj(Grafik);               // postavljamo sve vrednosti na prazan znak


    for(long i = 0 ; i < BROJ_UZORAKA ; ++i) {
        double broj = dn();


        if(broj > srednjaVr - OPSEG * devijacija && broj < srednjaVr + OPSEG * devijacija) {
            randomNiz.push_back(broj);

        }

        // ovaj if sluzi ukoliko zelimo da uzmemo samo vrednosti iz datog opsega
        // Ukoliko ne zelimo, onda cemo imati da se sve vrednosti manje od srednjaVr - OPSEG * devijacija biti uracunate u prvi podopseg (prvi stub koji se
        // ispisuje) pa otuda dolazi taj skok na pocetku. Za promenu konstante OPSEG mozemo da 'razvucemo' krivu i onda cemo videti i te vrednosti koju
        // su prvobitno bile uracunate u taj prvi opseg. Za vecu tacnost, povecajte Y_PODEOKA i X_PODEOKA

        //randomNiz.push_back(broj);

    }

    int duzinaNiza = randomNiz.size();

    sort(randomNiz.begin(), randomNiz.end());       // sortiramo niz u rastucem poretku da bismo mogli da nadjemo broj pojavljivanja cifara u poditervalima

    double maxValue = 1 / (devijacija * sqrt(2 * PI));        // predstavlja najvecu vrednost koju y moze da ima za zadatu standardnu devijaciju

    // korak za kretanje po x i y osi
    const double y_podeok = maxValue / Y_PODEOKA;               // podelim y na Y_PODEOKA jednakih intervala i imamo koliko iznosi 1 podeok
    const double x_podeok = 2 * OPSEG * devijacija / X_PODEOKA; // ista ideja kao i kod y_podeok


    vector<int> brojPojavljivanja;


    double deltaX = srednjaVr - OPSEG * devijacija + x_podeok;
    // Posto treba da nadjemo koliko su se puta neki brojevi pojavili u odredjenom opsegu onda cemo da prolazimo kroz svaki taj opseg i brojimo pojavljivanje
    // cifara u tim granicama. Granica opsega ce biti deltaX i ona predstavlja gornje ogranicenje opsega. Npr. ukoliko imamo 80 opsega onda ce prvi biti od
    // [pocetak_podopsega, pocetak_podopsega  + x_podeok] . deltaX predstavlja ovo pocetak_podopsega  + x_podeok. I ako hocemo da udjemo u sledeci podopseg
    // potrebno je samo da dodamo x_podeok

    int i = 0;      // za kretanje po nizu

    int brojac;     // brojac pojavljivanja cifara iz oderdjenog opsega


    while(deltaX <= srednjaVr + OPSEG * devijacija) {

        brojac = 0;

        while(i < duzinaNiza && randomNiz[i] < deltaX) {
            ++brojac;
            ++i;
        }

        brojPojavljivanja.push_back(brojac);

        deltaX += x_podeok;
    }


    long brojZvezdica = Y_PODEOKA * 40;
    // koliko znakova ce biti raspodeljeno na grafiku. Vrednost je uzeta eksperimentalno


    vector<int> visine;     // zadajemo visinu ('y vrednost') za svaki 'stub' koji ispisujemo

    vector<int>::iterator it;


    for(it = brojPojavljivanja.begin() ; it != brojPojavljivanja.end() ; ++it) {
        visine.push_back(*it * brojZvezdica / BROJ_UZORAKA);
    }

    vector<int>::iterator it2 = visine.begin();


    // I, konacno, crtamo grafik

    for(int j = 0 ; j < X_PODEOKA && it2 != visine.end() ; ++j, ++it2) {

        for(int i = 0 ; i < *it2 ; ++i) {
            Grafik[Y_PODEOKA - 1 - i][j] = ZNAK_ZA_ISPIS;       // za crtanje odozdo na gore
        }
    }

    stampaj(Grafik, maxValue, srednjaVr, devijacija);

    return 0;
}
