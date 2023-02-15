#!/usr/bin/env python3

# Author: Tsz-Chiu Au
# Copyright (c) 2022 Tsz-Chiu Au. All rights reserved.


import re
import sys

def is_any_duplicate_cards(all_cards):
    for i, card1 in enumerate(all_cards):
        if card1 != "__":
            for card2 in all_cards[i+1:]:
                if card2 != "__":
                    if card1 == card2:
                        return card1
    return ""

card_num = {
  'A' : 1,
  '2' : 2,
  '3' : 3,
  '4' : 4,
  '5' : 5,
  '6' : 6,
  '7' : 7,
  '8' : 8,
  '9' : 9,
  'X' : 10,
  'J' : 11,
  'Q' : 12,
  'K' : 13,
}

def sum_to_13(card1, card2):
    return card_num[card1[-1:]] + card_num[card2[-1:]] == 13

def is_king(card):
    return card[-1:] == 'K'


class State:
    pyramid = []
    deck = []
    waste_pile = []
    position = {}
    cycle_num = 1

    def __init__(self, problem_filename):
        with open(problem_filename) as infile:
            y = 0
            for line in infile:
                s = line.strip().upper()
                if s == '': break
                cards = re.split(" +", s)
                self.pyramid.append(cards)
                for x, card in enumerate(cards):
                    self.position[card] = (x, y)
                y += 1
            for line in infile:
                s = line.strip().upper()
                if s == '': continue
                self.deck.extend(re.split(" +", s))
            self.deck.reverse()
        all_cards = [ x for ls in self.pyramid for x in ls ] + self.deck
        r = is_any_duplicate_cards(all_cards)
        if r:
            print("Error: " + r + " is duplicated.")
            return

    def is_on_top_of_pyramid(self, card):
        if card in self.position:               # is originally in pyramid?
            (x,y) = self.position[card]
            if self.pyramid[y][x] != "__":      # has not been removed?
                if y == len(self.pyramid)-1:    # at the top row?
                    return True
                else:
                    return self.pyramid[y+1][x] == "__" and self.pyramid[y+1][x+1] == "__"
            else:
                return False
        else:
            return False

    def is_on_deck(self, card):
        if self.deck:
            return self.deck[-1] == card
        else:
            return False

    def is_on_waste_pile(self, card):
        if self.waste_pile:
            return self.waste_pile[-1] == card
        else:
            return False

    def is_pyramid_empty(self):
        return self.pyramid[0][0] == "__"

    def remove_card(self, card):
        if self.is_on_top_of_pyramid(card):
            (x,y) = self.position[card]
            self.pyramid[y][x] = "__"
            return True
        elif self.is_on_deck(card):
            self.deck.pop()
            return True
        elif self.is_on_waste_pile(card):
            self.waste_pile.pop()
            return True
        else:
            return False

    def match(self, card1, card2):
        if not sum_to_13(card1, card2):
            return False
        return self.remove_card(card1) and self.remove_card(card2)

    def match_king(self, card):
        if is_king(card):
            return self.remove_card(card)
        else:
            return False

    def draw(self):
        if self.deck:
            self.waste_pile.append(self.deck.pop())
            return True
        else:
            if self.waste_pile:
                self.cycle_num += 1
                if self.cycle_num > 3:
                    return False
                self.deck = self.waste_pile
                self.deck.reverse()
                self.waste_pile = []
                return True
            else:
                return False

    def print(self):
        n = len(self.pyramid)
        for i in range(0,n):
            for j in range(0,n-i-1):
                print("  ", end="")
            print("  ".join(self.pyramid[i]))
        print()
        print("Deck: " + " ".join(self.deck))
        print("Waste Pile: " + " ".join(self.waste_pile))
        print()


def print_usage():
    print("Usage:")
    print()
    print("      ", sys.argv[0], "[-v]", "problem.dat", "solution.txt")
    print()


def main():
    if len(sys.argv) == 3:
        problem_filename = sys.argv[1]
        solution_filename = sys.argv[2]
        is_verbose = False
    elif len(sys.argv) == 4 and sys.argv[1] == "-v":
        problem_filename = sys.argv[2]
        solution_filename = sys.argv[3]
        is_verbose = True
    else:
        print_usage()
        exit(1)


    pyramid = []
    deck = []
    waste_pile = []

    state = State(problem_filename)
    if is_verbose: state.print()

    with open(solution_filename) as infile:
        action_no = 1
        for line in infile:
            s = line.strip().upper()
            if s[0] == ';':
                continue
            print("Action " + str(action_no) + ": " + s)
            if is_verbose: print()
            terms = s.split(' ')
            if terms[0] == "MATCH":
                if len(terms) == 2:
                    card = terms[1]
                    if state.match_king(card):
                        if is_verbose: state.print()
                    else:
                        print("Error: cannot match " + card)
                        return
                else:
                    card1 = terms[1]
                    card2 = terms[2]
                    if state.match(card1, card2):
                        if is_verbose: state.print()
                    else:
                        print("Error: cannot match " + card1 + " and " + card2)
                        return
            elif terms[0] == "DRAW":
                if state.draw():
                   if is_verbose: state.print()
                else:
                    print("Error: cannot draw")
                    return
            action_no += 1

    if state.is_pyramid_empty():
        print("Solution is valid.")
    else:
        print("Solution is invalid.")


if __name__ == "__main__":
    main()


