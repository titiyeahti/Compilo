# check some syntax and bad syntax rules
#
# Usage:
#   bash bad-syntax.sh
# Configuration:
#  change the run_logic shell builtin to run the logic executable

#T4:0
#good function ff = a ;             # 2
#err  function ff = a               # 3
#T5:3
#good function ff(a) = a;           # 4
#err  function ff()  = a;           # 5
#T6:6
#good function ff(a) = [ 0,1 ];     # 6
#err  function ff(a) = [ 0,* ];     # 7
#T7:9
#good function ff(a) = [ 0,1 ];     # 8
#err  function ff(a) = [  ];        # 9
#T8:12
#good function ff = [ 0:1 , 1:1];   # 10
#err  function ff = [ 0:2 , 1:0];   # 11
#T9:15
#good function ff = [ 0:65536 ];    # 12
#err  function ff = [ 0:65537 ];    # 13
#T10:18
#good def-set  ff = { 1:4 };        # 14
#err  def-set  ff = { 0:0 };        # 15
#T11:21
#good def-set  ff = { 0:16 };       # 16
#err  def-set  ff = { 0:17 };       # 17
#T12:24
#good def-set  ff = {  };           # 18
#T13:26
#good ls ;                          # 19
#err  ls                            # 20
#T14:29
#good fct ff=a; print ff /le/fp/pc/le/oa/le/df/le # 21
#good /tt/df/tt/le/fp/pc/le/oa/le/df/le/tt/df/tt; # 22
#err  print ff /le/pc/fp ;                        # 23
#T15:33
#good fct ff=a; print ff /le /fp /pc /le /oa      # 24
#good /le /df /le /tt /df /tt;                    # 25
#err  print ff /pc/oa/le ;                        # 26
#T16:37
#good fct ff=a; print ff /le /fp /pc /le /oa      # 27
#good /le /df /le /tt /df /tt;                    # 28
#err  print ff /oa/tt ;                           # 29
#T17:41
#good fct ff=a; graph ff /le/pc/le/oa/le /df/le;  # 30
#err  fct ff=a; graph ff /fp;                     # 31
#T18:44
#good fct ff=a; graph ff /le/pc/le/oa/le /df/le;  # 32
#err  fct ff=a; graph ff /tt;                     # 33

#############################################################
########################## AUTO-TEST ########################
shline=58         # CAUTION: must be the number of this line
teline=9          # 1st line of logic source

# Configure this builtin to run the logic compiler.
run_logic()
{
#   ./logic               # verbose
    ./logic > /dev/null   # silence
}

########################## UTILITIES ########################

test=1            # test counter
score=0           # number of successful tests

source $(dirname $0)/bad-util.sh

########################## THE TESTS ########################

title="programme sans instruction"
if echo | run_logic; then
    print_test_status 1 "$title"
else
    print_test_status 0 "$title"
fi

title="instruction end"
if echo "end a b c d" | run_logic; then
    print_test_status 1 "$title"
else
    print_test_status 0 "$title"
fi

title="instruction quit"
if echo "quit a b c d" | run_logic; then
    print_test_status 1 "$title"
else
    print_test_status 0 "$title"
fi

run_test_2line_g_gb   $[teline+ 0] "fct ... sans ';'"
run_test_2line_g_gb   $[teline+ 3] "fct ff() = ..."
run_test_2line_g_gb   $[teline+ 6] "fct ff = [ 0 * ]"
run_test_2line_g_gb   $[teline+ 9] "fct ff = [  ]"
run_test_2line_g_gb   $[teline+12] "pts ff = [ 0:2 , 1:0 ]"
run_test_2line_g_gb   $[teline+15] "pts ff = [ 0:2**16+1 ]"
run_test_2line_g_gb   $[teline+18] "pts ff = { 0:0  }"
run_test_2line_g_gb   $[teline+21] "pts ff = { 0:17 }"
run_test_range_good   $[teline+24] "pts ff = {  }"
run_test_2line_g_gb   $[teline+26] "instruction ls without ';'"
run_test_3line_g_gb   $[teline+29] "print options: /pc/fp"
run_test_3line_g_gb   $[teline+33] "print options: /pc/oa/le"
run_test_3line_g_gb   $[teline+37] "print options: /oa/tt"
run_test_2line_g_gb   $[teline+41] "graph options: /fp"
run_test_2line_g_gb   $[teline+44] "graph options: /tt"

##########################   SCORE   ########################

test=$[$test-1]
printf "score= %d/%d ~ %d/100\n" $score $test $[($score*100)/$test] 1>&0
