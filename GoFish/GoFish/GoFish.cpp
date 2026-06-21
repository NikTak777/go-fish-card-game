#include <iostream>
#include <cstdlib>
#include <ctime>
#include <conio.h>

extern "C" signed char choose_opponent_card(signed char* opponent_cards, signed char* eventual_cards, short count_of_check_opponent);

class SetsOfCards {
public:
    signed char* deck_of_cards = new signed char[13] {'2', '3', '4', '5', '6', '7', '8', '9', '0', 'J', 'Q', 'K', 'A'};
    signed char* count_of_cards = new signed char[13] {4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4};
    signed char* player_cards = new signed char[13] {};
    signed char* opponent_cards = new signed char[13] {};
    signed char* eventual_cards = new signed char[13] {};

    ~SetsOfCards() {
        delete[] deck_of_cards;
        delete[] count_of_cards;
        delete[] player_cards;
        delete[] opponent_cards;
        delete[] eventual_cards;
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
        std::cout << std::endl << "Opponent's ev cards:\t";
        for (int i = 0; i < 13; ++i) {
            std::cout << (int)eventual_cards[i] << " ";
        }
        std::cout << std::endl;
    }

    signed char give_player_card() {
        signed char new_card;
        while (true) {
            new_card = rand() % 13;
            if (count_of_cards[(int)new_card] != 0) {
                player_cards[(int)new_card]++;
                count_of_cards[(int)new_card]--;
                break;
            }
        }
        return new_card;
    }

