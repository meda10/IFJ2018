#/bin/bash

# IFJ18 - Priklady pro studenty pro dokresleni zadani

COMPILER="../ifj2018"
INTERPRETER="../ic18int"

for i in *.src; do

	IN=`echo $i | sed 's/\(.*\)\.src/\1.in/g'`
	OUT=`echo $i | sed 's/\(.*\)\.src/\1.out/g'`
	CODE=`echo $i | sed 's/\(.*\)\.src/\1.code/g'`

	$COMPILER < $i > $CODE   # nekontroluji se chyby prekladu (pro chybove testy nutno rozsirit)
	RETURNED_COMPILER=$?

	echo -n "DIFF: $INTERPRETER $i: "
	cat $IN | $INTERPRETER $CODE | diff - $OUT > /dev/null 2> /dev/null
	DIFFOK=$?
	if [ "x$DIFFOK" == "x0" ]; then
		echo "OK"
	else
		echo "DIFFERENCE to $OUT"
		cat $IN | $INTERPRETER $CODE | diff - $OUT
	fi
done;

rm *.code

exit 0
