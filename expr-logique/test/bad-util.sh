# shell script utilities for checking semantic and syntax rules
#
# cmd 0/1 message
print_test_status()
{
    fmt="test %2d:%4s: %s\n"
    if test $1 = 0 ; then
        printf "$fmt" $test FAIL "$2"
    else
        printf "$fmt" $test ok "$2"
        score=$[$score+1]
    fi
    test=$[test+1]
}

# cmd range
line_good()
{
#   echo 1>&2 "sed -e $shline,\$d $0 | sed -e \"$1s/#good//\""
    sed -e $shline,\$d $0 | sed -e "$1s/#good//"
}
line_err()
{
#   echo 1>&2 "sed -e $shline,\$d $0 | sed -e \"$1s/#err//\""
    sed -e $shline,\$d $0 | sed -e "$1s/#err//"
}
line_good_err()
{
   echo 1>&2 "sed -e $shline,\$d $0 | sed -e \"$1s/#good//\" -e \"$1s/#err//\""
    sed -e $shline,\$d $0 | sed -e "$1s/#good//" -e "$1s/#err//" > /tmp/test
    sed -e $shline,\$d $0 | sed -e "$1s/#good//" -e "$1s/#err//"
}

# args: line-range mess
run_test_range_g_gb()
{
    if line_good "$1" | run_logic ; then
        if line_good_err "$1"  | run_logic ; then
            print_test_status 0 "$2"
        else
            status=$?
            if test "$status" -eq 126 -o "$status" -eq 127 ; then
                echo "CAUTION: logic compiler not found"
                echo "CAUTION: set the run_logic() builin"
                exit 1
            fi
            if test "$status" -ge 126 -a "$status" -le $[128+15] ; then
                print_test_status 0 "$2"
            else
                print_test_status 1 "$2"
            fi
        fi
    else
       status=$?
       if test "$status" -eq 126 -o "$status" -eq 127 ; then
           echo "CAUTION: logic compiler not found"
           echo "CAUTION: set the run_logic() builin"
           exit 1
       fi
       print_test_status 0 "$2"
    fi
}

# args: line mess --> run_test_range_g_gb line,line+1 mess
run_test_2line_g_gb() { run_test_range_g_gb "$1,$[$1+1]" "$2" ; }
run_test_3line_g_gb() { run_test_range_g_gb "$1,$[$1+2]" "$2" ; }

# args: line mess 
run_test_range_good()
{
    if line_good "$1" | $run_logic; then
        print_test_status 1 "$2"
    else
        print_test_status 0 "$2"
    fi
}

# args: line mess 
run_test_range_bad()
{
    if line_err "$1" | $run_logic; then
        print_test_status 0 "$2"
    else
        print_test_status 1 "$2"
    fi
}

