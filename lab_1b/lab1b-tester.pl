#! /usr/bin/perl -w

open(FOO, "main-a.c") || die "Did you delete main-a.c?";
$lines = 0;
$lines++ while defined($_ = <FOO>);
close FOO;

$rev = 'rev';
if (-d '/w/class.1/cs/cs111/cbin/bin') {
    $ENV{'PATH'} = "/w/class.1/cs/cs111/cbin/bin:" . $ENV{'PATH'};
}

@tests = (
# Execute
	  [ 'Test 1 (Basic Execution)',
	    'echo Hooray',
	    'Hooray' ],

	  [ 'Test 2',
	    'echo Semi ;',
	    'Semi' ],

	  [ 'Test 3',
	    'echo Line & sleep 1',
	    'Line' ],

	  [ 'Test 4',
	    'echo "" """" EOL',
	    'EOL' ],

	  [ 'Test 5',
	    'echo He"llo wo"rld',
	    'Hello world' ],

	  [ 'Test 6',
	    'echo Hello   World',
	    'Hello World' ],

# Use a vertical tab to avoid tab completion
	  [ 'Test 7',
	    'echo Hello World',
	    'Hello World' ],

	  [ 'Test 8',
	    'echo Pipe | cat',
	    'Pipe' ],

# Conditional
	  [ 'Test 9 (Conditionals)',
	    'true && echo True',
	    'True' ],

	  [ 'Test 10',
	    'echo True || echo False',
	    'True' ],

	  [ 'Test 11',
	    'grep -cv NotThere main-a.c && echo False',
	    "$lines False" ],

	  [ 'Test 12',
	    'false || echo True',
	    'True' ],

	  [ 'Test 13',
	    'true && false || true && echo Good',
	    'Good' ],

	  [ 'Test 14',
	    'echo True && false || false && echo Bad',
	    'True' ],

# Test that conditional status is inherited through parentheses
	  [ 'Test 15',
	    '( ( false || echo False && false ) || ( echo Inherited ) ) && echo Nested',
	    'False Inherited Nested' ],

# Pipe
	  [ 'Test 16 (Pipes)',
	    'echo Good | grep G',
	    'Good' ],

	  [ 'Test 17',
	    'echo Bad | grep -c G',
	    '0' ],

	  [ 'Test 18',
	    'echo Line | cat | wc -l',
	    '1' ],

	  [ 'Test 19',
	    "echo GoHangASalamiImALasagnaHog | $rev | tee temp.out | $rev | $rev",
	    'goHangasaLAmIimalaSAgnaHoG' ],

	  [ 'Test 20',
	    "$rev temp.out | $rev",
	    'goHangasaLAmIimalaSAgnaHoG' ],

	  [ 'Test 21',
	    'cat temp.out | tr [A-Z] [a-z] | md5sum | tr -d -',
	    '8e21d03f7955611616bcd2337fe9eac1' ],

	  [ 'Test 22',
	    "$rev temp.out | md5sum | tr [a-z] [A-Z] | tr -d -",
	    '502B109B37EC769342948826736FA063' ],

# Sequential
	  [ 'Test 23 (Sequential Execution)',
	    'echo Hello ; echo There',
	    'Hello There' ],

	  [ 'Test 24',
	    'echo Hello ;   echo There ; echo Who ; echo Are ; echo You ; echo ?',
	    'Hello There Who Are You ?' ],

	  [ 'Test 25',
	    'rm -f temp.out ; echo Removed',
	    'Removed' ],

	  [ 'Test 26',
	    'sleep 2 ; ps | grep sleep | grep -v grep ; echo Done',
	    'Done' ],

# Parentheses
	  [ 'Test 27 (Parentheses)',
	    '( echo Line )',
	    'Line' ],

	  [ 'Test 28',
	    '( echo Hello ; echo World ) | wc -l',
	    '2' ],

	  [ 'Test 29',
	    '( echo Line ; echo Line ) | uniq | wc -l',
	    '1' ],

	  [ 'Test 30',
	    '( echo Hello -" ) " World " ( "- ; echo Next ) | cat',
	    'Hello - ) World ( - Next' ],

	  [ 'Test 31',
	    '( true ) && echo True',
	    'True' ],

	  [ 'Test 32',
	    '( false || true ) && echo True',
	    'True' ],

	  [ 'Test 33',
	    '( sleep 1 ; echo Sleep ) & echo Wake ; sleep 2',
	    'Wake Sleep' ],

# BuildMultiLine (setup for Multiple-lines case below)
	  [ 'Test 34 (Multi-line Scripts)',
	    'echo echo Line 1 > temp.out ; echo echo Line 2 | cat temp.out - > temp2.out ; mv -f temp2.out temp.out ; echo echo Line 3 | cat temp.out - > temp2.out ; mv -f temp2.out temp.out ; echo Build',
	    'Build' ],

# Multiple-lines
	  [ 'Test 35',
	    './ospsh -q < temp.out',
	    'Line 1 Line 2 Line 3' ],

# Redirection
	  [ 'Test 36 (Redirection)',
	    'echo Start ; echo File > temp.out',
	    'Start' ],

	  [ 'Test 37',
	    'cat < temp.out ; echo Done',
	    'File Done' ],

	  [ 'Test 38',
	    'rm file_that_is_not_there 2> temp.out ; wc -l temp.out ; rm -f temp.out',
	    '1 temp.out' ],

# Test simultaneous input and output redirection
	  [ 'Test 39',
	    '( echo Hello ; echo Bye ) > temp.out ; cat < temp.out > temp2.out ; cat temp.out temp2.out',
	    'Hello Bye Hello Bye' ],

	  [ 'Test 40',
	    'sort < temp.out | ( head -2 ; echo First && echo Good )',
	    'Bye Hello First Good' ],

	  [ 'Test 41',
	    'sort < temp.out > temp2.out ; tail -2 temp2.out ; rm -f temp.out temp2.out',
	    'Bye Hello' ],

# Background
# Test that sleep is backgrounded and later commands are run
	  [ 'Test 42 (Background)',
	    'sleep 2 & ps | grep sleep | grep -v grep | wc -l',
	    '1' ],

# Test parentheses
	  [ 'Test 43',
	    '( sleep 2 ) & ps | grep sleep | grep -v grep | wc -l',
	    '1' ],

# Test multiple background commands
	  [ 'Test 44',
	    '( echo Hello ; sleep 2 ) & sleep 1 ; ps | grep sleep | grep -v grep | wc -l',
	    'Hello 1' ],

	  [ 'Test 45',
	    '( echo Hello ; sleep 2 & echo Bye ) ; ps | grep sleep | grep -v grep | wc -l',
	    'Hello Bye 1' ],

# cd
	  [ 'Test 46 (cd)',
	    'cd / ; pwd',
	    '/' ],

	  [ 'Test 47',
	    'cd / ; cd /usr ; pwd',
	    '/usr' ],

# cd without redirecting stdout
	  [ 'Test 48',
	    'cd / ; cd /doesnotexist 2> /dev/null ; pwd',
	    '/' ],

	  [ 'Test 49',
	    'cd / ; ( cd /tmp ) ; pwd',
	    '/' ],

# Fancy conditionals
	  [ 'Test 50',
	    'cd / && pwd',
	    '/' ],

	  [ 'Test 51',
	    'echo go ; cd /doesnotexist 2> /dev/null > /dev/null && pwd',
	    'go' ],

	  [ 'Test 52',
	    'cd /doesnotexist 2> /dev/null > /dev/null || echo does not exist',
	    'does not exist' ],

	  [ 'Test 53',
	    'cd /tmp && cd / && pwd',
	    '/' ],

	  [ 'Test 54',
	    'cd / ; ( cd /doesnotexist1 2> /dev/null > /dev/null || cd / || cd /doesnotexist2 2> /dev/null > /dev/null ) && pwd',
	    '/' ],

	  [ 'Test 55',
	    'cd / ; cd /doesnotexist 2> /dev/null > /dev/null ; pwd',
	    '/' ],

# BuildMultiLine (setup for Exit case below)
	  [ 'Test 56 (exit)',
	    'echo echo Line 1 > temp.out ; echo exit | cat temp.out - > temp2.out ; mv -f temp2.out temp.out ; echo echo Line 3 | cat temp.out - > temp2.out ; mv -f temp2.out temp.out ; cat temp.out',
	    'echo Line 1 exit echo Line 3' ],

# Exit
	  [ 'Test 57',
	    './ospsh -q < temp.out',
	    'Line 1' ],

	  [ 'Test 58',
	    'echo Before ; exit ; echo After',
	    'Before' ],

	  [ 'Test 59',
	    '( exit ) ; echo After',
	    'After' ],

# BuildMultiLine
	  [ 'Test 60',
	    'rm -f temp.out ; echo echo Line 1 "&" sleep 1 > temp1.out ; echo echo Line 2 | cat temp1.out - > tempt.out ; mv -f tempt.out temp1.out ; cat temp1.out',
	    'echo Line 1 & sleep 1 echo Line 2' ],

	  [ 'Test 61',
	    'echo "sleep 2 & ps | grep sleep | grep -v grep | wc -l" > temp2.out ; echo "sleep 3" | cat temp2.out - > tempt.out ; mv -f tempt.out temp2.out ; cat temp2.out',
	    'sleep 2 & ps | grep sleep | grep -v grep | wc -l sleep 3' ],

	  [ 'Test 62',
	    'echo "ps | grep sleep | grep -v grep | wc -l" | cat temp2.out - > tempt.out ; mv -f tempt.out temp2.out ; cat temp2.out',
	    'sleep 2 & ps | grep sleep | grep -v grep | wc -l sleep 3 ps | grep sleep | grep -v grep | wc -l' ],

# Zombie
# Method 1
	  [ 'Test 63 (Zombie Processes)',
	    'cat temp1.out | ./ospsh -q & sleep 2 ; ps | grep echo | grep -v grep | grep "<defunct>$"',
	    'Line 1 Line 2' ],

# Method 2
	  [ 'Test 64',
	    './ospsh -q < temp2.out',
	    '1 0' ],

# CleanupMultiLine
	  [ 'Test 65',
	    'rm -f temp1.out temp2.out ; echo clean',
	    'clean' ],
	  );

