#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <string>
#include <SFML/Graphics.hpp>

using std::vector, std::cin, std::string, std::cout, std::endl,std::pair,std::random_device,std::mt19937;

/////////////////////////////////////////
//Klasa reprezentująca talię kart do gry
/////////////////////////////////////////
class Karty {
public:
    //wektory reprezentujące wartości kart i kolory
    vector<string> karta = { "2", "3", "4", "5", "6", "7", "8", "9", "10", "Walet", "Dama", "Krol", "As" };
    vector<string> kolor = { "pik", "kier", "karo", "trefl" };

    //wektor par reprezentujący karty w talii
    vector<pair<string, string>> talia;

    //konstruktor inicjalizujący talie kart
    Karty() {
        for (int i = 0; i < kolor.size(); ++i) {
            for (int j = 0; j < karta.size(); ++j) {
                talia.push_back(make_pair(karta[j], kolor[i]));
            }
        }
    }
};


//////////////////////////////////////
// Klasa reprezentująca grę w oczko
//////////////////////////////////////
class Oczko {
private:
    // wektory przechowujące talie kart oraz ręce gracza i komputera
    vector<pair<string, string>> taliaKart;
    vector<pair<string, string>> rekaGracza;
    vector<pair<string, string>> rekaKomputera;

public:
    // konstruktor inicjalizujący talie kart i tasujący je
    Oczko() {
        Karty talia;
        taliaKart = talia.talia;
        tasowanie();
    }

    // tasowanie talii kart
    void tasowanie() {
        random_device rd;
        mt19937 g(rd());
        shuffle(taliaKart.begin(), taliaKart.end(), g);
    }

    int wartoscReki(const vector<pair<string, string>>& reka) {
        int wartosc = 0;
        int liczbaAsow = 0;

        for (int i = 0; i < reka.size(); ++i) {
            if (reka[i].first == "As") {
                wartosc += 11;
                liczbaAsow++;
            }
            else if (reka[i].first == "Walet" || reka[i].first == "Dama" || reka[i].first == "Krol") {
                wartosc += 10;
            }
            else {
                wartosc += stoi(reka[i].first);
            }
        }

        while (wartosc > 21 && liczbaAsow > 0) {
            wartosc -= 10;
            liczbaAsow--;
        }

        return wartosc;
    }

    void rozdaniePoczatkowe() {
        dobierzKarte(rekaGracza);
        dobierzKarte(rekaKomputera);
        dobierzKarte(rekaGracza);
        //dobierzKarte(rekaKomputera);
    }

    void pokazReke(const vector<pair<string, string>>& reka) {
        for (int i = 0; i < reka.size(); i++) {
            cout << reka[i].first << " " << reka[i].second;
            if (i < reka.size() - 1) {
                cout << ", ";
            }
        }
        cout << endl;
    }

    void dobierzKarte(vector<pair<string, string>>& reka) {
        reka.push_back(taliaKart.back());
        taliaKart.pop_back();
    }

    void graj() {
        char wybor;
        do {
            cout << "Czy chcesz dobrac karte? (D - dobierz, S - stoj): ";
            cin >> wybor;

            if (wybor == 'D' || wybor == 'd') {
                dobierzKarte(rekaGracza);
                cout << "Twoje karty: ";
                pokazReke(rekaGracza);
                cout << "Wartość Twoich kart: " << wartoscReki(rekaGracza) << endl;

                if (wartoscReki(rekaGracza) > 21) {
                    cout << "Przekroczyles 21! Przegrales." << endl;
                    return;
                }
            }
            else if (wybor == 'S' || wybor == 's') {
                break;
            }
            else {
                cout << "Niepoprawny wybor. Wybierz ponownie." << endl;
            }
        } while (true);

        cout << "Ruch krupiera:" << endl;
        while (wartoscReki(rekaKomputera) < 17 || (wartoscReki(rekaKomputera) == 17 && rekaKomputera.size() == 2)) {
            dobierzKarte(rekaKomputera);
        }

        int wartoscGracza = wartoscReki(rekaGracza);
        int wartoscKomputera = wartoscReki(rekaKomputera);

        cout << "Twoje karty: ";
        pokazReke(rekaGracza);
        cout << "Wartość Twoich kart: " << wartoscGracza << endl;

        cout << "Karty komputera: ";
        pokazReke(rekaKomputera);
        cout << "Wartość kart komputera: " << wartoscKomputera << endl;

        if (wartoscGracza > 21 || (wartoscKomputera <= 21 && wartoscKomputera >= wartoscGracza)) {
            cout << "Przegrałeś." << endl;
        }
        else if (wartoscGracza == wartoscKomputera) {
            cout << "Remis!" << endl;
        }
        else {
            cout << "Gratulacje! Wygrałeś." << endl;
        }
    }

    vector<pair<string, string>>& getRekaGracza() {
        return rekaGracza;
    }

    vector<pair<string, string>>& getRekaKomputera() {
        return rekaKomputera;
    }
};

