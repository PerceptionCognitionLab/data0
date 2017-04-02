#! usr/bin/perl

#####################################
# DMDX item file generator          #
# Richard Morey                     #
# University of Missouri - Columbia #
# 2002                              #
#####################################

#remember: DOS newlines are In text files, "\n" eq "\015\012" 
#in Unix, \n eq "\012"
#use strict;
($sec,$min,$hour,$mday,$mon,$year,$wday,$yday) = gmtime(time);
$houroffset=-5;  

#User editable sections: Parameters, Experiement stuff - scroll down to edit

#Begin parameters

#Resolutions:
#1 : 320x200
#2 : 640x480
#3 : 800x600
#4 : 1024x768
#5 : 1280x960
#6 : 1600x1200

#Color depths:
#1 : Black and white
#2 : 256 color (8bpp)
#3 : 65536 color (16 bpp)
#4 : 16.7 mill colors (24bpp)
#5 : 4.3 bill colors (32bpp)

$screenres           = 2;
$colordepth          = 2;
$fontcolor           = 255255255;  #for colors, use 255255255 for white, 
$backgroundcolor     = 000000000;  #000000000 for black (RGB triplet)
$fillcolor           = 000000000;
$fontname            = "Courier New";

$usekeyboard         = 1;
$usepiocard          = 0;
$usemouse            = 0;

$frameduration       = 0; #in ticks
$timeforresponse     = 0;   #in milliseconds (0 to disable)
$delaybetweentrials  = 15;   #in ticks (if -1, it will not specify)

$continuousrun       = 1;   #Don't wait for subject to request next item
$nofeedback          = 1;
$correctfeedbackonly = 0;
$wrongfeedbackonly   = 0;
$clearfeedback       = 0;   #<SEEMS BROKEN>Clear the screen after response
$feedbacktimedisplay = 0; 
$feedbackduration    = 0;   #in ticks


$usezillion          = 1;
$zilliononeresponse  = 1;
$zillionresponses    = 1;

$wavpan=2;		        #0:left, 1:right, 2:both
#$filename            = "exp.rtf";


#Get user input
print "File name, no extension (if none is entered, time and date will be used):\n";
chomp($filename=<STDIN>);
print "Seed [0]:\n";
chomp($randomseed=<STDIN>);
print "Group [0]:\n";
chomp($group=<STDIN>);

#####end parameters

$filename=($mon+1).".".$mday.".".($year+1900).".".($hour+$houroffset).".".$min.".".$sec if ($filename eq '');
$outfile=$filename.".out";
$filename.=".rtf";

open OUTFILE,  ">$outfile" or die "Cannot open $outfile for write :$!";
open ITEMFILE, ">$filename" or die "Cannot open $filename for write :$!";

#####Experiment stuff
@openingmessages=(
		  "This is the beginning.",
		  "This is the second beginning screen."                
					 ); #add more messages, inside quotes and comma deliniated. 
@endingmessages=(
                  "This is the end.",
		  "This is the second ending screen."							
                                    );#add more messages, inside quotes and comma deliniated

@breakmessages=(
		  "This is a break. 2 minutes",
		  "Ready?"
					);

@postpracticemessages=(
			"You have finished practice."
					);

$numberofpostpracticemessages=$#postpracticemessages+1;
$numberofbreakmessages=$#breakmessages+1;
$numberofopeningmessages=$#openingmessages+1;
$numberofendingmessages =$#endingmessages+1;

#Stimuli type
#0=text
#1=Bitmap (picture)
#2=Wavetable file (sound)

$stimulitype=1;
$randomseed=0 if ($randomseed eq '');
#$randomseed=3417;   #sets the random seed

$group=0 if ($group eq '');
#$group=0;           #for the line length experiment

$bmpjustification     = 1;   #0 = left, 1 = center, 2 = right
$movearound           = 1;   #move stimulus around screen
@movearoundpixels     = (6,6); #max pixels to move (x,y)
#####End Experiment stuff
#####Begin RTF formatting

$RTFdataline = "{\\rtf1\\ansi\\ansicpg1252\\deff0\\deflang1033{\\fonttbl{\\f0\\fnil\\fcharset0 ".$fontname.";}}\n"; 
$newparagraph= "\n\\pard\\f0\\fs20\\par\n";
$firstparagraph= "\n\\pard\\f0\\fs20";

#####end RTF formatting


	$parameterline=&createparameterline;	
	
	
	print "\nPrinting parameters to file....\n";
	print ITEMFILE $parameterline;
	print ITEMFILE $newparagraph;
	
	print "Printing opening to file....\n";
	print ITEMFILE $opening;
	
	print "Printing experiment body to file....\n";
	$experimentcounter=&createexperiment;
	print "$experimentcounter items written.\n";
	
	print "Printing ending to file....\n";
#	print "\n$numberofendingmessages\n";
	
close OUTFILE;
close ITEMFILE;
end;

###begin subroutines

