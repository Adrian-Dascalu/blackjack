#include <iostream>
#include <stdlib.h>
#include <conio.h>
#include <stdio.h>
#include <random>
#include <time.h>
#include <ctime>
#include <cstdlib>
#include <string.h>
#include <sstream>

using namespace std;

const char* valoare_carte[13] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};

const char* culoare_carte[4] = {"ROMB", "INIMA ROSIE", "TREFLA", "INIMA NEAGRA"};

class Pachet_Carti
{
    public:
        class Carte_De_Joc
        {
            private:
                int valoare;
                int culoare;

            public:
                void Seteaza(int x)
                {
                    valoare = x % 13;
                    culoare = x / 13;
                }

                int Valoare(void)
                {
                    return valoare;
                }

                int Culoare(void)
                {
                    return culoare;
                }

                void Display(void);

                int transf(void);
        };
    
    private:
        Carte_De_Joc cards[52];

        int next_card;
        
        public:
            void Creeaza_Pachet(void)
            {
                for (int i = 0; i < 52; i++) cards[i].Seteaza(i);

                next_card = 0;
            }

            void Amesteca(void)
            {
                int i, k;
            
                Carte_De_Joc temp;
        
                srand(time(NULL));
                
                for(i = 0; i < 52; i++)
                {
                    k = rand() % 52;
            
                    temp = cards[i];
                    cards[i] = cards[k];
                    cards[k] = temp;
                }
            }

            void Bet(int n)
            {
                string user_input;

                for(int i = 0; i < n; i++)
                {
                    cout << "Bet jucator " << i + 1 << " : ";
                    while(1)
                    {
                        getline(cin, user_input);
                        stringstream convert(user_input);
                        if(convert >> suma_bet[i] &&!(convert >> user_input))
                        {
                            break;
                        }
                        else
                        {
                        cin.clear();
                        cout << "Pune ma un bet numar real : ";
                        }
                    }
                };

                nr_jucatori = n;
            }

            void impartire()
            {
                for(int i = 0; i <= nr_jucatori; i++)
                {
                    nr_asi[i] = 0;
                    nr_asi1[i] = 0;
                    bust[i] = 0;
                    blackjack[i] = 0;
                    nr_carte[i] = 2;   

                    for(int k = 0; k < 2; k++)
                    {
                        carti_mana[i][k] = cards[carti_impartite];

                        if(carti_mana[i][k].transf() == 11)
                        {
                            nr_asi[i]++;
                        }
                        
                        carti_impartite++;
                    }

                    suma_mana[i] = carti_mana[i][0].transf() + carti_mana[i][1].transf();
                    
                    if(suma_mana[i] > 21)
                    {
                        suma_mana[i] -= 10;
                        nr_asi1[i]++;
                    }
                    else if(suma_mana[i] == 21)
                    {
                        blackjack[i] = 1;
                    }
                }
            }

            void afisare_masa()
            {
                for(int i = 0; i < nr_jucatori; i++)
                {
                    cout << "Jucator " << i + 1 << " carti:\n";

                    carti_mana[i][0].Display();
                    carti_mana[i][1].Display();

                    cout << endl << endl;

                    cout << "bet : " << suma_bet[i] << endl << endl;
                }

                cout << "Carti Dealer :\n";
                carti_mana[nr_jucatori][0].Display();
                cout << endl;
            }

            void alege(int i)
            {
                if(nr_carte[i] == 2)
                {
                    cout << "\nhit / stay / double / split\n";
                }
                else
                {
                    cout << "\nhit / stay\n";
                }

                char tura[10];

                cin >> tura;

                if(strcmp(tura, "hit") == 0)
                {
                    hit(i, 0);
                }
                else if(strcmp(tura, "stay") == 0)
                {
                    cout << endl;

                    return;
                }
                else if((strcmp(tura, "double") == 0) && (nr_carte[i] == 2))
                {
                    suma_bet[i] *= 2;
                    hit(i, 1);

                    return;
                }
                else if((strcmp(tura, "split") == 0) && (nr_carte[i] == 2))
                {
                    if(carti_mana[i][0].transf() == carti_mana[i][1].transf())
                    {
                        nr_asi1[i] = 0;
                        cout << "\nprima mana : \n";
                        suma_mana[i] = carti_mana[i][0].transf();
                        carti_split[i][0][0] = carti_mana[i][0];
                        hitsplit1(i);

                        nr_asi1[1] = 0;
                        cout << "\na doua mana : \n";
                        suma_mana[i] = carti_mana[i][1].transf();
                        carti_split[i][1][0] = carti_mana[i][1];
                        hitsplit2(i);
                    }
                    else
                    {
                        cout << "nu poti da split";
                        alege(i);
                    }
                }
                else
                {
                    cout << "ce vrei sa faci ma?\n";

                    alege(i);
                }
            }

