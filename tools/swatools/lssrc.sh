#!/bin/bash

function echo_help() {
	echo "lssrc"
	echo "lists source files"
	echo
}

function grep_sources() {
	grep -P '\.(h|cpp|inc|tpp|js|sh)$'
}

function filt_ignored() {
	grep -v -P '(moc|ui_|qrc_|/res.|contrib|build|project|node_modules)'
}

rootPath=$(realpath $(dirname $(realpath -s $BASH_SOURCE))/../..)

if [ "$1" = "--help" ]
then
	echo_help
	exit
fi

find $rootPath | grep_sources | filt_ignored | perl -pe "s|$rootPath/||"
