#!/bin/bash

if [ "$1" = "--help" ]; then
	echo "lscmod"
	echo "lists C++ modules"
	echo
	exit
fi

rootPath=$(realpath $(dirname $(realpath -s $BASH_SOURCE))/../..)

pushd $rootPath >/dev/null

tools/swatools/lscsrc.sh | grep "src/" | perl -e '
while(<STDIN>) {
	chomp;
	m|^src/([^/]+)/([^/]+)/|;
	$mods->{$1}->{$2}++;
}
for (keys %{$mods}) {
	$group = $_;
	print "$group\n";
	for (keys %{$mods->{$group}})
	{
		$name = $_;
		print "\t$name\n";
	}
}
'

popd >/dev/null