            void hit(int i, int d)  
            {
                carti_mana[i][nr_carte[i]] = cards[carti_impartite];
                suma_mana[i] += carti_mana[i][nr_carte[i]].transf();

                if(carti_mana[i][nr_carte[i]].transf() == 11)
                {
                    nr_asi[i]++;
                }

                carti_impartite++;
                nr_carte[i]++;

                if(suma_mana[i] == 21)
                {
                    blackjack[i] = 1;

                    for(int j = 0; j < nr_carte[i]; j++)
                    {
                        carti_mana[i][j].Display();
                    }

                    cout << "\n\nsuma : 21\n\nBLACKJACK!" << endl;
                    getchar();

                    return;
                }
                else if(suma_mana[i] > 21)
                {   
                    for(int j = 0; j < nr_carte[i]; j++)
                    {
                        if(nr_asi[i] > nr_asi1[i])
                        {
                            suma_mana[i] -= 10;
                            nr_asi1[i]++;

                            if(suma_mana[i] == 21)
                            {
                                blackjack[i] = 1;

                                for(int j = 0; j < nr_carte[i]; j++)
                                {
                                    carti_mana[i][j].Display();
                                }

                                cout << "\n\nsuma : 21\n\nBLACKJACK!" << endl;
                                getchar();

                                return;
                            }

                            for(int j = 0; j < nr_carte[i]; j++)
                            {
                                carti_mana[i][j].Display();
                            }

                            cout << "\n\nsuma : " << suma_mana[i] << endl;
                            if(!d) alege(i);

                            return;
                        }
                    }

                    for(int j = 0; j < nr_carte[i]; j++)
                    {
                        carti_mana[i][j].Display();
                    }

                    cout << "\n\nsuma : " << suma_mana[i];
                    bust[i] = 1;

                    cout << "\n\nAi pierdut, ghinion\n\n---||---\n";
                    getchar();

                    return;
                }
                else
                {
                    for(int j = 0; j < nr_carte[i]; j++)
                    {
                        if((d == 2) && (j == 0))
                        {}
                        else if((d == 3) && (j == 1))
                        {}
                        else
                        carti_mana[i][j].Display();
                    }

                    cout << endl << endl << "suma : " << suma_mana[i] << endl;
                }

                if(d == 1) cout << endl;

                if(!d) alege(i);
            }

            void hitsplit1(int i)
            {

            }

            void hitsplit2(int i)
            {

            }

            void mana()
            {
                for(int i = 0; i < nr_jucatori; i++)
                {
                    cout << "Randul jucatorului " << i + 1 << endl;

                    carti_mana[i][0].Display();
                    carti_mana[i][1].Display();

                    cout << endl << endl;
                    cout << "bet : " << suma_bet[i] << endl;
                    cout << "suma : " << suma_mana[i] << endl;

                    if(suma_mana[i] == 21)
                    {
                        blackjack[i] = 1;

                        cout << "\nBLACKJACK!" << endl;
                    }
                    else
                    {
                        alege(i);
                    }

                    getchar();
                }

                sfarsit();
            }

