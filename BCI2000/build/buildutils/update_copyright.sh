#!/bin/sh
IFS=$'\n';
for i in `find . \( -iname "*.txt" -o -iname "*.cpp" -o -iname "*.h" -o -iname "*.m" -o -iname "Makefile" -o -iname "*.bat" -o -iname "*.y" \)`;
  do echo "$i";
  mv "$i" "$i.bak" && ( sed 's/(C) 2000-2011: BCI2000/(C) 2000-2012: BCI2000/' < "$i.bak" > "$i" )
done;