    signed char give_opponent_card() {
        signed char new_card;
        while (true) {
            new_card = rand() % 13;
            if (count_of_cards[(int)new_card] != 0) {
                opponent_cards[(int)new_card]++;
                count_of_cards[(int)new_card]--;
                break;
            }
        }
        return new_card;
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

    signed char get_count_opponent_card() {
        signed char count = 0;
        for (int i = 0; i < 13; ++i) {
            if ((int)opponent_cards[i] > 0) {
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
    short count_of_player_cards;
    short count_of_cards;
    short count_of_player_sets;
    short count_of_opponent_sets;
    short count_of_check_opponent;
    bool is_victory;


    void Loop() {
        while (true) {
            // Cards.print();

            count_of_player_cards = Cards.get_count_player_card();

            if (Cards.get_count_player_card() == 0) {
                Cards.give_player_card();
                count_of_player_cards = Cards.get_count_player_card();
            }

            print_player_display();

            select_player_card();

            system("cls");

            short count = 0;
            for (int i = 0; i < 13; ++i) {
                if ((int)Cards.player_cards[i] > 0) {
                    ++count;
                    if (count == cur_card) {
                        if ((int)Cards.opponent_cards[i] > 0) {
                            Cards.player_cards[i] += Cards.opponent_cards[i];
                            Cards.opponent_cards[i] = 0;
                            Cards.eventual_cards[i] = 9;
                            std::cout << "The opponent has given you his set of " << Cards.deck_of_cards[i] << std::endl;
                            check_collected_player_set(i);
                            break;
                        }
                        else {
                            ++Cards.eventual_cards[i];
                            int new_card_player = (int)Cards.give_player_card();
                            std::cout << "The opponent didn't have a card with the value " << Cards.deck_of_cards[i] << "! You take a card " << Cards.deck_of_cards[new_card_player] << " from the deck." << std::endl;
                            if (count_of_cards != 0) {
                                check_collected_player_set(new_card_player, true);
                                --count_of_cards;
                            }
                            else {
                                check_collected_player_set(new_card_player, true);
                            }
                            break;
                            
                        }
                    }
                }
            }

            if (check_victory() == 0) {
                break;
            }

            if (Cards.get_count_opponent_card() == 0) {
                Cards.give_player_card();
            }

            if (count_of_check_opponent >= Cards.get_count_opponent_card() or count_of_check_opponent >= 5) {
                count_of_check_opponent = 0;
            }

            // Cards.print();

            // std::cout << "Oponnet randomize: " << count_of_check_opponent << std::endl;
            // тут выбирает противник
            short choose_opponent = (short)choose_opponent_card(Cards.opponent_cards, Cards.eventual_cards, count_of_check_opponent);
            // std::cout << "Oponnet choose: " << Cards.deck_of_cards[choose_opponent] << std::endl;
            ++count_of_check_opponent;

            if ((int)Cards.player_cards[choose_opponent] > 0) {
                Cards.opponent_cards[choose_opponent] += Cards.player_cards[choose_opponent];
                Cards.player_cards[choose_opponent] = 0;
                Cards.eventual_cards[choose_opponent] = 0;
                std::cout << "You gave the opponent a set of " << Cards.deck_of_cards[choose_opponent] << std::endl;
                check_collected_opponent_set(choose_opponent);
                //++count_of_check_opponent;
            }
            else {
                Cards.eventual_cards[choose_opponent] = 0;
                std::cout << "You didn't have a card with the value " << Cards.deck_of_cards[choose_opponent] <<"! The opponent takes a card from the deck." << std::endl;
                if (count_of_cards != 0) {
                    check_collected_opponent_set((int)Cards.give_opponent_card());
                    --count_of_cards;
                }
                else check_collected_opponent_set(choose_opponent);
            }

            if (check_victory() == 0) {
                break;
            }
        }
        system("pause");
    }

    int check_victory() {
        if (count_of_player_sets + count_of_opponent_sets == 13 or count_of_player_sets >= 7 or count_of_opponent_sets >= 7) {
            if (count_of_player_sets > count_of_opponent_sets) {
                std::cout << "Game over! You win!" << std::endl;
            }
            else {
                std::cout << "Game over! You lose!" << std::endl;
            }
            return 0;
        }
        return 1;
    }

    void check_collected_player_set(int player_choose_card, bool from_deck = false) {
        if ((int)Cards.player_cards[player_choose_card] == 4) {
            Cards.player_cards[player_choose_card] = 0;
            ++count_of_player_sets;
            if (!from_deck) Cards.eventual_cards[player_choose_card] = -1;
            std::cout << "You have collected set of " << Cards.deck_of_cards[player_choose_card] << ". In total you have " << count_of_player_sets << " sets" << std::endl;
        }
    }

    void check_collected_opponent_set(int opponent_choose_card) {
        if ((int)Cards.opponent_cards[opponent_choose_card] == 4) {
            Cards.opponent_cards[opponent_choose_card] = 0;
            ++count_of_opponent_sets;
            Cards.eventual_cards[opponent_choose_card] = -1;
            std::cout << "The opponent have collected set. In total he has " << count_of_opponent_sets << " sets" << std::endl;
        }
    }


    void StartGame() {
        for (int i = 0; i < 7; ++i) {
            Cards.give_player_card();
            Cards.give_opponent_card();
        }

        cur_card = 0;
        count_of_cards = 52;
        count_of_player_cards = Cards.get_count_player_card();
        count_of_player_sets = 0;
        count_of_opponent_sets = 0;
        count_of_check_opponent = 0;
        is_victory = 0;

        Loop();
    }

    void select_player_card() {
        cur_card = 1;
        char key;

        while (true) {
            std::cout << "\r";

            for (int i = 1; i <= count_of_player_cards; ++i) {
                if (i == cur_card) std::cout << "^ ";
                else std::cout << "  ";
            }

            std::cout << "(A - left, D - right, Enter - select)";

            key = _getch();

            if (key == 'a' || key == 'A') {
                if (cur_card > 1) cur_card--;
            }
            else if (key == 'd' || key == 'D') {
                if (cur_card < count_of_player_cards) cur_card++;
            }
            else if (key == '\r') {
                std::cout << std::endl;
                break;
            }
        }
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