#include <iostream>
#include <cstdlib>
#include <ctime>

class SetsOfCards {
private:
    signed char* deck_of_cards = new signed char[13] {'2', '3', '4', '5', '6', '7', '8', '9', '0', 'J', 'Q', 'K', 'A'};
    signed char* count_of_cards = new signed char[13] {4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4};
    signed char* player_cards = new signed char[13] {};
    signed char* opponent_cards = new signed char[13] {};

public:
    SetsOfCards() {

    }
    ~SetsOfCards() {
        delete[] deck_of_cards;
        delete[] count_of_cards;
    }

    void print() {
        for (int i = 0; i < 13; ++i) {
            std::cout << deck_of_cards[i] << " ";
        }
        std::cout << std::endl;
        for (int i = 0; i < 13; ++i) {
            std::cout << (int)count_of_cards[i] << " ";
        }
        std::cout << std::endl;
        for (int i = 0; i < 13; ++i) {
            std::cout << (int)player_cards[i] << " ";
        }
        std::cout << std::endl;
    }

    void get_player_card() {
        signed char new_card = rand() % 13;
        std::cout << (int)new_card << std::endl;
        player_cards[(int)new_card]++;
        count_of_cards[(int)new_card]--;
    }

    void get_opponent_card() {
        signed char new_card = rand() % 13;
        std::cout << (int)new_card << std::endl;
        opponent_cards[(int)new_card]++;
        count_of_cards[(int)new_card]--;
    }
};

class GameLoop {
public:
    SetsOfCards Cards;

    void Loop() {
        Cards.print();
    }

    void StartGame() {
        Cards.print();
        for (int i = 0; i < 7; ++i) {
            Cards.get_player_card();
            Cards.get_opponent_card();
        }
        Cards.print();
    }

};

int main()
{
    srand(time(0));
    GameLoop Game;

    Game.StartGame();
}