sub createparameterline {
	print "Building parameter line....\n";
	$filesofar=$RTFdataline.$firstparagraph;
	
	#Output video mode and color depth
	$filesofar.="<VideoMode ".&getvideomode($screenres, $colordepth).">";
	
	$filesofar.="<cr>" if ($continuousrun);
	$filesofar.="<NoFeedBack>" if ($nofeedback);
	$filesofar.="<CorrectFeedbackOnly>" if ($correctfeedbackonly);
	$filesofar.="<WrongFeedbackOnly>" if ($wrongfeedbackonly);
	$filesofar.="<ClearFeedback>" if ($clearfeedback);
	$filesofar.="<NoFeedbackTime>" if (!$feedbacktimedisplay);
	$filesofar.="<Delay ".$delaybetweentrials.">";
	$filesofar.="<DefaultFrameDuration ".$frameduration.">";
	$filesofar.="<DefaultBackgroundColor ".$backgroundcolor.">";
	$filesofar.="<DefaultWritingColor ".$fontcolor.">";
	if ($timeforresponse){ $filesofar.="<Timeout ".$timeforresponse.">";}
		else{ $filesofar.="<NoTimeLimit>";}
	$filesofar.="<id keyboard>" if $usekeyboard;
	$filesofar.="<id mouse>" if $usemouse;
	$filesofar.="<id pio12>" if $usepiocard;
	if ($usezillion) {
		$filesofar.="<Zillion ".$zillionresponses.">";
		$filesofar.="<ZillionOneResponse>" if ($zillionresponses);
	}

	$filesofar.="\n";	
	return $filesofar;
}
			

sub getvideomode {
	my ($modenumber, $colornumber)=@_;
	my $vmline="";	

	$videomode = "320,240,240," if ($modenumber==1);	
	$videomode = "640,480,480," if ($modenumber==2);	
	$videomode = "800,600,600," if ($modenumber==3);	
	$videomode = "1024,768,768," if ($modenumber==4);	
	$videomode = "1280,960,960," if ($modenumber == 5);	
	$videomode = "1600,1200,1200," if ($modenumber == 6);	
	
	$colors = "1,0" if ($colornumber==1);
	$colors = "8,0" if ($colornumber==2);
	$colors = "16,0" if ($colornumber==3);
	$colors = "24,0" if ($colornumber==4);
        $colors = "32,0" if ($colornumber==5);

	$vmline = $videomode.$colors;
	return $vmline;

}

sub createopening {
	for ($i=0;$i<=($numberofopeningmessages-1);$i++) {
		$openinglines.="0 \"".$openingmessages[$i]."\";\n";
	}
	return $openinglines;
}

sub createending {
	for ($i=0;$i<=($numberofendingmessages-1);$i++) {
		$endinglines.="0 \"".$endingmessages[$i]."\";\n";
	}
	$endinglines.="\n\\par\n}";
	return $endinglines;
}

sub createbreak {
	my $breaklines;
	for ($l=0;$l<=($numberofbreakmessages-1);$l++) {
		$breaklines.="0 \"".$breakmessages[$l]."\";\n";
	}
	return $breaklines;
}

