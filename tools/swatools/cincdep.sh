#!/bin/bash

if [ "$1" = "--help" ]; then
	echo "cincdep"
	echo "lists C++ include dependencies"
	echo
	exit
fi

rootPath=$(realpath $(dirname $(realpath -s $BASH_SOURCE))/../..)

pushd $rootPath >/dev/null

for file in $( tools/swatools/lscsrc.sh | grep "src/" )
do
	perl -sne '
		$src =~ s|src/||;
		$src =~ m|(.*)/[^/]+$|;
		$dir = $1."/";
		if(/#include "([^"]*)"/) {
			$inc = $1;
			if ($inc !~ m|/|) { $inc = $dir.$inc; }
			print "$src: $inc\n";
		}' -- -src=$file < $file
done

popd >/dev/null
