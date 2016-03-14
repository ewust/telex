#!/bin/sh

head -n 6 $1 > .foo
tail -n +7 $1 | grep  '[[:alnum:]]' | sort >> .foo
echo >> .foo
rm $1