sub createexperiment {
	#Line length experiment
	$itemcounter=0;
	$trialspergroup=30;
	$keymapping="1234567";
	#feedback stuff: text:0 bmp:1 wav:2 correct answer:3
	$correctfeedbacktype=2;
	$wrongfeedbacktype=3;
	$correctfeedbackcontent="correct.wav";
	$wrongfeedbackcontent="";
	$framedurationfortext=75;

	$opening=&createopening;
	$ending=&createending;

	
	%stimuligroup=('A','013579B','B','02468AB');
	@addtoseed  =("1..6..7..2..3..8..5..4..2..7..8..3..4..5..6..1.",
		      "1..6..7..2..3..8..5..4..2..7..8..3..4..5..6..1.",
		      "91..6..7..2..3..8..5..4..2..7..8..3..4..5..6..1..",
		      "91..6..7..2..3..8..5..4..2..7..8..3..4..5..6..1..");
	@grouporders=("AA.BB.BB.AA.AA.BB.BB.AA.AA.BB.BB.AA.AA.BB.BB.AA",
		      "BB.AA.AA.BB.BB.AA.AA.BB.BB.AA.AA.BB.BB.AA.AA.BB",
		      "AA.AB.BB.BA.AA.AB.BB.BA.AA.AB.BB.BA.AA.AB.BB.BA.A",
		      "BB.BA.AA.AB.BB.BA.AA.AB.BB.BA.AA.AB.BB.BA.AA.AB.B");	
	@stimulisource=("0.bmp",
			"1.bmp",
			"2.bmp",
			"3.bmp",
			"4.bmp",
			"5.bmp",
			"6.bmp",
			"7.bmp",
			"8.bmp",
			"9.bmp",
			"10.bmp",
			"11.bmp");	
	
	print OUTFILE "*BaseSeed:$randomseed,Group:$group\n";
	print ITEMFILE "1 <bu 1000>;\n\n";
	
	#create feedback
	for ($i=0;$i<=(length($keymapping)-1);$i++){
		$feedbacksection="";
		$feedbacksection.=(($i+1)*10)." d2 <biw ".((($i+1)*10)+2)."> c;\n";
		$feedbacksection.=((($i+1)*10)+1)." ";
		$feedbacksection.="<fd 0>/<fd $framedurationfortext> \"$correctfeedbackcontent\"/<fd 0>;\n" if ($correctfeedbacktype==0);
		$feedbacksection.="<bmp> \"$correctfeedbackcontent\"><return>;\n" if ($correctfeedbacktype==1);
		$feedbacksection.="<wav $wavpan> \"$correctfeedbackcontent\"<return>;\n" if ($correctfeedbacktype==2);
		$feedbacksection.="<fd 0>/<fd $framedurationfortext> \"".substr($keymapping,$i,1)."\"/<fd 0><return>;\n" if ($correctfeedbacktype==3);
		$feedbacksection.=((($i+1)*10)+2)." ";
		$feedbacksection.="<fd 0>/<fd $framedurationfortext> \"$wrongfeedbackcontent\"/<fd 0><return>;\n" if ($wrongfeedbacktype==0);
		$feedbacksection.="<bmp> \"$wrongfeedbackcontent\"><return>;\n" if ($wrongfeedbacktype==1);
		$feedbacksection.="<wav $wavpan> \"$wrongfeedbackcontent\"<return>;\n" if ($wrongfeedbacktype==2);
		$feedbacksection.="<fd 0>/<fd $framedurationfortext> \"".substr($keymapping,$i,1)."\"/<fd 0><return>;\n" if ($wrongfeedbacktype==3);
		print ITEMFILE "$feedbacksection\n";
	}
	
	print ITEMFILE "1000;\n";
	print ITEMFILE $opening;
			
	for ($i=1;$i<=length($grouporders[$group]);$i++) {
		if (substr($grouporders[$group],$i-1,1) ne '.') {
			srand ($randomseed+substr($addtoseed[$group], $i-1,1)) if (substr($addtoseed[$group], $i-1,1) ne '.');
			if (substr($addtoseed[$group], $i-1,1) ne '.'){
				print OUTFILE "*CHUNK,SEED:".($randomseed+substr($addtoseed[$group], $i-1,1)).",GROUP:".substr($grouporders[$group],$i-1,1)."\n"; 
				}else{print OUTFILE "*CHUNK,GROUP:".substr($grouporders[$group],$i-1,1)."\n";}
			for ($j=1;$j<=$trialspergroup;$j++) {
				$itemcounter++;
				$itemline="";
				$correct=substr($keymapping, index($stimuligroup{substr($grouporders[$group], $i-1,1)}, &digitbase36(substr($stimuligroup{substr($grouporders[$group],$i-1,1)}, int(rand(1)*(length($stimuligroup{substr($grouporders[$group], $i-1, 1)})))+1,1))),1);
				$corrects[$correct]++;
				$itemline="+".(1000+$itemcounter)." ";	
				$itemline.="<umnr><umpr>";
				if ($stimulitype==1) {
					$itemline.="<bmp><XYJustification $bmpjustification>";
					if ($movearound) {$itemline.="<XY ".&randomdigit(-$movearoundpixels[0],$movearoundpixels[0]).",".&randomdigit(-$movearoundpixels[1],$movearoundpixels[1]).">";}				
				}
				if ($stimulitype==2) {
					$itemline.="<wav>";
				}
				for ($k=0;$k<=length($stimuligroup{substr($grouporders[$group], $i-1, 1)})-1;$k++){
					if (substr($keymapping, index($stimuligroup{substr($grouporders[$group], $i-1, 1)} ,&digitbase36(substr($stimuligroup{substr($grouporders[$group], $i-1, 1)}, $k,1))),1)==$correct){
						$itemline.="<mpr +".$correct.">";
					}else{
						$itemline.="<mnr +".substr($keymapping,index($stimuligroup{substr($grouporders[$group], $i-1,1)}, &digitbase36(substr($stimuligroup{substr($grouporders[$group], $i-1, 1)}, $k, 1))),1).">";
					}
				}
				#$itemline.="<CorrectFeedback \"Correct\"><WrongFeedback \"$correct\">";
				$itemline.=" * ";
				
				$itemline.="\"$stimulisource[&digitbase36(substr($stimuligroup{substr($grouporders[$group], $i-1,1)}, index($keymapping,$correct),1))]\" <Call -".((index($keymapping, $correct)+1)*10).">;\n";
				print ITEMFILE $itemline;
				print OUTFILE (1000+$itemcounter).",".$correct.",".&digitbase36(substr($stimuligroup{substr($grouporders[$group], $i-1,1)}, index($keymapping,$correct),1))."\n";
			}	
		}else{
			print OUTFILE "*BREAK*\n";
			print ITEMFILE &createbreak;
		}	
		
					


	}
	print ITEMFILE $ending;
	print "Corrects: @corrects\n";
	return $itemcounter;

}

sub digitbase36{
	
	($digit)=@_;
	my $mapdigits = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	return index($mapdigits,$digit);
}

sub randomdigit{
	my $max;
	my $min;
	($min, $max)=@_;
	return (int(rand(1)*($max-$min+1))+$min);
	
}