#include "Player.hpp"
#include <cassert>
#include <algorithm>
using namespace std;
class SimplePlayer : public Player {
    public:
        //ctor
        SimplePlayer(string name)
        :name(name) {}

        const string & get_name() const override {
            return name;
        }

        void add_card(const Card &c) override {
            assert(hand.size() < MAX_HAND_SIZE);
            hand.push_back(c);
        }

        bool make_trump(const Card &upcard, bool is_dealer, int round, Suit &order_up_suit) const override {
            assert(round == 1 || round == 2);
            /*Round 1 Strategy: Order up if simple player will have >= 2 that are either face or ace of
            the potential trump suit (Trump suit A, K, Q, J, RBower, LBower)*/
            vector<Card> meetsCriteria;
            Suit upcard_suit = upcard.get_suit();
            if(round == 1) {
                for(int i = 0; i < hand.size(); ++i) {
                    if((hand[i].get_suit(upcard_suit) == upcard_suit && hand[i].is_face_or_ace())) {
                        meetsCriteria.push_back(hand[i]);
                    }
                }
                if(meetsCriteria.size() >= 2) {
                    meetsCriteria.clear();
                    order_up_suit = upcard_suit;
                    return true;
                }
                meetsCriteria.clear();
                return false;
            }
            /*Round 2 Strategy: Order up with same color suit as upcard (next suit or same suit). 
            Order up if simple player will have >= 1 that are either face or ace of the potential 
            trump suit (Trump suit A, K, Q, J, RBower, LBower)*/
            if(round == 2) {
                Suit order_suit = Suit_next(upcard_suit);
                if(is_dealer) {
                    order_up_suit = Suit_next(upcard_suit);
                    return true;
                }
                for(int i = 0; i != hand.size(); ++i) {
                    if(hand[i].get_suit(order_suit) == order_suit && hand[i].is_face_or_ace()) {
                        meetsCriteria.push_back(hand[i]);
                    }
                }
                if(meetsCriteria.size() >= 1) {
                    meetsCriteria.clear();
                    order_up_suit = order_suit;
                    return true;
                }
                meetsCriteria.clear();
                return false;
            }
            return false;
        }

        void add_and_discard(const Card &upcard) override {
            assert(hand.size() >= 1);
            //pick up upcard
            hand.push_back(upcard);
            Suit trump = upcard.get_suit();
            Card min = hand[0];
            int min_i = 0;
            //find min card in hand
            for(int i = 0; i != hand.size(); ++i) {
                //must consider trump, which is upcard suit now
                if(Card_less(hand[i], min, trump)) {
                    min = hand[i];
                    min_i = i;
                }
            }
            hand.erase(hand.begin() + min_i);
        }

        //play highest non-trump card in hand. if all trumps, play highest
        Card lead_card(Suit trump) override {
            //determine whether we have non-trumps
            int non_trump_counter = 0;
            for(int i = 0; i != hand.size(); ++i) {
                if(!hand[i].is_trump(trump)) {
                    ++non_trump_counter;
                }
            }
            //If we have non-trumps, play highest non-trump
            if(non_trump_counter != 0) {
                Card lead_card;
                int first_i = 0;
                //find first non-trump card, will be used for comparison
                for(int i = 0; i != hand.size(); ++i) {
                    if(!hand[i].is_trump(trump)) {
                        lead_card = hand[i];
                        first_i = i;
                        break;
                    }
                }
                int lead_i = first_i;
                for(int i = first_i; i != hand.size(); ++i) {
                    if(Card_less(lead_card, hand[i], trump) && !hand[i].is_trump(trump)) {
                        lead_card = hand[i];
                        lead_i = i;

                    }
                }
                hand.erase(hand.begin() + lead_i);
                return lead_card;
            }
            //if we only have trump cards, play highest
            int lead_i = 0;
            Card lead_card = hand[0];
            for(int i = 0; i != hand.size(); ++i) {
                if(Card_less(lead_card, hand[i], trump)) {
                    lead_card = hand[i];
                    lead_i = i;
                }
            }
            hand.erase(hand.begin() + lead_i);
            return lead_card;
        }

