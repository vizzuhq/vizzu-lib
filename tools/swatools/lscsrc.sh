#!/bin/bash

if [ "$1" = "--help" ]; then
	echo "lscsrc"
	echo "lists C++ source files"
	echo
	exit
fi

rootPath=$(realpath $(dirname $(realpath -s $BASH_SOURCE))/../..)

pushd $rootPath >/dev/null

tools/swatools/lssrc.sh | grep -P '\.(cpp|h|inc|tpp)$' | grep -v tools

popd >/dev/null
