#! /bin/sh
if [ -z "$JAVA_HOME" ]; then
  echo "The JAVA_HOME environment variable is not defined"
  echo "This environment variable is needed to run this program"
  exit 1
fi

IXSTRIM_HOME=`dirname $0`/..

JVM_PARAMS='-Xms64M -Xmx150M'

CP=$IXSTRIM_HOME/classes
CP=$IXSTRIM_HOME/lib/servlet.jar:$CP
JAVA_CMD=$JAVA_HOME/bin/java

$JAVA_CMD $JVM_PARAMS -classpath $CP ixstrim.examples.handler.HttpServer $1 1>out 2>err &
