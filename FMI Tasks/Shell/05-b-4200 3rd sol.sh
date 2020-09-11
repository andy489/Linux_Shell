#!/bin/bash

# https://github.com/steneva

# we assume that there is only one curly bracket per row

awk 'BEGIN{level=0;maxLevel=0;}\
        /{/{level++}\
        /}/{level--}\
        (level>maxLevel){maxLevel = level}\
END{print "The deepest nesting is " maxLevel ".";}' $1