my($ntest) = 0;

my($sh) = "./ospsh";
my($tempfile) = "lab1btest.txt";
my($ntestfailed) = 0;

if (!-x $sh) {
    $editsh = $sh;
    $editsh =~ s,^\./,,;
    print STDERR "$sh does not exist, so I can't run any tests!\n(Try running \"make $editsh\" to create $sh.)\n";
    exit(1);
}

select STDOUT;
$| = 1;

foreach $test (@tests) {
    my($desc, $in, $want) = @$test;
	if ($ENV{'HOME'} =~ "/Users/") { # if we're in os x, change md5sum to md5
		$in =~ s/md5sum/md5/g;
	}
    $ntest++;
    print STDOUT "Running $desc...";
    open(F, ">$tempfile") || die;
    print F $in, "\n";
    close(F);
    $result = `$sh -q < $tempfile 2>&1`;
    print STDOUT " done\n";
    $result =~ s%^cs111_winter11\$ %%;
    $result =~ s%cs111_winter11\$ $%%;
    $result =~ s%^\[\d+\]\s+\d+$%%mg;
    $result =~ s|\[\d+\]||g;
    $result =~ s|^\s+||g;
    $result =~ s|\s+| |g;
    $result =~ s|\s+$||;
    next if $result eq $want;
    next if $want eq 'Syntax error [NULL]' && $result eq '[NULL]';
    next if $result eq $want;
    print STDERR "$desc FAILED!\n  input was \"$in\"\n  expected output like \"$want\"\n  got \"$result\"\n";
    $ntestfailed += 1;
}

unlink($tempfile);
my($ntestpassed) = $ntest - $ntestfailed;
print "$ntestpassed of $ntest tests passed\n";
exit(0);
