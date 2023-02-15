#!/usr/bin/env python3

# Author: Alambek Gulamidinov
# Student ID: 20192012
# EMail: alan.gulamidinov@gmail.com

import re
import sys

# Output the PDDL problem file from the problem specification file
# Input:
#   problem_name - the name of the problem
#   pyramid - the cards in the pyramid. It is a double list with a structure:
#             [ [card00], [card10, card11], [card20, card21, card22], ...  ]
#             where card00 is the card the top of the pyramid; card10 and card11 are the cards
#             on the second rows of the pyramid; etc.  A card is a string with two characters.
#             A card "__" means that the position is empty.
#   deck - the list of cards on the deck (deck[0] is the top card, deck[-1] is the bottom card)
def print_pddl(problem_name, pyramid, deck):
    print("(define (problem " + problem_name + ")")

    ##################################################################
    # TODO: Please write code to print the content of the PDDL file. #
    ##################################################################

    print(")")

def is_any_duplicate_cards(all_cards):
    for i, card1 in enumerate(all_cards):
        if card1 != "__":
            for card2 in all_cards[i+1:]:
                if card2 != "__":
                    if card1 == card2:
                        return card1
    return ""

def print_usage():
    print("Usage:")
    print()
    print("      ", sys.argv[0], "problem_spec.txt")
    print()

def main():
    if len(sys.argv) != 2:
        print_usage()
        exit(1)

    infile_name = sys.argv[1]
    problem_name = re.sub(r"\.[^\.]+", "", re.sub(r".*\/", "", infile_name))

    pyramid = []
    deck = []
   
    with open(infile_name) as infile:
        for line in infile:
            s = line.strip().upper()
            if s == '': break
            pyramid.append(re.split(" +", s))
        for line in infile:
            s = line.strip().upper()
            if s == '': continue
            deck.extend(re.split(" +", s))

    all_cards = [ x for ls in pyramid for x in ls ] + deck
    r = is_any_duplicate_cards(all_cards)
    if r:
        print("Error: " + r + " is duplicated.")
        return
    print_pddl(problem_name, pyramid, deck)

if __name__ == "__main__":
    main()

