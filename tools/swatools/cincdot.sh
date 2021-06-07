#!/bin/bash

if [ "$1" = "--help" ]; then
	echo "cincdot"
	echo "prints DOT graph of C header includes"
	echo
	exit
fi

rootPath=$(realpath $(dirname $(realpath -s $BASH_SOURCE))/../..)

pushd $rootPath >/dev/null

echo '
digraph G {
    graph [splines=true overlap=false];
'
tools/swatools/cincdep.sh | perl -pe 's/^([^:]+): (.+)$/"\2" -> "\1";/'
echo "}"

popd >/dev/null