            void sfarsit()
            {
                int i = nr_jucatori;

                if(suma_mana[i] == 21)
                {
                    blackjack[i] = 1;
                }

                while((bust[i] == 0) && (dealer_win == 0) && (blackjack[i] == 0))
                {
                    if(suma_mana[i] == 21)
                    {
                        blackjack[i] = 1;
                    }

                    for(int j = 0; j < nr_jucatori; j++)
                    {
                        if(bust[j])
                        {
                            dealer_win++;
                        }
                    }

                    if(dealer_win >= nr_jucatori / 2)
                    {
                        dealer_win = 1;
                    }
                    else
                    {
                        dealer_win = 0;
                    }

                    for(int j = 0; j < nr_jucatori; j++)
                    {
                        if(suma_mana[i] > suma_mana[j])
                        {
                            dealer_win = 1;

                            break;
                        }
                    }
                    
                    if((dealer_win == 0) && (bust[i] == 0) && (blackjack[i] == 0) && (suma_mana[i] < 17) || (suma_mana[i] < 11))
                    {
                        carti_mana[i][nr_carte[i]] = cards[carti_impartite];
                        suma_mana[i] += carti_mana[i][nr_carte[i]].transf();

                        if(carti_mana[i][nr_carte[i]].transf() == 11)
                        {
                            nr_asi[i] += 1;
                        }

                        if(suma_mana[i] == 21)
                        {
                            blackjack[i] = 1;
                        }

                        if(suma_mana[i] > 21)
                        {
                            if(nr_asi[i] > nr_asi1[i])
                            {
                                suma_mana[i] -= 10;
                                nr_asi1[i]++;
                            }
                            else
                            {
                                bust[i] = 1;
                            }
                        }

                        nr_carte[i]++;
                        carti_impartite++;
                    }
                }

                for(i = 0; i < nr_jucatori; i++)
                {
                    cout << "Jucator : " << i + 1 << endl;

                    for(int j = 0; j < nr_carte[i]; j++)
                    {
                        carti_mana[i][j].Display();
                    }
                    
                    cout << endl << endl;
                    
                    cout << "suma : " << suma_mana[i] << endl;

                    if(bust[i])
                    {
                        cout << "Bust, ai pierdut : " << suma_bet[i] << endl << endl;
                    }
                    else if((blackjack[i]) && ((suma_mana[i] > suma_mana[nr_jucatori]) || (suma_mana[i] < suma_mana[nr_jucatori])))
                    {
                        cout << "Blackjack, ai castigat : " << suma_bet[i] * 2 << endl << endl;
                    }
                    else if((blackjack[i]) && (suma_mana[i] == suma_mana[nr_jucatori]))
                    {
                        cout << "Egalitate, libertate, fraternitate, ai primit inapoi : " << suma_bet[i] << endl << endl;
                    }
                    else if(suma_mana[i] > suma_mana[nr_jucatori])
                    {
                        cout << "Ai castigat : " << suma_bet[i] * 2 << endl << endl;
                    }
                    else if(bust[nr_jucatori])
                    {
                        cout << "Ai castigat : " << suma_bet[i] * 2 << endl << endl;
                    }
                    else
                    {
                        cout << "Ai pierdut : " << suma_bet[i] << endl << endl;
                    }
                }

                cout << "Mana dealer :\n";
                
                for(int j = 0; j < nr_carte[nr_jucatori]; j++)
                {
                    carti_mana[nr_jucatori][j].Display();
                }

                cout << "\n\nsuma : " << suma_mana[i] << endl;
            }

            int dealer_win = 0;
            int nr_asi[8];
            int nr_asi1[8];
            int carti_impartite = 0;
            int nr_carte[8];
            int blackjack[8];
            int bust[8];
            int lose[8];
            int valori_c[13];
            float suma_bet[8];
            int nr_jucatori;
            int suma_mana[8];

            Carte_De_Joc carti_split[8][10][2];
            Carte_De_Joc carti_mana[8][10];
};

int Pachet_Carti::Carte_De_Joc::transf(void)
{
    if((Valoare() > 8) && (Valoare() < 12)) return 10;
    else if(Valoare() == 12) return 11;
    else return Valoare() + 2;
}

void Pachet_Carti::Carte_De_Joc::Display(void)
{
    cout << endl << valoare_carte[Valoare()] << " DE " << culoare_carte[Culoare()];
}

int main (void)
{
    int n;
    string user_input;
   
    cout << "Numar jucatori : ";
    while(1)
    {
        getline(cin, user_input);
        stringstream convert(user_input);
        if(convert >> n &&!(convert >> user_input) && (n > 0) && (n < 8))
        {
            break;
        }
        else
        {
           cin.clear();
           cout << "Baga ma un numar de jucatori intre 1 si 7 : ";
        }
    }

    cout << endl;

    Pachet_Carti::Carte_De_Joc carte;

    Pachet_Carti blackjack;

    blackjack.Creeaza_Pachet();

   // system("clear");

    blackjack.Amesteca();

   // system("clear");

    blackjack.Bet(n);

   // system("clear");

    blackjack.impartire();

    blackjack.afisare_masa();
    
    getchar();
   //
    system("clear");

    blackjack.mana();
}