#!/usr/bin/env python3

# Author: Tsz-Chiu Au
# Copyright (c) 2014 Tsz-Chiu Au. All rights reserved.

import re
import sys
import random

all_cards = [
    ("SA", "HA", "CA", "DA"),
    ("S2", "H2", "C2", "D2"),
    ("S3", "H3", "C3", "D3"),
    ("S4", "H4", "C4", "D4"),
    ("S5", "H5", "C5", "D5"),
    ("S6", "H6", "C6", "D6"),
    ("S7", "H7", "C7", "D7"),
    ("S8", "H8", "C8", "D8"),
    ("S9", "H9", "C9", "D9"),
    ("SX", "HX", "CX", "DX"),
    ("SJ", "HJ", "CJ", "DJ"),
    ("SQ", "HQ", "CQ", "DQ"),
    ("SK", "HK", "CK", "DK"),
]


def show(pyramid, deck):
    n = len(pyramid)
    for i in range(0,n):
        for j in range(0,n-i-1):
            print("  ", end="")
        print("  ".join(pyramid[i]))
    print()
    for card in reversed(deck):
        print(card)


def is_king(card):
    return card[-1:] == 'K'


def find_empty_pos(pyramid):
    result = []
    for y in range(0, 7):
        for x in range(0, y+1):
            if pyramid[y][x] == "__":
                if y == 0:
                    result.append((x, y))
                else:
                    left_okay = (x == 0) or (pyramid[y-1][x-1] != "__")
                    right_okay = (x > y-1) or (pyramid[y-1][x] != "__")
                    if left_okay and right_okay:
                        result.append((x, y))
    return result


def randomize_cards(pyramid_size, deck_size):

    # compute random pairs

    pair1 = []
    pair2 = [] 
    for i in range(0,12):
        if random.randint(0,1) == 0:
            if random.randint(0,1) == 0:
                pair1.append((all_cards[i][0], all_cards[11-i][1]))
                pair2.append((all_cards[i][2], all_cards[11-i][3]))
            else:
                pair1.append((all_cards[11-i][1], all_cards[i][0]))
                pair2.append((all_cards[11-i][3], all_cards[i][2]))
        else:
            if random.randint(0,1) == 0:
                pair1.append((all_cards[i][0], all_cards[11-i][3]))
                pair2.append((all_cards[i][2], all_cards[11-i][1]))
            else:
                pair1.append((all_cards[11-i][3], all_cards[i][0]))
                pair2.append((all_cards[11-i][1], all_cards[i][2]))
    pair3 = [ all_cards[12][0], all_cards[12][1], all_cards[12][2], all_cards[12][3] ]
    pairs = pair1 + pair2 + pair3
    random.shuffle(pairs)


    # directly build the pyramid and the deck

    pyramid = [ [ "__" for j in range(0,i) ] for i in range(1,8) ]
    deck = []
    count = 0
    while count < pyramid_size:
        if pairs:
            p = pairs.pop()
        else:
            # all pairs are enumerated, recycle the kings
            for card in deck:
                if is_king(card):
                    deck.remove(card)
                    p = card
                    break
            
        if type(p) is str:
            if random.random() < 0.8:
                (x,y) = random.choice(find_empty_pos(pyramid))
                pyramid[y][x] = p
                count += 1
            else:
                deck.append(p)
        else:
            empty_pos = find_empty_pos(pyramid)
            (x,y) = random.choice(empty_pos)
            pyramid[y][x] = p[0]
            count += 1
            if count < pyramid_size and len(empty_pos) > 1 and random.random() < 0.2:
                (x,y) = random.choice(empty_pos)
                if pyramid[y][x] == "__":
                    pyramid[y][x] = p[1]
                    count += 1
                else:
                    deck.append(p[1])
            else:
                deck.append(p[1])

    # if deck is too small, increase its size

    remaining_cards = []
    for p in pairs:
        if type(p) is str:
            remaining_cards.append(p)
        else:
            remaining_cards.append(p[0])
            remaining_cards.append(p[1])
    random.shuffle(remaining_cards)

    while len(deck) < deck_size:
        deck.insert(random.randint(0,len(deck)), remaining_cards.pop())

    # restruct the deck, to force recycling
    # using three imtermediate deck because we allow cycling three times

    deck0 = []
    deck1 = []
    deck2 = []

    n = len(deck)
    for x in range(0,n):
        i = random.randint(0,2)
        if i == 0:
            deck0.append(deck.pop())
        elif i == 1:
            deck1.append(deck.pop())
        else:  # i == 2
            deck2.append(deck.pop())

    while deck0 or deck1 or deck2:
        i = random.randint(0,2)
        if i == 0:
            if deck0: deck.append(deck0.pop())
        elif i == 1:
            if deck1: deck.append(deck1.pop())
        else:  # i == 2
            if deck2: deck.append(deck2.pop())

    # return the solution

    return (pyramid, deck)


def print_usage():
    print("Usage:")
    print()
    print("      ", sys.argv[0], "m n")
    print()
    print("where m is the size of the pyramid and (1 <= m <= 28)")
    print("      n is the minimum size of the deck (m + n <= 52)")
    print()


def main():
    if len(sys.argv) == 3:
        pyramid_size = int(sys.argv[1])
        deck_size = int(sys.argv[2])
    else:
        print_usage()
        exit(1)

    if pyramid_size < 1:
        print("Error: pyramid is too small.")
        exit(1)

    if pyramid_size > 28:
        print("Error: pyramid is too large.")
        exit(1)

    if pyramid_size + deck_size > 52:
        print("Error: too many cards.")
        exit(1)


    (pyramid, deck) = randomize_cards(pyramid_size, deck_size)
    show(pyramid, deck)


if __name__ == "__main__":
    main()
