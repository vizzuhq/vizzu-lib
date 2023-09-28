#!/bin/bash

if [ "$1" = "--help" ]; then
	echo "todos"
	echo "lists all todos from all source files"
	echo
	exit
fi

rootPath=$(realpath $(dirname $(realpath -s $BASH_SOURCE))/../..)
if [ -z "$1" ]; then
  srcPath="$rootPath"
else
  srcPath="$1"
fi

pushd $rootPath >/dev/null

grep -ni todo $( tools/swatools/lssrc.sh "$srcPath" | grep -v tools ) \
	| perl -pe "s|^([^:]*:[^:]*):.*todo:?[ \t]*|${rootPath}/\1:0: info: todo: |i"

popd >/dev/null
