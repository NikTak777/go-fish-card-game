#include <iostream>
#include <cstdlib>
#include <ctime>
#include <conio.h>

extern "C" signed char choose_opponent_card(signed char* opponent_cards, signed char* eventual_cards, short count_of_check_opponent);

const std::string RESET = "\033[0m";
const std::string RED = "\033[31m";
const std::string GREEN = "\033[32m";
const std::string YELLOW = "\033[33m";
const std::string CYAN = "\033[36m";
const std::string MAGENTA = "\033[35m";

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


    int get_count_cards_in_deck() {
        int total = 0;
        for (int i = 0; i < 13; ++i) {
            total += count_of_cards[i];
        }
        return total;
    }

    void reset() {
        for (int i = 0; i < 13; ++i) {
            count_of_cards[i] = 4;
            player_cards[i] = 0;
            opponent_cards[i] = 0;
            eventual_cards[i] = 0;
        }
    }
};

class GameLoop {
public:
    SetsOfCards Cards;
    short cur_card;
    short count_of_player_cards;
    short count_of_player_sets;
    short count_of_opponent_sets;
    short count_of_check_opponent;
    bool is_victory;

    short total_player_win = 0;
    short total_opponent_win = 0;


    void Loop() {
        while (true) {
            // Cards.print();

            count_of_player_cards = Cards.get_count_player_card();

            if (Cards.get_count_player_card() == 0) {
                Cards.give_player_card();
                count_of_player_cards = Cards.get_count_player_card();
                std::cout << CYAN << "[!] INFO: You had no cards! You drew a card from the deck." << RESET << std::endl;
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
                            std::cout << GREEN << "[+] SUCCESS: The opponent has given you his set of " << Cards.deck_of_cards[i] << "." << RESET << std::endl;
                            check_collected_player_set(i);
                            break;
                        }
                        else {
                            ++Cards.eventual_cards[i];
                            int new_card_player = (int)Cards.give_player_card();
                            std::cout << MAGENTA << "[-] MISS: The opponent didn't have a card with the value " << Cards.deck_of_cards[i] << "! You take a card " << Cards.deck_of_cards[new_card_player] << " from the deck." << RESET << std::endl;
                            if (Cards.get_count_cards_in_deck() > 0) {
                                check_collected_player_set(new_card_player, true);
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
                Cards.give_opponent_card();
                std::cout << CYAN << "[!] INFO: The opponent had no cards! He drew a card from the deck." << RESET << std::endl;
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
                std::cout << RED << "[-] ALERT: You gave the opponent a set of " << Cards.deck_of_cards[choose_opponent] << RESET << std::endl;
                check_collected_opponent_set(choose_opponent);
                //++count_of_check_opponent;
            }
            else {
                Cards.eventual_cards[choose_opponent] = 0;
                std::cout << CYAN << "[?] THINKING: You didn't have a card with the value " << Cards.deck_of_cards[choose_opponent] <<"! The opponent takes a card from the deck." << RESET << std::endl;
                if (Cards.get_count_cards_in_deck() > 0) {
                    check_collected_opponent_set((int)Cards.give_opponent_card());
                }
                else check_collected_opponent_set(choose_opponent);
            }

            if (check_victory() == 0) {
                break;
            }
        }
    }

    int check_victory() {
        if (count_of_player_sets + count_of_opponent_sets == 13 or count_of_player_sets >= 7 or count_of_opponent_sets >= 7) {

            system("cls");

            if (count_of_player_sets > count_of_opponent_sets) {
                ++total_player_win;
                std::cout << GREEN
                    << "\n=========================================================\n"
                    << "                 [W] VICTORY: YOU WIN!                   \n"
                    << "=========================================================\n"
                    << "[=] ROUND STATS: You have " << count_of_player_sets << " sets. Opponent has " << count_of_opponent_sets << " sets.\n"
                    << "=========================================================\n"
                    << RESET;
            }
            else {
                ++total_opponent_win;
                std::cout << RED
                    << "\n=========================================================\n"
                    << "                 [L] DEFEAT: YOU LOSE!                   \n"
                    << "=========================================================\n"
                    << "[=] ROUND STATS: Opponent has " << count_of_opponent_sets << " sets. You have " << count_of_player_sets << " sets.\n"
                    << "=========================================================\n"
                    << RESET;
            }

            std::cout << YELLOW
                << "\n==================== GLOBAL SCORE =======================\n"
                << "       [*] YOU: " << total_player_win << " WINS   |   OPPONENT: " << total_opponent_win << " WINS      \n"
                << "=========================================================\n"
                << RESET;

            return 0;
        }
        return 1;
    }

    void check_collected_player_set(int player_choose_card, bool from_deck = false) {
        if ((int)Cards.player_cards[player_choose_card] == 4) {
            Cards.player_cards[player_choose_card] = 0;
            ++count_of_player_sets;
            if (!from_deck) Cards.eventual_cards[player_choose_card] = -1;
            std::cout << GREEN << "[★] SET COLLECTED: You have collected set of " << Cards.deck_of_cards[player_choose_card] << ". In total you have " << count_of_player_sets << " sets" << RESET << std::endl;
        }
    }

    void check_collected_opponent_set(int opponent_choose_card) {
        if ((int)Cards.opponent_cards[opponent_choose_card] == 4) {
            Cards.opponent_cards[opponent_choose_card] = 0;
            ++count_of_opponent_sets;
            Cards.eventual_cards[opponent_choose_card] = -1;
            std::cout << RED << "[☠] OPPONENT SET: The opponent have collected set. In total he has " << count_of_opponent_sets << " sets" << RESET << std::endl;
        }
    }

    bool EndGame() {
        std::cout << "Press Enter to play again or Esc to exit!" << std::endl;

        char key;
        while (true) {
            key = _getch();
            if (key == '\r') {
                system("cls");
                return true;
            }
            if (key == 27) {
                return false;
            }
        }
    }


    void StartGame() {
        Cards.reset();

        cur_card = 0;
        count_of_player_sets = 0;
        count_of_opponent_sets = 0;
        count_of_check_opponent = 0;
        is_victory = 0;

        for (int i = 0; i < 7; ++i) {
            Cards.give_player_card();
            Cards.give_opponent_card();
        }

        count_of_player_cards = Cards.get_count_player_card();

        for (int i = 0; i < 13; ++i) {
            check_collected_player_set(i);
            check_collected_opponent_set(i);
        }

        Loop();
    }

    void select_player_card() {
        cur_card = 1;
        char key;

        while (true) {
            std::cout << "\r" << YELLOW << "       ";

            for (int i = 1; i <= count_of_player_cards; ++i) {
                if (i == cur_card) std::cout << "  ^   ";
                else std::cout << "      ";
            }

            std::cout << "(A - left, D - right, Enter - select)" << RESET;

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
        std::cout << YELLOW << "\n================= YOUR HAND =================\n" << "       ";
        for (int i = 0; i < 13; ++i) {
            if ((int)Cards.player_cards[i] > 0) {
                std::cout << "+---+ ";
            }
        }
        std::cout << std::endl << "Value: ";

        for (int i = 0; i < 13; ++i) {
            if ((int)Cards.player_cards[i] > 0) {
                std::cout << "| " << Cards.deck_of_cards[i] << " | ";
            }
        }
        std::cout << std::endl << "Count: ";

        for (int i = 0; i < 13; ++i) {
            if ((int)Cards.player_cards[i] > 0) {
                std::cout << "| " << (int)Cards.player_cards[i] << " | ";
            }
        }
        std::cout << std::endl << "       ";

        for (int i = 0; i < 13; ++i) {
            if ((int)Cards.player_cards[i] > 0) {
                std::cout << "+---+ ";
            }
        }
        std::cout << RESET << std::endl;
    }
};

int main()
{
    srand(time(0));
    GameLoop Game;

    while (true) {
        Game.StartGame();

        if (!Game.EndGame()) {
            break;
        }
    }
}