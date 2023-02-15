#!/usr/bin/env python3

# Author: Alambek Gulamidinov
# Student ID: 20192012
# EMail: alan.gulamidinov@gmail.com

import re
import sys

# Output a plan in the format that can be read by the validator.
# Input:
#   plan - the list of actions in the sas_plan file.

def print_solution(plan):
    for action in plan:
        s = re.sub(r"\(|\)", "", action).upper()
        if s[0] == ';':
            continue
        terms = s.split(" ")

        ###############################################################
        # TODO: Please write code to print the MATCH or DRAW actions. #
        ###############################################################


def print_usage():
    print("Usage:")
    print()
    print("      ", sys.argv[0], "sas_plan")
    print()

def main():
    if len(sys.argv) != 2:
        print_usage()
        exit(1)

    infile_name = sys.argv[1]

    plan = []
    with open(infile_name) as infile:
        for line in infile:
            s = line.strip()
            if s == '': continue
            plan.append(s)

    print_solution(plan)

if __name__ == "__main__":
    main()
