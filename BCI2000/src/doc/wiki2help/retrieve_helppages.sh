#!/bin/bash
#####################################################################
# $Id: retrieve_helppages.sh 5769 2018-05-24 12:30:40Z mellinger $
# Author: juergen.mellinger@uni-tuebingen.de
# Description: A shell script to download BCI2000 wiki pages using
#   the HTMLhelp user account.
#
#   Requires wget >= 1.10.
#   Execute this script from the directory where it is located 
#   in the BCI2000 source tree.
#
# (C) 2000-2011, BCI2000 Project
# http://www.bci2000.org
#####################################################################

INITIALWD=`pwd`;
SERVER="www.bci2000.org"
BASE="$SERVER/wiki"
TARGET="../../../doc/htmlhelp"
TMPFILE="~cookies"
TMPEXT="~tmp"
TOCFILE="htmlhelp.toc"

# Log in to the HTMLhelp account using /wiki/helplogin.php.
wget --spider --keep-session-cookies --save-cookies $TMPFILE \
     http://$BASE/helplogin
# Get all content listed in Special:Allpages.
wget --recursive --level=1 --convert-links --page-requisites \
	 --restrict-file-names=windows \
     --html-extension -e robots=off --load-cookies $TMPFILE \
     http://$BASE/index.php/Special:Allpages
# Log out of the server.
wget --spider --load-cookies $TMPFILE \
     http://$BASE/index.php/Special:Userlogout
rm $TMPFILE

mv $BASE/index.php $BASE/html
cd $BASE/html

# Remove unwanted content (scripts, comments) from html files
for i in *.html;
do
  mv $i $i.$TMPEXT &&
  sed 's/<script .*<\/script>//' < $i.$TMPEXT |
  sed 's/ onload=[^>]*>/>/' |
  sed 's/BCI2000 Wiki/BCI2000 Help/' |
  sed 's/<!-- Served by [^>]* -->//' > $i &&
  rm $i.$TMPEXT
done;

# Create an index of section headers
(
  echo "# BCI2000 Help TOC file" &&
  echo "# Format:" &&
  echo "#  Each file TOC begins with the file name on its own line." &&
  echo "#  For each TOC entry in the file, there is a line in the format:" &&
  echo "#  <toc level> <anchor name> <displayed heading>" &&
  echo "#  Each file TOC is concluded with a blank line." 
) > $TOCFILE
for i in *.html;
do
  ( 
    echo && ( echo $i | sed 's/%/%25/' ) &&
    sed '/<a name=\"[^\"]*\">/!d' < $i |
    sed 's/.*<a name=\"\([^\"]*\)\"><\/a><h\([0-9]*\)>\(.*\)<\/h[0-9]*>.*/\2 \1 \3/'
  ) >> $TOCFILE
done;
echo >> $TOCFILE

# Some browsers have problems with URLs escaping the '%' character.
# As a fix, we duplicate pages with ambiguous file names.
for i in *.html;
do
  if ( echo $i | grep % ); then
    j=`echo $i | sed 's/%/%25/'`;
    cp $i $j
  fi;
done;
echo

# Rename directories, and move content into the doc directory.
cd "$INITIALWD"
mkdir -p "$TARGET"
cp -af $SERVER/* "$TARGET" && rm -r $SERVER
