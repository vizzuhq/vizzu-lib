#!/bin/bash

rootPath=$(realpath $(dirname $(realpath -s $BASH_SOURCE))/../..)

echo '#include "base/app/git.h"'
echo 'namespace App::Git {'
echo -n 'const char *const hash = "'
$rootPath/tools/gittools/hash.sh | tr -d '\n'
echo '";'
echo -n 'const unsigned int depth = '
$rootPath/tools/gittools/commitdepth.sh | tr -d '\n'
echo ' + 0;'
echo '}'
