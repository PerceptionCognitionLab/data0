#! usr/bin/perl

$subjectline="Subjects incorp";
$itemline="Item";

($filename)=@ARGV;

$dosnewline=chr(13).chr(10);
$outfile=$filename.".out";
$zilfile=$filename.".zil";
$datfile=$filename.".dat";

open OUTFILE, $outfile or die "Could not open $outfile for read.";
open ZILFILE, $zilfile or die "Could not open $zilfile for read.";
open DATFILE, ">$datfile" or die "Could not open $datfile for write.";

while (<OUTFILE>){
#	$_=~s/$dosnewline//g;
	chomp($_);
	if (substr($_,0,6) eq '*BREAK'){
		$breaks[$itemnumber]="*\n";}
	if (substr($_,0,6) eq '*CHUNK'){
		if ($_=~/SEED/){
			($garbage,$seed,$group)=split(/,/,$_);
		}else{($garbage,$group)=split(/,/,$_);}
		($garbage,$trialgroup)=split(/:/,$group);
		($garbage,$groupseed)=split(/:/,$seed);
	}
	if (substr($_,0,1) ne '*'){
		($itemnumber, $correctanswer, $actualanswer)=split(/,/ ,$_);	
		if (!$foundfirstitem){$firstitem=$itemnumber;$foundfirstitem=1;}
		$itemnumbers[$itemnumber]="$correctanswer,$actualanswer,$trialgroup,$groupseed";
		$totalitems++;
	}
}

while (<ZILFILE>){
	$_=~s/$dosnewline//g;
	if ($_=~/$subjectline/){
		($garbage,$subjectsinc)=split(/:/,$_);
		die "More than one subjects are included in $zilfile. Quitting...\n" if (($subjectsinc+1)>2);
	}
	if ($_=~/$itemline/){
		@items=split(/ /,$_);
		$itemnumber=$items[1];
		$itemnumber=~s/,//g;
		$_=(<ZILFILE>);
		$_=~s/$dosnewline//g;
		$_=substr($_,1);
		($rtime,$subjectanswer)=split(/,/,$_);
		$subjectanswer=~s/[+-]//g;
		$itemnumbers[$itemnumber].=",$subjectanswer,$rtime";
		($correctanswer,$actualanswer,$trialgroup,$groupseed,$subjectanswer,$rtime)=split(/,/,$itemnumbers[$itemnumber]);
		if ($correctanswer==$subjectanswer){$iscorrect=1;}else{$iscorrect=0;}
		$rtime=sprintf("%09f", $rtime);
		$itemnumbers[$itemnumber]="$subjectanswer $correctanswer $iscorrect $actualanswer $trialgroup $groupseed $rtime";
		}


}

#print DATFILE "*ITEM#,SUBANS,CORANS,ISCORR,ACTANS,SGRP,SEED,RTIME\n";
for ($i=$firstitem;$i<=$#itemnumbers;$i++){
	$l++;
	print DATFILE sprintf("%04d",($i-$firstitem+1))." $itemnumbers[$i]\n";
	print DATFILE $breaks[$i];
}
print "Output $l items to $datfile.\n\n";



close OUTFILE;
close ZILFILE;
close DATFILE;
