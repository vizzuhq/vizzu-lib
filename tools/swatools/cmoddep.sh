#!/bin/bash

if [ "$1" = "--help" ]; then
	echo "cmoddep"
	echo "lists C++ module dependencies"
	echo
	exit
fi

rootPath=$(realpath $(dirname $(realpath -s $BASH_SOURCE))/../..)

pushd $rootPath >/dev/null

tools/swatools/cincdep.sh | perl -e '
while(<STDIN>) {
	m/^([^:]+): (.+)$/;
	$into = $1;
	$what = $2;
	$into =~ s|^[^/]+/([^/]+)/.*|\1|;
	$what =~ s|^[^/]+/([^/]+)/.*|\1|;
	if ($into ne $what) {
		$line = "$into: $what";
		$lines{$line}++;
	}
}
print "$_\n" for (keys %lines);
'

popd >/dev/null
