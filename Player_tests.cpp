#include "Player.hpp"
#include "unit_test_framework.hpp"
#include <cassert>
#include <iostream>

using namespace std;

TEST(test_simple_player_get_name) {
    Player * alice = Player_factory("Alice", "Simple");
    ASSERT_EQUAL("Alice", alice->get_name());

    delete alice;
}

TEST(test_player_insertion) {
  // Create a Human player
  Player * human = Player_factory("NotRobot", "Human");

    // Print the player using the stream insertion operator
    ostringstream oss1;
    oss1 << * human;

    // Verify that the output is the player's name
    ASSERT_EQUAL(oss1.str(), "NotRobot");

    // Create a Simple player
    Player * alice = Player_factory("Alice", "Simple");

    // Print the player using the stream insertion operator
    ostringstream oss2;
    oss2 << *alice;
    ASSERT_EQUAL(oss2.str(), "Alice");

    // Clean up players that were created using Player_factory()
    delete human;
    delete alice;
}

TEST(test_human_player_get_name) {
    Player * james =  Player_factory("James", "Human");
    ASSERT_EQUAL("James", james->get_name());

    delete james;
}

TEST(test_simple_make_trump_round_1) {
    Player * james = Player_factory("James", "Simple");
    james->add_card(Card(JACK, HEARTS));
    james->add_card(Card(QUEEN, HEARTS));
    james->add_card(Card(NINE, SPADES));
    james->add_card(Card(TEN, CLUBS));
    james->add_card(Card(NINE, DIAMONDS));

    Suit order_up_suit = SPADES;
    bool order_up = james->make_trump(Card(ACE, HEARTS), true, 1, order_up_suit);

    assert(order_up);
    ASSERT_EQUAL(HEARTS, order_up_suit);

    delete james;
}

TEST(test_simple_make_trump_round_2) {
    Player * james = Player_factory("James", "Simple");
    james->add_card((Card(NINE, DIAMONDS)));
    james->add_card((Card(ACE, DIAMONDS)));
    james->add_card((Card(KING, SPADES)));
    james->add_card((Card(TEN, CLUBS)));
    james->add_card((Card(NINE, HEARTS)));

    Suit order_up_suit = SPADES;
    assert(james->make_trump(Card(TEN, HEARTS), true, 2, order_up_suit));
    ASSERT_EQUAL(DIAMONDS, order_up_suit);

    delete james;
}

TEST(test_human_make_trump) {
    Player * james = Player_factory("James", "Human");
    james->add_card((Card(NINE, DIAMONDS)));
    james->add_card((Card(ACE, DIAMONDS)));
    james->add_card((Card(KING, SPADES)));
    james->add_card((Card(TEN, CLUBS)));
    james->add_card((Card(NINE, HEARTS)));

    Suit order_up_suit = SPADES;
    istringstream in("Diamonds");
    cin.rdbuf(in.rdbuf());  //so cin can read from the istringstream

    assert(james->make_trump(Card(TEN, HEARTS), true, 2, order_up_suit));
    ASSERT_EQUAL(DIAMONDS, order_up_suit);

    delete james;
}

TEST(test_simple_add_and_discard) {
    Player *james = Player_factory("James", "Simple");

    james->add_card(Card(NINE, DIAMONDS));
    james->add_card(Card(ACE, DIAMONDS));
    james->add_card(Card(KING, SPADES));
    james->add_card(Card(TEN, CLUBS));
    james->add_card(Card(NINE, HEARTS));

    Card upcard(ACE, HEARTS);
    james->add_and_discard(upcard);
    // 9 of diamonds should've been discarded
    ASSERT_NOT_EQUAL(Card(NINE, DIAMONDS), james->lead_card(CLUBS));

    delete james;
}

TEST(test_human_add_and_discard) {
    Player *james = Player_factory("James", "Human");

    james->add_card(Card(NINE, DIAMONDS));
    james->add_card(Card(ACE, DIAMONDS));
    james->add_card(Card(KING, SPADES));
    james->add_card(Card(TEN, CLUBS));
    james->add_card(Card(NINE, HEARTS));

    istringstream input("-1");
    streambuf *old = cin.rdbuf(input.rdbuf());
    james->add_and_discard(Card(ACE, HEARTS));
    cin.rdbuf(old);
    //upcard was discarded
    ASSERT_NOT_EQUAL(Card(ACE, HEARTS), james->lead_card(SPADES));

    delete james;
}

TEST(test_simple_lead_card) {
    Player * james = Player_factory("james", "Simple");
    Player * jake = Player_factory("jake", "Simple");
    Player * jonno = Player_factory("i_love_you", "Simple");

    //james has normal hand, will lead highest non trump
    james->add_card(Card(NINE, DIAMONDS));
    james->add_card(Card(ACE, DIAMONDS));
    james->add_card(Card(KING, SPADES));
    james->add_card(Card(TEN, CLUBS));
    james->add_card(Card(NINE, HEARTS));

    //if trump is diamonds, should lead king of spades
    ASSERT_EQUAL(james->lead_card(DIAMONDS), Card(KING, SPADES));

    //jake only has trumps
    jake->add_card(Card(ACE, DIAMONDS));
    jake->add_card(Card(KING, DIAMONDS));
    jake->add_card(Card(QUEEN, DIAMONDS));
    jake->add_card(Card(NINE, DIAMONDS));
    jake->add_card(Card(TEN, DIAMONDS));

    //jake should lead with highest card in his hand
    ASSERT_EQUAL(jake->lead_card(DIAMONDS), Card(ACE, DIAMONDS));

    //jonno has right bower and left bower
    jonno->add_card(Card(JACK, DIAMONDS));
    jonno->add_card(Card(JACK, HEARTS));
    jonno->add_card(Card(ACE, HEARTS));
    jonno->add_card(Card(TWO, SPADES));
    jonno->add_card(Card(THREE, SPADES));

    //if trump is diamonds, jonno should lead with ace of hearts
    ASSERT_EQUAL(jonno->lead_card(DIAMONDS), Card(ACE, HEARTS));
}