        //if simple player can follow suit, play highest. if we can't, play lowest
        Card play_card(const Card &led_card, Suit trump) override {
            //check if simple player can follow suit
            int follow_suit_counter = 0;
            Suit led_card_suit = led_card.get_suit(trump);
            for(int i = 0; i != hand.size(); ++i) {
                if(hand[i].get_suit(trump) == led_card_suit) {
                    ++follow_suit_counter;
                }
            }
            //if we can, play highest card of led card suit
            if(follow_suit_counter != 0) {
                //find first card that can follow suit for comparison
                Card play_card;
                int play_card_i = 0;
                for(int i = 0; i != hand.size(); ++i) {
                    if(hand[i].get_suit(trump) == led_card_suit) {
                        play_card = hand[i];
                        play_card_i = i;
                        break;
                    }
                }
                for(int i = play_card_i; i < hand.size(); ++i) {
                    if(hand[i].get_suit(trump) == led_card_suit && Card_less(play_card, hand[i], trump)) {
                        play_card = hand[i];
                        play_card_i = i;
                    }
                }
                hand.erase(hand.begin() + play_card_i);
                return play_card;
            }
            //if we have no cards that are same suit as led card, play lowest
            Card min_card = hand[0];
            int min_card_i = 0;
            for(int i =0; i != hand.size(); ++i) {
                if(Card_less(hand[i], min_card, trump)) {
                    min_card = hand[i];
                    min_card_i = i;
                }
            }
            hand.erase(hand.begin() + min_card_i);
            return min_card;
        }
    private:
        string name;
        vector<Card> hand;
};
//end SimplePlayer class

class HumanPlayer : public Player {
    public:
        //ctor
        HumanPlayer(string name)
        :name(name) {}

        const string & get_name() const override {
            return  name;
        }

        void add_card(const Card &c) override {
            assert(hand.size() < MAX_HAND_SIZE);
            hand.push_back(c);
        }

        //private helper function to print hand
        void print_hand() const {
            for(size_t i = 0; i != hand.size(); ++i) {
                cout << "Human player " << name << "'s hand:"\
                << "[" << i << "] " << hand[i] << "\n";
            }
        }

        //private helper function to sort hand
        void sort_hand() {
            sort(hand.begin(), hand.end());
        }

        bool make_trump(const Card &upcard, bool is_dealer, int round, Suit &order_up_suit) const override {
            assert(round == 1 || round == 2);
            if(round == 1) {
                print_hand();
                //prompt user
                cout << "Human player " << name << ", please enter a suit, or \"pass\":\n";
                //read in decision
                string decision;
                cin >> decision;
                //if they order up, get suit of their decision
                if(decision != "pass") {
                    order_up_suit = string_to_suit(decision);
                    return true;
                } else {
                    return false;
                }
            }
            if(round == 2) {
                //screw the dealer
                if(is_dealer) {
                    //if we are here, use same color suit
                    order_up_suit = Suit_next(upcard.get_suit());
                    return true;
                }
                print_hand();
                //prompt user
                cout << "Human player " << name << ", please enter a suit, or \"pass\":\n";
                //read in decision
                string decision;
                cin >> decision;
                //if they order up, get suit of their decision
                if(decision != "pass") {
                    order_up_suit = string_to_suit(decision);
                    return true;
                } else {
                    return false;
                }
            }
            return false;
        }

        void add_and_discard(const Card &upcard) override {
            //pick up upcard
            hand.push_back(upcard);
            sort_hand();
            print_hand();
            //prompt user
            cout << "Discard upcard [-1]\n";
            cout << "Human player " << name << ", please select a card:\n";
            int decision;
            cin >> decision;
            if(decision == -1) {
                //find upcard
                for(int i = 0; i != hand.size(); ++i) {
                    if(hand[i] == upcard) {
                        hand.erase(hand.begin() + i);
                        break;
                    }
                }
            } else {
                hand.erase(hand.begin() + decision);
            }
        }

        Card lead_card(Suit trump) override {
            sort_hand();
            print_hand();
            //prompt user
            cout << "Human player " << name << ", please select a card:\n";
            //read in decision
            int decision;
            cin >> decision;
            //get card they wish to lead
            Card lead_card = hand[decision];
            //remove it
            hand.erase(hand.begin() + decision);
            return lead_card;
        }

        Card play_card(const Card &led_card, Suit trump) override {
            sort_hand();
            print_hand();
            //prompt user
            cout << "Human player " << name << ", please select a card:\n";
            //read in decision
            int decision;
            cin >> decision;
            //get card they wish to play
            Card play_card = hand[decision];
            //remove it
            hand.erase(hand.begin() + decision);
            return play_card;
        }
        
    private:
        string name;
        vector<Card> hand;
};

Player * Player_factory(const string &name, const string &strategy) {
    if(strategy == "Simple") {
        return new SimplePlayer(name);
    }
    if(strategy == "Human") {
        return new HumanPlayer(name);
    }
    assert(false);
    return nullptr;
}

ostream & operator<<(ostream &os, const Player &p) {
    os << p.get_name();
    return os;
}


