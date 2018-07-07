#!/bin/bash

module load impi
hostname
#sudo cpupower frequency-set --governor cpufreq_performance
#sudo cpupower frequency-set --freq 3200000
#sudo cpupower frequency-set --governor cpufreq_powersave

case "$2" in
    "lj")
        ./toolkits/"$1" /storage/youwei/datasets/ljournal-2008.ep 5363260 "$3"
        ;;
    "tw")
        ./toolkits/"$1" /storage/youwei/datasets/twitter-2010.txt.ep 41652230 "$3"
        ;;
    "uk")
        ./toolkits/"$1" /storage/youwei/datasets/uk-2007-05.txt.ep 105896555 "$3"
        ;;
    "wk")
        ./toolkits/"$1" /storage/youwei/datasets/enwiki-2013.txt.ep 4206785 "$3"
        ;;
    "wa")
        ./toolkits/"$1" /storage/youwei/datasets/wordassociation-2011.ep 10617 "$3"
        ;;
    "test")
        ./toolkits/"$1" /storage/youwei/datasets/test.ep 10 "$3"
        ;;
    "simple")
        ./toolkits/"$1" /home/amadeuschan/original_version/GeminiGraph/simple_graph.biedgelist 1000000 "$3"
        ;;
     *)
        echo "no "$2" dataset"
        exit 1
        ;;
esac

