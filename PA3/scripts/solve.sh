#!/bin/bash


# Please update the following five constants to point to the corresponding files/directories

DOMAIN_FILE="domain-pyramid.pddl"
PLANNER_DIRECTORY="../downward"
CONVERT_PROBLEM_TO_PDDL="convert-problem-to-pddl.py"
CONVERT_PLAN_TO_SOLUTION="convert-plan-to-solution.py"
VALIDATE_SOLUTION="validate-solution.py"


usage() {
  echo "Usage: $0 [-t] problem-spec.dat" 1>&2
  echo "where -t option keep the temporary directory after a successful execution." 1>&2
  exit 1
}


# SEARCH_OPTION=("--alias seq-sat-lama-2011")
SEARCH_OPTION=("--alias lama-first")

keep_temp_dir=no

while getopts "tv" opt; do
    case "${opt}" in
        t) keep_temp_dir=yes;;
        *) usage ;;
    esac
done
shift $((OPTIND-1))

if (( $# != 1 )); then
  usage
fi

SPEC_FILE=$1
PROBLEM_NAME=${SPEC_FILE/\.dat/}
PDDL_FILE=${PROBLEM_NAME}.pddl
SOL_FILE=${PROBLEM_NAME}-solution.txt
TMP_DIR=tmp-${PROBLEM_NAME}

if [[ ! -f $SPEC_FILE ]] ; then
  echo "Error: specification file does not exist" 1>&2
  exit 1
fi

if [[ $SPEC_FILE == $TMP_DIR ]] ; then
  echo "Error: specification file must end with .dat" 1>&2
  exit 1
fi

if [[ -f $SOL_FILE ]] ; then
    rm $SOL_FILE
fi

if [[ -d $TMP_DIR ]] ; then
  echo "Deleting temporary directory: $TMP_DIR"
  rm -R $TMP_DIR
fi

echo "Creating temporary directory: $TMP_DIR"
mkdir $TMP_DIR

cd $TMP_DIR

../$CONVERT_PROBLEM_TO_PDDL ../$SPEC_FILE > $PDDL_FILE
../$PLANNER_DIRECTORY/fast-downward.py ${SEARCH_OPTION[@]} ../$DOMAIN_FILE $PDDL_FILE

if (( $? != 0 )); then
  echo "=== Plan not found ==="
  exit 1
fi

echo "=== Plan found ==="
cat sas_plan
echo

echo "Converting plan to solution ..."
../$CONVERT_PLAN_TO_SOLUTION sas_plan > ../$SOL_FILE

echo "Validating solution ..."
../$VALIDATE_SOLUTION -v ../$SPEC_FILE ../$SOL_FILE

if [[ $keep_temp_dir == "no" ]] ; then
  echo "Deleting temporary directory ..."
  cd ..
  rm -R $TMP_DIR
fi

# Author: Tsz-Chiu Au
# Copyright (c) 2022 Tsz-Chiu Au. All rights reserved.
