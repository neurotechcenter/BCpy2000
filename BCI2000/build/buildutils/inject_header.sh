#!/bin/sh
for i in `find . -iname "*.cpp"` `find . -iname "*.h"` `find . -iname "*.m"` `find . -iname "Makefile"` `find . -iname "*.bat"` `find . -iname "*.y"` `find . -iname "*.h.in"`;
  do echo "$i";
  mv "$i" "$i.bak" && ( build/buildutils/fix_newlines --style-UNIX < "$i.bak" | build/buildutils/inject_header "$1" | build/buildutils/fix_newlines --style-PC > "$i" )
done;
