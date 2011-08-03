#!/bin/sh
echo "Starting Player..."
 appname=`basename "$0" | sed s,\.sh$,,`

 dirname=`dirname "$0"`
 tmp="${dirname#?}"

 if [ "${dirname%$tmp}" != "/" ]; then
 dirname="$PWD/$dirname"
 fi
 
 echo "Set LD_LIBRARY_PATH=$dirname"
 LD_LIBRARY_PATH="$dirname"

 export LD_LIBRARY_PATH
 mkdir "$dirname/logs"
 chmod +x "$dirname/Player.com"
 "$dirname/Player.com" "$1"

