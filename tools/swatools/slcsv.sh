#!/bin/bash

if [ "$1" = "--help" ]; then
	echo "slcsv"
	echo "prints source line data in CSV format"
	echo
	exit
fi

rootPath=$(realpath $(dirname $(realpath -s $BASH_SOURCE))/../..)

pushd $rootPath >/dev/null

echo "chars, lines, files, dir0, dir1, dir2, dir3, dir4, dir5, dir6, tmp"
wc -m -l $( tools/swatools/lssrc.sh )  | grep -v total | sort -n | perl -ne '
    @cells = split /\s+/, $_;
    $lines = $cells[1];
    $chars = $cells[2];
    $path = $cells[scalar @cells - 1];
    $path =~ s/(.*)\/([^\/]+)$//;
    $dirs = $1;
    $file = $2;
	$dirs =~ s|web/public/scripts|web.public.srcipts|;
	@dirlist = split /\//, $dirs;
	$dirlist[7] = 0;
	$dircells = join ",", @dirlist;
    print "$chars,$lines,$file,$dircells\n";
'
popd >/dev/null