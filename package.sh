#!/bin/sh

PACKAGE=`pwd`/strat.zip

rm -f "$PACKAGE"
cd src
zip "$PACKAGE" *.cpp *.h