TEST(test_human_lead_card) {
    Player *james = Player_factory("james", "Human");
    Player *jake = Player_factory("jake", "Human");
    Player *jonno = Player_factory("jonno", "Human");

    // james has non trumps, but human can choose any card
    james->add_card(Card(NINE, DIAMONDS));
    james->add_card(Card(ACE, DIAMONDS));
    james->add_card(Card(KING, SPADES));
    james->add_card(Card(TEN, CLUBS));
    james->add_card(Card(NINE, HEARTS));

    istringstream input1("3");
    streambuf *old = cin.rdbuf(input1.rdbuf());

    ASSERT_EQUAL(james->lead_card(DIAMONDS), Card(KING, SPADES));
    cin.rdbuf(old);

    // jake only has trump cards
    jake->add_card(Card(ACE, DIAMONDS));
    jake->add_card(Card(KING, DIAMONDS));
    jake->add_card(Card(QUEEN, DIAMONDS));
    jake->add_card(Card(NINE, DIAMONDS));
    jake->add_card(Card(TEN, DIAMONDS));

    istringstream input2("4");
    old = cin.rdbuf(input2.rdbuf());

    ASSERT_EQUAL(jake->lead_card(DIAMONDS), Card(ACE, DIAMONDS));
    cin.rdbuf(old);

    // jonno has bowers and non trumps
    jonno->add_card(Card(JACK, DIAMONDS));
    jonno->add_card(Card(JACK, HEARTS));
    jonno->add_card(Card(ACE, HEARTS));
    jonno->add_card(Card(TWO, SPADES));
    jonno->add_card(Card(THREE, SPADES));

    istringstream input3("2");
    old = cin.rdbuf(input3.rdbuf());

    ASSERT_EQUAL(jonno->lead_card(DIAMONDS), Card(JACK, HEARTS));
    cin.rdbuf(old);

    delete james;
    delete jake;
    delete jonno;
}

TEST(test_simple_play_card) {
    Player * james = Player_factory("james", "Simple");
    Player * jonno = Player_factory("goat", "Simple");

    //james can follow led suit (hearts)
    james->add_card(Card(ACE, HEARTS));
    james->add_card(Card(NINE, HEARTS));
    james->add_card(Card(TWO, SPADES));
    james->add_card(Card(FOUR, CLUBS));
    james->add_card(Card(ACE, DIAMONDS));

    //james should play ace of hearts
    ASSERT_EQUAL(james->play_card(Card(THREE, HEARTS), SPADES), Card(ACE, HEARTS));

    //jonno can't follow suit (hearts)
    jonno->add_card(Card(ACE, DIAMONDS));
    jonno->add_card(Card(KING, DIAMONDS));
    jonno->add_card(Card(QUEEN, DIAMONDS));
    jonno->add_card(Card(JACK, DIAMONDS));
    jonno->add_card(Card(NINE, DIAMONDS));

    //jonno should play nine of diamonds
    ASSERT_EQUAL(jonno->play_card(Card(THREE, HEARTS), SPADES), Card(NINE, DIAMONDS));
}

TEST(test_human_play_card) {
    Player *james = Player_factory("james", "Human");
    Player *jonno = Player_factory("jonno", "Human");
    Player *goat = Player_factory("goat", "Human");

    //james can follow led suit
    james->add_card(Card(ACE, HEARTS));
    james->add_card(Card(NINE, HEARTS));
    james->add_card(Card(TWO, SPADES));
    james->add_card(Card(FOUR, CLUBS));
    james->add_card(Card(ACE, DIAMONDS));

    //choose a heart
    istringstream input1("4");
    streambuf *old = cin.rdbuf(input1.rdbuf());

    ASSERT_EQUAL(james->play_card(Card(THREE, HEARTS), SPADES), Card(ACE, DIAMONDS));
    cin.rdbuf(old);

    //jonno cannot follow hearts
    jonno->add_card(Card(ACE, DIAMONDS));
    jonno->add_card(Card(KING, DIAMONDS));
    jonno->add_card(Card(QUEEN, DIAMONDS));
    jonno->add_card(Card(JACK, DIAMONDS));
    jonno->add_card(Card(NINE, DIAMONDS));

    // choose any card
    istringstream input2("0");
    old = cin.rdbuf(input2.rdbuf());

    Card result2 = jonno->play_card(Card(THREE, HEARTS), SPADES);
    cin.rdbuf(old);
    ASSERT_EQUAL(result2, Card(NINE, DIAMONDS));

    //left bower is trump
    goat->add_card(Card(JACK, HEARTS));
    goat->add_card(Card(ACE, HEARTS));
    goat->add_card(Card(KING, SPADES));
    goat->add_card(Card(NINE, CLUBS));
    goat->add_card(Card(TEN, DIAMONDS));

    //choose jack of hearts
    istringstream input3("2");
    old = cin.rdbuf(input3.rdbuf());

    Card result3 = goat->play_card(Card(THREE, DIAMONDS), DIAMONDS);
    cin.rdbuf(old);
    ASSERT_EQUAL(result3, Card(JACK, HEARTS));

    delete james;
    delete jonno;
    delete goat;
}

TEST_MAIN()