string nazwaKartyNaPlik(const pair<string, string>& karta) {
    return "obrazy_kart/" + karta.first + "_" + karta.second + ".png";
}

void rysujKarte(sf::RenderWindow& window, const pair<string, string>& karta, float x, float y) {
    sf::Texture texture;
    texture.loadFromFile(nazwaKartyNaPlik(karta));

    sf::Sprite cardSprite(texture);
    cardSprite.setPosition(x, y);
    window.draw(cardSprite);
}

void rysujAktualneKarty(sf::RenderWindow& window, Oczko& oczko) {
    // Rysowanie kart gracza
    float x = 50.0f;
    float y = 300.0f;
    vector<pair<string, string>>& rekaGracza = oczko.getRekaGracza();
    for (int i = 0; i < rekaGracza.size();i++) {
        rysujKarte(window, rekaGracza[i], x, y);
        x += 182.0f;
    }

    // Rysowanie kart komputera
    x = 50.0f;
    y = 100.0f;
    vector<pair<string, string>>& rekaKomputera = oczko.getRekaKomputera();
    for (int i = 0; i < rekaKomputera.size(); i++) {
        rysujKarte(window, rekaKomputera[i], x, y);
        x += 182.0f;
    }
}

int main() {
    char grajPonownie;

    do {
        sf::RenderWindow window(sf::VideoMode(800, 600), "Oczko - Gra Karciana");

        // Inicjalizacja czcionki
        sf::Font font;
        if (!font.loadFromFile("arial.ttf")) {
            // Obsługa błędu ładowania czcionki
            return EXIT_FAILURE;
        }

        // Konfiguracja tekstu
        sf::Text instructions;
        instructions.setFont(font);
        instructions.setCharacterSize(16);
        instructions.setFillColor(sf::Color::White);
        instructions.setString("Klawisz D = dobierz\nKlawisz S = stój                                          INFORMACJE O ROZGRYWCE W KONSOLI\n                                                                   po zakonczeniu rozgrywki prosze wpisac w konsoli T lub N ");
        instructions.setPosition(10, 10);

        Oczko oczko;
        oczko.rozdaniePoczatkowe();

        bool graczPrzekroczyl21 = false;

        while (window.isOpen() && !graczPrzekroczyl21) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
                else if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::D) {
                        oczko.dobierzKarte(oczko.getRekaGracza());
                        cout << "Twoje karty: ";
                        oczko.pokazReke(oczko.getRekaGracza());
                        cout << "Wartość Twoich kart: " << oczko.wartoscReki(oczko.getRekaGracza()) << endl;

                        window.clear();
                        rysujAktualneKarty(window, oczko);
                        window.draw(instructions);  // Dodanie napisu na górze okna
                        window.display();

                        if (oczko.wartoscReki(oczko.getRekaGracza()) > 21) {
                            cout << "Przekroczyłeś 21! Przegrałeś." << endl;
                            graczPrzekroczyl21 = true;
                        }
                    }
                    else if (event.key.code == sf::Keyboard::S) {
                        graczPrzekroczyl21 = true;
                    }
                }
            }

            if (!graczPrzekroczyl21) {
                window.clear();
                rysujAktualneKarty(window, oczko);
                window.draw(instructions);  // Dodanie napisu na górze okna
                window.display();
            }
        }

        // Ruch komputera
        while (oczko.wartoscReki(oczko.getRekaKomputera()) < 17 || (oczko.wartoscReki(oczko.getRekaKomputera()) == 17 && oczko.getRekaKomputera().size() == 2)) {
            oczko.dobierzKarte(oczko.getRekaKomputera());

            window.clear();
            rysujAktualneKarty(window, oczko);
            window.draw(instructions);  // Dodanie napisu na górze okna
            window.display();
        }

        // Wyniki gry
        int wartoscGracza = oczko.wartoscReki(oczko.getRekaGracza());
        int wartoscKomputera = oczko.wartoscReki(oczko.getRekaKomputera());

        cout << "Twoje karty: ";
        oczko.pokazReke(oczko.getRekaGracza());
        cout << "Wartość Twoich kart: " << wartoscGracza << endl;

        cout << "Karty komputera: ";
        oczko.pokazReke(oczko.getRekaKomputera());
        cout << "Wartość kart komputera: " << wartoscKomputera << endl;

        if (wartoscGracza > 21 || (wartoscKomputera <= 21 && wartoscKomputera >= wartoscGracza)) {
            cout << "Przegrałeś." << endl;
        }
        else if (wartoscGracza == wartoscKomputera) {
            cout << "Remis!" << endl;
        }
        else {
            cout << "Gratulacje! Wygrałeś." << endl;
        }

        cout << "Czy chcesz zagrać ponownie? (T - tak, N - nie): ";
        cin >> grajPonownie;

    } while (grajPonownie == 'T' || grajPonownie == 't');

    return 0;
}

