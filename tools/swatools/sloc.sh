#!/bin/bash

if [ "$1" = "--help" ]; then
	echo "sloc"
	echo "prints source line count for the project"
	echo
	exit
fi

rootPath=$(realpath $(dirname $(realpath -s $BASH_SOURCE))/../..)

pushd $rootPath >/dev/null

wc -m -l $( tools/swatools/lssrc.sh ) | sort -n

popd >/dev/null
