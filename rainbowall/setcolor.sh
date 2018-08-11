#!/bin/bash

# updates the colour of the background in the css file (we have a placeholder XXX there)
sed "s/XXX/$1/g" < styletemplate.css > style.css

# updates the last modified stuff
convert -background white -fill black -pointsize 24 label:"$(date +'%T')" lastmodified_old.gif
cp lastmodified_old.gif lastmodified.gif

# set the css to random query string to force uncache
str="s/YYY/`head -c 8 /dev/urandom | base64 | tr -d '='`/"
echo $str
sed $str indextemplate.html > index.html
