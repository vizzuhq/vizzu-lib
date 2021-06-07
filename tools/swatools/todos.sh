#!/bin/bash

if [ "$1" = "--help" ]; then
	echo "todos"
	echo "lists all todos from all source files"
	echo
	exit
fi

rootPath=$(realpath $(dirname $(realpath -s $BASH_SOURCE))/../..)

pushd $rootPath >/dev/null

grep -n todo $( tools/swatools/lssrc.sh | grep -v tools ) \
	| perl -pe "s|^([^:]*:[^:]*):.*todo:?[ \t]*|${rootPath}/\1:0: info: todo: |"

popd >/dev/null
