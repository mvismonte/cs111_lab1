#! /usr/bin/perl -w

@tests = (
# Some simple commands
	  [ 'true',
	    '[1 args "true"] .' ],
	  [ 'echo Hooray',
	    '[2 args "echo" "Hooray"] .' ],
	  [ 'echo Hello   World',
	    '[3 args "echo" "Hello" "World"] .' ],
# Use a vertical tab to avoid tab completion
	  [ 'echo Hello World',
	    '[3 args "echo" "Hello" "World"] .' ],

# Heh. "Simple."
	  [ '( echo & ( echo ) | echo ) && echo | ( ( echo ; echo ) || echo )',
	    '[0 args [1 args "echo"] & [0 args [1 args "echo"] . ] | [1 args "echo"] . ] && [1 args "echo"] | [0 args [0 args [1 args "echo"] ; [1 args "echo"] . ] || [1 args "echo"] . ] .' ],

# Some redirections
	  [ 'ls > out.txt',
	    '[1 args "ls" >out.txt] .' ],
	  [ 'cat < in.txt',
	    '[1 args "cat" <in.txt] .' ],
	  [ 'cat < file > "copy of file"',
	    '[1 args "cat" <file >copy of file] .' ],
	  [ 'ls < in.txt 2> err.txt > out.txt',
	    '[1 args "ls" <in.txt >out.txt 2>err.txt] .' ],

# Later redirections override earlier ones
	  [ 'ls > out.txt > out2.txt > out3.txt',
	    '[1 args "ls" >out3.txt] .' ],

# Redirection errors
	  [ 'ls >',
	    'Syntax error [NULL]' ],
	  [ 'ls <',
	    'Syntax error [NULL]' ],
	  [ '2> out',
	    'Syntax error [NULL]' ],

# Simple connections between commands
	  [ 'ls ; cat',
	    '[1 args "ls"] ; [1 args "cat"] .' ],
	  [ 'ls & cat',
	    '[1 args "ls"] & [1 args "cat"] .' ],
	  [ 'ls && cat',
	    '[1 args "ls"] && [1 args "cat"] .' ],
	  [ 'ls || cat',
	    '[1 args "ls"] || [1 args "cat"] .' ],
	  [ 'ls | cat',
	    '[1 args "ls"] | [1 args "cat"] .' ],

# Only some kinds of commands can end the list
	  [ 'ls ;',
	    '[1 args "ls"] ;' ],
	  [ 'ls &',
	    '[1 args "ls"] &' ],
	  [ 'ls &&',
	    'Syntax error [NULL]' ],
	  [ 'ls ||',
	    'Syntax error [NULL]' ],
	  [ 'ls |',
	    'Syntax error [NULL]' ],

# Various errors
	  [ 'echo "Qu"ot"e',
	    'Syntax error [NULL]' ],
	  [ 'echo Combine | && cat',
	    'Syntax error [NULL]' ],
	  [ '|| echo Prefix',
	    'Syntax error [NULL]' ],
	  [ ') echo Ouch',
	    'Syntax error [NULL]' ],

# Longer command lines
	  [ 'ls && ls1 || ls2 | ls3 | ls4',
	    '[1 args "ls"] && [1 args "ls1"] || [1 args "ls2"] | [1 args "ls3"] | [1 args "ls4"] .' ],
	  [ 'ls ; ls1 ; ls2 ; ls3 ; ls4',
	    '[1 args "ls"] ; [1 args "ls1"] ; [1 args "ls2"] ; [1 args "ls3"] ; [1 args "ls4"] .' ],

# Parentheses
	  [ '( a )',
	    '[0 args [1 args "a"] . ] .' ],
	  [ '( a && ( b ; c ) ) | d',
	    '[0 args [1 args "a"] && [0 args [1 args "b"] ; [1 args "c"] . ] . ] | [1 args "d"] .' ],
	  [ '( a b c ) | ( d e f )',
	    '[0 args [3 args "a" "b" "c"] . ] | [0 args [3 args "d" "e" "f"] . ] .' ],
	  [ '( a b',
	    'Syntax error [NULL]' ],
	  [ '( a b ( c ) )',
	    'Syntax error [NULL]' ],
	  [ 'a b )',
	    'Syntax error [NULL]' ],

# Double quotes
	  [ 'echo "" ""',
	    '[3 args "echo" "" ""] .' ],
	  [ 'echo "" """"',
	    '[3 args "echo" "" ""] .' ],
	  [ 'echo He"llo wo"rld',
	    '[2 args "echo" "Hello world"] .' ],
	  [ 'echo "a b c" d "e ( f" ">" > foo',
	    '[5 args "echo" "a b c" "d" "e ( f" ">" >foo] .' ],
	  [ 'echo hel"lo ; wor"ld',
	    '[2 args "echo" "hello ; world"] .' ],
	  [ 'echo " abc this is an error',
	    'Syntax error [NULL]' ],
	  [ '"space program"',
	    '[1 args "space program"] .' ],

# Mixed examples
	  [ 'ls > ist ; cat list && rm list',
	    '[1 args "ls" >ist] ; [2 args "cat" "list"] && [2 args "rm" "list"] .' ],
	  [ 'cat < file | more',
	    '[1 args "cat" <file] | [1 args "more"] .' ],
	  [ 'echo | true > temp.out',
	    '[1 args "echo"] | [1 args "true" >temp.out] .' ],
	  [ '( ls ; uptime ) > file',
	    '[0 args >file [1 args "ls"] ; [1 args "uptime"] . ] .' ],
	  );

my($ntest) = 0;

my($sh) = "./cmdline";
my($tempfile) = "lab1atest.txt";
my($ntestfailed) = 0;

if (!-x $sh) {
    $editsh = $sh;
    $editsh =~ s,^\./,,;
    print STDERR "$sh does not exist, so I can't run any tests!\n(Try running \"make $editsh\" to create $sh.)\n";
    exit(1);
}

foreach $test (@tests) {
    $ntest++;
    print STDOUT "Running test $ntest\n";
    my($in, $want) = @$test;
    open(F, ">$tempfile") || die;
    print F $in, "\n";
    close(F);
    $result = `$sh < $tempfile 2>&1`;
    $result =~ s%^cs111_winter11\$ %%;
    $result =~ s%cs111_winter11\$ $%%;
    $result =~ s|^\s+||g;
    $result =~ s|\s+| |g;
    $result =~ s|\s+$||;
    next if $result eq $want;
    next if $want eq 'Syntax error [NULL]' && $result eq '[NULL]';
    next if $result eq $want;
    print STDERR "Test $ntest FAILED!\n  input was \"$in\"\n  expected output like \"$want\"\n  got \"$result\"\n";
    $ntestfailed += 1;
}

unlink($tempfile);
my($ntestpassed) = $ntest - $ntestfailed;
print "$ntestpassed of $ntest tests passed\n";
exit(0);
