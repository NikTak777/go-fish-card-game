#include <iostream>
#include <cstdlib>
#include <ctime>

//extern "C" signed char asdfcascf();

class SetsOfCards {
public:
    signed char* deck_of_cards = new signed char[13] {'2', '3', '4', '5', '6', '7', '8', '9', '0', 'J', 'Q', 'K', 'A'};
    signed char* count_of_cards = new signed char[13] {4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4};
    signed char* player_cards = new signed char[13] {};
    signed char* opponent_cards = new signed char[13] {};

    ~SetsOfCards() {
        delete[] deck_of_cards;
        delete[] count_of_cards;
        delete[] player_cards;
        delete[] opponent_cards;
    }

    void print() {
        std::cout << "Set of suits:\t\t";
        for (int i = 0; i < 13; ++i) {
            std::cout << deck_of_cards[i] << " ";
        }
        std::cout << std::endl << "Dest of cards:\t\t";
        for (int i = 0; i < 13; ++i) {
            std::cout << (int)count_of_cards[i] << " ";
        }
        std::cout << std::endl << "Player's cards:\t\t";
        for (int i = 0; i < 13; ++i) {
            std::cout << (int)player_cards[i] << " ";
        }
        std::cout << std::endl << "Opponent's cards:\t";
        for (int i = 0; i < 13; ++i) {
            std::cout << (int)opponent_cards[i] << " ";
        }
        std::cout << std::endl;
    }

    void give_player_card() {
        signed char new_card = rand() % 13;
        player_cards[(int)new_card]++;
        count_of_cards[(int)new_card]--;
    }

    void give_opponent_card() {
        signed char new_card = rand() % 13;
        opponent_cards[(int)new_card]++;
        count_of_cards[(int)new_card]--;
    }

    signed char get_count_player_card() {
        signed char count = 0;
        for (int i = 0; i < 13; ++i) {
            if ((int)player_cards[i] > 0) {
                ++count;
            }
        }
        return count;
    }
};

class GameLoop {
public:
    SetsOfCards Cards;
    short cur_card;
    short count_of_cards;

    void Loop() {
        while (true) {
            Cards.print();

            print_player_display();

            select_player_card();

            short count = 0;
            for (int i = 0; i < 13; ++i) {
                if ((int)Cards.player_cards[i] > 0) {
                    ++count;
                    if (count == cur_card) {
                        if ((int)Cards.opponent_cards[i] > 0) {
                            Cards.player_cards[i] += Cards.opponent_cards[i];
                            Cards.opponent_cards[i] = 0;
                            std::cout << "Yeah!" << std::endl;
                            break;
                        }
                        else {
                            Cards.give_player_card();
                            std::cout << "Nope!" << std::endl;
                            break;
                        }
                    }
                }
            }


            // тут выбирает противник
        }
    }

    void StartGame() {
        for (int i = 0; i < 7; ++i) {
            Cards.give_player_card();
            Cards.give_opponent_card();
        }

        cur_card = 0;
        count_of_cards = Cards.get_count_player_card();

        Loop();
    }

    void select_player_card() {
        std::cout << "Choose a card: ";
        std::cin >> cur_card;
    }

    void print_player_display() {
        for (int i = 0; i < 13; ++i) {
            if ((int)Cards.player_cards[i] > 0) {
                std::cout << Cards.deck_of_cards[i] << " ";
            }
        }
        std::cout << std::endl;
        for (int i = 0; i < 13; ++i) {
            if ((int)Cards.player_cards[i] > 0) {
                std::cout << (int)Cards.player_cards[i] << " ";
            }
        }
        std::cout << std::endl;
        //for (int i = 0; i < count_of_cards; ++i) {
        //    if (i == cur_card) std::cout << "^ ";
        //    else std::cout << "  ";
        //}
        //std::cout << std::endl;
    }
};

int main()
{
    srand(time(0));
    GameLoop Game;

    Game.StartGame();
}