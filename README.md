## Introduction

A zot langauge interpreter in C++.

http://semarch.linguistics.fas.nyu.edu/barker/Iota/zot.html

Zot

    Syntax:      Semantics:

    F --> F B    [F]([B])
    F --> e      ^c.cI
    B --> 0      ^c.c(^f.fSK)
    B --> 1      ^cL.L(^lR.R(^r.c(lr)))

I've also included the reference javascript interpreter from Chris Barker and a
debug mode which allows to visually see program evaluation and term reduction.
