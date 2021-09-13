#!/bin/bash

PACKAGE=hello
lines=10 
tail -n+$lines $0 > /tmp/${PACKAGE}.tar.bz2
cd /tmp
tar jxvf ${PACKAGE}.tar.bz2
cp -r ${PACKAGE}/* /
exit 0
