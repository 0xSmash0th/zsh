#!/bin/sh
#
# mkmodindex.sh: search for *.mdd files, and index the modules
#
# $@ = directories to search from
#

echo "# module index generated by mkmodindex.sh"
echo

omit_modules="`echo $OMIT_MODULES | sed 's/,/ /'`"

module_list=' '
while test $# -ne 0; do
    dir=$1
    shift
    ( set $dir/*.mdd; test -f $1 ) || continue
    for modfile in $dir/*.mdd; do
	name=`( . $modfile; echo $name )`
	case "$name" in *[!/_0-9A-Za-z]* | /* | */ | *//*)
	    echo >&2 "WARNING: illegally named module \`$name' in $modfile"
	    echo >&2 "         (ignoring it)"
	    continue
	;; esac
	q_name=`echo $name | sed 's,Q,Qq,g;s,_,Qu,g;s,/,Qs,g'`
	case " $module_list " in *" $name "*)
	    eval "omodfile=\$modfile_$q_name"
	    echo >&2 "WARNING: module \`$name' (in $omodfile) duplicated in $modfile"
	    echo >&2 "         (ignoring duplicate)"
	    continue
	;; esac
	case " $omit_modules " in *" $name "*)
	    echo >&2 "Module \`$name' found in \$OMIT_MODULES"
	    echo >&2 "         (omitting it)"
	    continue
	;; esac
	module_list="$module_list$name "
	echo "modfile_$q_name=$modfile"
	eval "modfile_$q_name=\$modfile"
    done
    set `echo $dir/*/. '' | sed 's,/\. , ,g'` "$@"
done

echo
echo $module_list | sed 's/^/module_list="/;s/$/"/'

exit 0
