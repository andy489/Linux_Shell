#!/bin/bash

awk 'BEGIN{level=0;maxLevel=0;}\
        /{/{level++}\
        /}/{level--}\
        (level>maxLevel){maxLevel = level}\
END{print "The deepest nesting is " maxLevel ".";}' $1
