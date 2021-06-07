#!/bin/bash

if [ "$1" = "--help" ]; then
	echo "cmoduml"
	echo "prints PlantUML graph of C++ module dependencies"
	echo
	exit
fi

rootPath=$(realpath $(dirname $(realpath -s $BASH_SOURCE))/../..)

pushd $rootPath >/dev/null

echo '@startuml'
tools/swatools/lscmod.sh | perl -e '
$opened = 0;
while (<>) {
	chomp;
	$name = $_;
	if (not /^\t/) {
		if ($opened == 1) { print "}\n"; };
		print "rectangle $name {\n";
		$opened = 1;
	} else {
		$name =~ s/\t//;
		print "rectangle $name\n";
	}
}
print "}\n";
'
tools/swatools/cmoddep.sh | grep -vP '(io|ipc|geom|text|util|math|type)' | perl -ne '
m/^([^:]+): (.+)$/;
$user = $1;
$usee = $2;
$user =~ s|/|_|g;
$usee =~ s|/|_|g;
print "$user -- $usee\n"
'
echo '@enduml'

popd >/dev/null
