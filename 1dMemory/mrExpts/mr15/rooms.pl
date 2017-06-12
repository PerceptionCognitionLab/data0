#! /usr/bin/perl

srand (time ^ $$ ^ unpack "%L*", `ps axww | gzip`);
@rooms=(1,2,3,4);
push(@scrambledrooms,splice(@rooms, int(rand($#rooms+1)), 1)) while (@rooms);
print "@scrambledrooms";

