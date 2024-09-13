#include <cstdio>

#define STL_ALIASES
#define STL_HELPERS
#include "Types.h"

#include <array>
#include <random>

enum class Suit {
    Clubs,
    Diamonds,
    Hearts,
    Spades,
    Last = Spades,
};

const Map<Suit, str> SuitNames = {
  {Suit::Clubs, "Clubs"},
  {Suit::Diamonds, "Diamonds"},
  {Suit::Hearts, "Hearts"},
  {Suit::Spades, "Spades"},
};

const Map<i32, str> RankNames = {
  {1, "Ace"},
  {2, "2"},
  {3, "3"},
  {4, "4"},
  {5, "5"},
  {6, "6"},
  {7, "7"},
  {8, "8"},
  {9, "9"},
  {10, "10"},
  {11, "Jack"},
  {12, "Queen"},
  {13, "King"},
};

enum class Rank {
    Two = 2,
    Three,
    Four,
    Five,
    Six,
    Seven,
    Eight,
    Nine,
    Ten,
    Jack,
    Queen,
    King,
    Ace
};

enum class HandRank {
    HighCard,
    OnePair,
    TwoPair,
    ThreeOfAKind,
    Straight,
    Flush,
    FullHouse,
    FourOfAKind,
    StraightFlush,
    RoyalFlush
};

struct Card {
    Suit suit;
    Rank rank;

    bool operator<(const Card& other) const {
        if (suit != other.suit)
            return suit < other.suit;
        return rank < other.rank;
    }

    [[nodiscard]] str Name() const {
        return RankNames.at((i32)rank) + " of " + SuitNames.at(suit);
    }
};

struct Hand {
    std::array<Card, 2> cards;
};

class Deck {
public:
    std::deque<Card> cards;
    std::array<Card, 5> communityCards;

    Deck() : communityCards({}) {
        for (const Suit suit : {Suit::Clubs, Suit::Diamonds, Suit::Hearts, Suit::Spades}) {
            for (i32 rank = 1; rank <= 13; ++rank) {
                cards.push_back({suit, (Rank)rank});
            }
        }
    }

    void Shuffle(i32 iterations = 7) {
        for (i32 i = 0; i < iterations; ++i) {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::ranges::shuffle(cards, gen);
        }
    }

    void Print() const {
        for (const Card& card : cards) {
            printf("%s\n", card.Name().c_str());
        }
    }

    Hand DealHand() {
        if (cards.size() < 2) {
            throw std::out_of_range("Not enough cards to deal a hand");
        }

        Hand deal = {};

        deal.cards[0] = cards.front();
        cards.pop_front();

        deal.cards[1] = cards.front();
        cards.pop_front();

        return deal;
    }

    void Flop() {
        if (cards.size() < 3) {
            throw std::out_of_range("Not enough cards to deal a hand");
        }

        for (auto i = 0; i < 3; ++i) {
            communityCards[i] = cards.front();
            cards.pop_front();
        }
    }

    void Turn() {
        if (cards.empty()) {
            throw std::out_of_range("Not enough cards to deal a hand");
        }

        communityCards[3] = cards.front();
        cards.pop_front();
    }

    void River() {
        if (cards.empty()) {
            throw std::out_of_range("Not enough cards to deal a hand");
        }

        communityCards[4] = cards.front();
        cards.pop_front();
    }
};

int main() {
    Deck deck;
    deck.Shuffle();

    Hand playerHand = deck.DealHand();

    deck.Flop();
    deck.Turn();
    deck.River();

    // Analyze player's hand
    {
        auto community = deck.communityCards;
        auto hand      = playerHand.cards;
        std::vector<Card> temp;

        // Insert elements from both arrays
        temp.insert(temp.end(), community.begin(), community.end());
        temp.insert(temp.end(), hand.begin(), hand.end());

        // Sort the combined vector
        std::sort(temp.begin(), temp.end());

        // Copy sorted elements to std::array
        std::array<Card, 7> inPlay = {};
        std::ranges::copy(temp, inPlay.begin());
    }

    return 0;
}
