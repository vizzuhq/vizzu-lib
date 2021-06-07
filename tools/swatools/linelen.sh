#!/bin/bash

if [ "$1" = "--help" ]; then
	echo "linelen"
	echo "prints line length histogram"
	echo
	exit
fi

rootPath=$(realpath $(dirname $(realpath -s $BASH_SOURCE))/../..)

pushd $rootPath >/dev/null

echo "lenght count"
cat $( tools/swatools/lssrc.sh ) | perl -e '
    while(<STDIN>) { $lens{length $_}++ }
    print "$_ $lens{$_}\n" for (sort { $a <=> $b } keys %lens)
'

popd >/dev/null
