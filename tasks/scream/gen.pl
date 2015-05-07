#!/usr/bin/perl

use strict;
use warnings;

my %res;

@res{0 .. 3} = ('AAAA', 'AAA', 'A', 'AA A');
$res{ord 'Q'} = 'AAAAA, AAAA';
$res{ord 'C'} = 'AAAAA, AAA';
$res{ord 'T'} = 'AAAAA, A';
$res{ord 'F'} = 'AAAAA, AA A';


sub update {
	my ($x, $y, $c, $z, $u) = @_;
	my $xx = $res{$x};
	my $yy = $res{$y};
	my $res = "$c $xx $yy";
	$res{$z} = $res unless defined $res{$z};
	$res{$z} = $res if length $res < length $res{$z};
}

for (1 .. 10) {
	my @keys = keys %res;
	for my $x (@keys) {
		for my $y (@keys) {
			update($x, $y, 'AA A,', ($x + $y), '+') if (($x + $y) <= 1000);
			update($x, $y, 'AAA,', ($x * $y), '*') if (($x * $y) <= 1000);
			update($x, $y, 'AA AAA,', ($x & $y), '&') if (($x & $y) <= 1000);
			update($x, $y, 'AAAA,', ($x ^ $y), '^') if (($x ^ $y) <= 1000);
			update($x, $y, 'AA AA,', abs($x - $y), '-') if (abs($x - $y) <= 1000);
		}
	}
}

$/ = undef;
open FLAG, '<scream.xml' or die "can't open flag.txt: $!\n";
$_ = <FLAG>;
close FLAG;
m!<solution case="insensitive">(.*)</solution>!;
my $flag = $1;

print $flag, "\n";


open TASK, '>static/AAA!' or die "can't open AAA!: $!\n";
print TASK "AAA AAAA AA AAAA! AAA AAAA AA AAA! AAA AAAA AA A! AAA AAAA AA AA A! ";
print TASK "AA AAA ", $res{ord $_},  "! " for split //, $flag;
print TASK "AA AAAA AA!";
close TASK;
