#!/bin/sh

tocfile=../../../doc/htmlhelp/wiki/html/htmlhelp.toc
beginmsg="<!-- Content created automatically by BCI2000/src/doc/wiki2help/create_lists.sh -->"
endmsg="<!-- End of auto-created content -->"

make toc2wiki
echo Running toc2wiki ...

( \
  echo $beginmsg && \
  ( ./toc2wiki "^Parameters$" "^[^ ,]+(, [^ ,]+)*$" < $tocfile ) && \
  echo $endmsg \
) > parameters.wiki

( \
  echo $beginmsg && \
  ( ./toc2wiki "^States$" "^[^ ,]+(, [^ ,]+)*$" < $tocfile ) && \
  echo $endmsg \
) > states.wiki

echo Done.
