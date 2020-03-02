#!/bin/bash

INPUT="$1"
ACTION="$2"
PREFIX="$3"

if [ "${INPUT}" == "help" ]; then
    echo "USAGE: ${0} chunksFolder [check|merge] prefix"
    echo "Default prefix: Friend"
    exit 0
fi
if [ "${PREFIX}" == "" ]; then
    PREFIX="Friend"
fi

if [ "${ACTION}" == "merge" ]; then
    CMD="$PWD/friendChunkAdd.sh ${PREFIX}"
    cd ${INPUT}
    sh ${CMD}
    cd -
fi

if [ "${ACTION}" == "check" ]; then
    echo "here"
    CMD="$PWD/friendChunkCheck.sh -z ${PREFIX} ${INPUT}"
    cd ${INPUT}
    sh ${CMD}
    cd -
fi

if [ "${ACTION}" == "finalcheck" ]; then
    CMD="python $PWD/checker.py ${INPUT}"
    sh ${CMD}
fi


echo ${CMD}


#if [ "${ACTION}" == "merge" ]; then
#    mkdir chunks
#    mv *.chunk*root chunks/
#fi


exit 0
