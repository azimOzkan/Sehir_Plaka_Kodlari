#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>

using namespace std;

const int MAX_SEHIR_SAYISI = 20;

struct Sehir {       // iki boyutlu diziyle ekrana yazd�r�lacak �ehir ve plakalar�n struct'�
    int plaka;
    string isim;
};

struct AgacDugumu {    // a�a� d���m� yap�s�
    int plaka;
    string sehir;
    AgacDugumu* sol;
    AgacDugumu* sag;

    AgacDugumu(int plaka, string sehir) {    // a�a� d���m� constructor'�
        this->plaka = plaka;
        this->sehir = sehir;
        sol = NULL;
        sag = NULL;
    }
};

void ekle(AgacDugumu* &kok, int plaka, string sehir);
bool dengeliMi(AgacDugumu* kok);
int derinlik(AgacDugumu* kok);
void agacGoruntusu(AgacDugumu* kok, int seviye);
void preorderSirala(AgacDugumu* kok);
void inorderSirala(AgacDugumu* kok);
void postorderSirala(AgacDugumu* kok);
void sehirAra(AgacDugumu* kok, int plaka1, int plaka2);


void ekle(AgacDugumu* &kok, int plaka, string sehir) {   // a�aca yeni bir d���m ekleme fonksiyonu
    if (kok == NULL) {
        kok = new AgacDugumu(plaka, sehir);
        return;
    }

    if (plaka < kok->plaka) {
        ekle(kok->sol, plaka, sehir);
    } else if (plaka > kok->plaka) {
        ekle(kok->sag, plaka, sehir);
    }
}

int derinlik(AgacDugumu* kok) {      // a�ac�n derinili�ini hesaplayan fonksiyon
    if (kok == NULL)
        return 0;

    int solDerinlik = derinlik(kok->sol);
    int sagDerinlik = derinlik(kok->sag);

    return max (solDerinlik, sagDerinlik) + 1;
}

bool dengeliMi(AgacDugumu* kok) {     // a�ac�n dengeli olup olmad���n� belirleyen fonksiyon
    if (kok == NULL)
        return true;

    int solDerinlik = derinlik(kok->sol);
    int sagDerinlik = derinlik(kok->sag);

    int fark = abs(solDerinlik - sagDerinlik);

    return (fark <= 1 && dengeliMi(kok->sol) && dengeliMi(kok->sag));
}

void agacGoruntusu(AgacDugumu* kok, int seviye) {    // a�ac�n g�r�nt�s�n� ekrana basan fonksiyon
    if (kok == NULL)
        return;

    agacGoruntusu(kok->sag, seviye + 1);

    for (int i = 0; i < seviye; i++)
        cout << "    ";

    cout << kok->plaka << endl;

    agacGoruntusu(kok->sol, seviye + 1);
}

void preorderSirala(AgacDugumu* kok) {              // a�ac� preodrder s�ralama
    if (kok == NULL)
        return;

    cout << kok->plaka << "-";

    preorderSirala(kok->sol);
    preorderSirala(kok->sag);
}

void inorderSirala(AgacDugumu* kok) {               // a�ac� inorder s�ralama
    if (kok == NULL)
        return;

    inorderSirala(kok->sol);

    cout << kok->plaka << "-";

    inorderSirala(kok->sag);
}

void postorderSirala(AgacDugumu* kok) {             // a�ac� posterder s�ralama
    if (kok == NULL)
        return;

    postorderSirala(kok->sol);
    postorderSirala(kok->sag);

    cout << kok->plaka << "-";
}

void sehirAra(AgacDugumu* kok, int plaka1, int plaka2) {     // Verilen plaka kodlar�na g�re �ehirleri ve 
    AgacDugumu* dugum1 = NULL;                               // aralar�ndaki d�zey fark�n� ekrana basan fonksiyon
    AgacDugumu* dugum2 = NULL;

    AgacDugumu* simdiki = kok;
    while (simdiki != NULL) {
        if (simdiki->plaka == plaka1) {
            dugum1 = simdiki;
            break;
        } else if (plaka1 < simdiki->plaka) {
            simdiki = simdiki->sol;
        } else {
            simdiki = simdiki->sag;
        }
    }

    simdiki = kok;
    while (simdiki != NULL) {
        if (simdiki->plaka == plaka2) {
            dugum2 = simdiki;
            break;
        } else if (plaka2 < simdiki->plaka) {
            simdiki = simdiki->sol;
        } else {
            simdiki = simdiki->sag;
        }
    }

    if (dugum1 != NULL && dugum2 != NULL) {
        cout << "Sehirler: " << dugum1->sehir << ", " << dugum2->sehir << endl;

        int duzeyFarki = abs(derinlik(dugum1) - derinlik(dugum2));
        cout << "Agactaki duzey farki: " << duzeyFarki << endl;
    } else {
        cout << "Bir veya her iki plaka kodu agacta bulunamadi." << endl;
    }
}

int main() {
	
	Sehir sehirler[MAX_SEHIR_SAYISI];                   // �ki boyutlu dizi tan�m�
    int sehirSayisi = 0;
    
    ifstream dosya("sehir_plaka_kodlari.txt");          // dosya okuma i�lemleri

    if (!dosya) {
        cout << "Dosya ac�lamadi." << endl;
        return 0;
    }
    
  if (dosya.is_open()) {
        while (sehirSayisi < MAX_SEHIR_SAYISI && dosya >> sehirler[sehirSayisi].plaka >> sehirler[sehirSayisi].isim) {
            sehirSayisi++;
        }
        
    AgacDugumu* kok = NULL;
    string satir;
    while (getline(dosya, satir)) {
        stringstream ss(satir);
        string sehir;
        int plaka;
        ss >> plaka >> sehir;
        ekle(kok, plaka, sehir);
    }

    dosya.close();
    
    cout << "Dosyadan " << sehirSayisi << " adet sehir bilgisi okundu." << endl;
    for (int i = 0; i < sehirSayisi; i++) {
            cout << "Plaka: " << sehirler[i].plaka << ", Isim: " << sehirler[i].isim << endl;
        }
        
          for (int i = 0; i < sehirSayisi; i++) {
            ekle(kok, sehirler[i].plaka, sehirler[i].isim);
        }

    cout << "________________________________________" << endl;
    cout << "________________________________________" << endl; 
    
    cout << "Olusturulan agacin goruntusu:" << endl;
    agacGoruntusu(kok, 0);

    cout << "Derinlik: " << derinlik(kok) << ", ";
    if (dengeliMi(kok)) {
        cout << "Dengeli" << endl;
    } else {
        cout << "Dengeli degil" << endl;
    }

    cout << "----------------------------------------" << endl;

    int plaka1, plaka2;
    cout << "1. ve 2. Sehir icin plaka kodunu giriniz: ";
    cin >> plaka1 >> plaka2;

    sehirAra(kok, plaka1, plaka2);

    cout << "----------------------------------------" << endl;

    cout << "Preorder: ";
    preorderSirala(kok);
    cout << endl;

    cout << "Inorder: ";
    inorderSirala(kok);
    cout << endl;

    cout << "Postorder: ";
    postorderSirala(kok);
    cout << endl;

}
    return 0;
}

