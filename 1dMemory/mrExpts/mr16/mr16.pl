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
$usetimedbreaks      = 1;
$ticklength          = 13.3; #in milliseconds
$breaklength         = 120;  #in seconds
$warn                = 0;    #0, no; 1, yes


$readybeep           = "ready.wav";
$wavpan=2;		        #0:left, 1:right, 2:both
#$filename            = "exp.rtf";
$experimentname      = "mr16";
$blockedrand=1;


#Get user input
#print "File name, no extension (if none is entered, time and date will be used):\n";
#chomp($filename=<STDIN>);
print "Subject number:\n";
chomp($subjectnumber=<STDIN>);
print "Seed [0]:\n";
chomp($randomseed=<STDIN>);
print "Condition [0]:\n";
chomp($condition=<STDIN>);

#print "Group [0]:\n";
#chomp($group=<STDIN>);

#####end parameters
$filename=$experimentname."s".$subjectnumber;

#$filename=($mon+1).".".$mday.".".($year+1900).".".($hour+$houroffset).".".$min.".".$sec if ($filename eq '');

$outfile=$filename.".out";
$filename.=".rtf";

open OUTFILE,  ">$outfile" or die "Cannot open $outfile for write :$!";
open ITEMFILE, ">$filename" or die "Cannot open $filename for write :$!";
print "Outfile: $outfile\nItemfile: $filename\n";


#####Experiment stuff
@openingmessages=(
		  "In this experiment, you will be shown lines.",
		  "Each line is associated with a number, 1-6.",
		  "Larger lines are larger numbers, in order.",
		  "When you see the line press the correct number.",
		  "You will have to guess the first few.",
		  "If you get one right, there will be a beep.",
		  "If you get one wrong, it will correct you.",
		  "There will be several breaks.",
		  "In breaks, complete the given worksheets.",
		  "A warning will sound when the break is over.",
		  "Good luck! If you have any questions, ask now.",
		  "Ready?"
		  		 ); #add more messages, inside quotes and comma deliniated. 

if ($group>3){
@openingmessages=(
                  "In this experiment you'll see three-digit numbers.",
                  "Each one is associated with a response 1-8.",
                  "Larger numbers go with larger responses.",
                  "Smaller numbers go with smaller responses.",
                  "You will have to guess the first few.",
                  "If you get one right, you will hear a beep.",
                  "If you get one wrong, it will correct you.",
                  "There will be several one-minute breaks.",
                  "In breaks, complete the given worksheets.",
                  "A warning will sound when the break is over.",
                  "Good luck! If you have any questions, ask now.",
                  "Ready?"
                                 ); #add more messages, inside quotes and comma deliniated.
}

@endingmessages=(
                  "This is the end. Thanks for participating.",
		  "END."							
                                    );#add more messages, inside quotes and comma deliniated

@breakmessages=(
		  "Break - work on worksheets.",
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
	my $warn;
	($warn)=@_;
	my $breaklines;
	$numbreaks++;
	for ($l=0;$l<=($numberofbreakmessages-1);$l++) {
		#srand (time ^ $$ ^ unpack "%L*", `ps axww | gzip`); 
		my $randomnumber=(int(rand(1)*51)+40)*10;
		my $breakmessagetemp=$breakmessages[$l];
		$breakmessagetemp=~s/RANDOMNUMBER/$randomnumber/gs;
		if ($usetimedbreaks){
		if (!$l) {$breaklines.=($numbreaks+10000)." / %".int($breaklength*(1000/$ticklength))." \"".$breakmessagetemp."\"/<wav $wavpan> \"$readybeep\"/;\n";}else{
		$breaklines.="0 \"".$breakmessagetemp."\";\n";}}else{
		$breaklines.="0 \"".$breakmessagetemp."\";\n";}
	}
	return $breaklines;
}

sub createexperiment {
	#Line length experiment
	$itemcounter=0;
	$trialspergroup=42;
	#feedback stuff: text:0 bmp:1 wav:2 correct answer:3
	$correctfeedbacktype=2;
	$wrongfeedbacktype=3;
	$correctfeedbackcontent="correct.wav";
	$wrongfeedbackcontent="";
	$framedurationfortext=75;

	$opening=&createopening;
	$ending=&createending;

	$keymapping="123456";
	%stimuligroup=('A','012345','B','6789AB');
	%groupnames=('A','SMALL','B','LARGE');

	@addtoseed  =("0........................................................................",
		      "0........................................................................",
		      "91..6..7..2..3..8..5..4..2..7..8..3..4..5..6..1..0..2..3..9..5..4..0..7..",
		      "91..6..7..2..3..8..5..4..2..7..8..3..4..5..6..1..0..2..3..9..5..4..0..7..");
	@grouporders=("A.A.B.B.A.A.B.B.A.A.B.B.A.A.B.B.A.A",
		      "B.B.A.A.B.B.A.A.B.B.A.A.B.B.A.A.B.B",
		      "AA.AA.BB.BB.AA.AA.BB.BB.AA.AA.BB.BB.AA.AA.BB.BB.AA.AA.BB.BB",
		      "BB.BB.AA.AA.BB.BB.AA.AA.BB.BB.AA.AA.BB.BB.AA.AA.BB.BB.AA.AA");	
	
	if (!$condition){
		$group=$subjectnumber%2;
		$warn=0;
			
	}else{
		$group=($subjectnumber%2);
		$warn=0;
	}
	



	if ($warn){
	    srand(2354+int($subjectnumber/4));
		$groupstring=$grouporders[$group%4];				
print "$groupstring\n";		
		$counterbalance=(int($subjectnumber/2)%2);
		$breakstring='.';
		$warnstring='|';
#	    @groupstringarray=split(/\Q.\E/, $groupstring);
@groupstringarray=split(//,$groupstring);		
	    foreach $temp (@groupstringarray){push(@temp, split(//, $temp));}
		@groupstringarray=@temp;			      
print "@temp\n";
	    for ($ll=1;$ll<=$#groupstringarray;$ll++) {		
			
		     if (
					(substr($groupstringarray[$ll],0,1) ne substr($groupstringarray[$ll-1], -1,1)) &&
					(
					(index("|.", $groupstringarray[$ll]) == -1) && 
					(index("|.", $groupstringarray[$ll-1]) == -1) 
					)
				
					){
				$numswitches++;
				print $groupstringarray[$ll-2].$groupstringarray[$ll-1].$groupstringarray[$ll]."\n";
		   	        print index("|.", $groupstringarray[$ll])."\n";
                                print index("|.", $groupstringarray[$ll-1])."\n";
			}
			
			if (substr($groupstringarray[$ll-1],0,1) eq '.'){
				$numswitches++ if (substr($groupstringarray[$ll-2],0,1) ne substr($groupstringarray[$ll],0,1));
				}

		}		
		print "NS: $numswitches\n";
		$numwarns=int($numswitches/2);
		$switchstring=(' ' x ($numswitches-$numwarns)).('|' x $numwarns);
		@switcharray=split(//,$switchstring);
		
            push(@list, splice(@switcharray, rand @switcharray, 1)) while @switcharray;
		
	    if ($counterbalance){push(@list2, pop(@list)) while @list;}else{@list2=@list;}
		$switchstring=join('',@list2);                                              
	    print "CB: $counterbalance\n";
	    print "SS: $switchstring\n";
	    for($ll=1;$ll<=$#groupstringarray;$ll++){

			   if (   
                                        (substr($groupstringarray[$ll],0,1) ne substr($groupstringarray[$ll-1], -1,1)) &&
                                        (
                                        (index("|.", $groupstringarray[$ll]) == -1) &&
                                        (index("|.", $groupstringarray[$ll-1]) == -1)
                                        )

                                        ){   
				$groupstringarray[$ll]=substr($switchstring,$counterwarn,1).$groupstringarray[$ll] if (substr($switchstring,$counterwarn,1) eq '|');
                        	$counterwarn++;
				}
                        
                        if (substr($groupstringarray[$ll-1],0,1) eq '.'){
                                if (substr($groupstringarray[$ll-2],-1,1) ne substr($groupstringarray[$ll],0,1)){
					$groupstringarray[$ll]=substr($switchstring,$counterwarn,1).$groupstringarray[$ll] if (substr($switchstring,$counterwarn,1) eq '|'); 
	        			$counterwarn++;
                               		print "Not supposed to be here: $groupstringarray[$ll-2] $groupstringarray[$ll-1] $groupstringarray[$ll]\n";
				}
				}
             


			
		    
                }
		$grouporders[$group]=join("",@groupstringarray);
		print "@groupstringarray\n";
		
	}
#	14  21  31  44  59  79 102 130 164 202 248 300
	if ($group<4){
		@stimulisource=(
			"14x1-255255255.bmp",
			"21x1-255255255.bmp",
			"31x1-255255255.bmp",
			"44x1-255255255.bmp",
			"59x1-255255255.bmp",
			"79x1-255255255.bmp",
			"102x1-255255255.bmp",
			"130x1-255255255.bmp",
			"164x1-255255255.bmp",
			"202x1-255255255.bmp",
			"248x1-255255255.bmp",
			"300x1-255255255.bmp");	
	} else{
		@stimulisource=("50",
                        "56",
                        "63",
                        "71",
                        "79",
                        "89",
                        "99", 
                        "111",
                        "125",
                        "140",
                        "157",
                        "174",
                        "191",
                        "208",
                        "225", 
                        "242");
		$stimulitype=0;
		$groupactual=$group;
		$group=$group%4;
	    }
	
	
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
		$nextgroupname=$groupnames{substr($grouporders[$group],$i,1)};
		if ((substr($grouporders[$group],$i-1,1) ne '.')&&(substr($grouporders[$group],$i-1,1) ne '|')) {
			$nextgroupname=substr($grouporders[$group],$i,1);
			srand ($randomseed+substr($addtoseed[$group], $i-1,1)) if (substr($addtoseed[$group], $i-1,1) ne '.');
			if (substr($addtoseed[$group], $i-1,1) ne '.'){
				print OUTFILE "*CHUNK,SEED:".($randomseed+substr($addtoseed[$group], $i-1,1)).",GROUP:".substr($grouporders[$group],$i-1,1)."\n"; 
				}else{print OUTFILE "*CHUNK,GROUP:".substr($grouporders[$group],$i-1,1)."\n";}
			
			@correctans=();
			if ($blockedrand){
			    
			    for ($j=0;$j<length($stimuligroup{substr($grouporders[$group], $i-1,1)});$j++){
				push(@correctans, (substr($stimuligroup{substr($grouporders[$group], $i-1,1)},$j,1)) x int($trialspergroup/length($stimuligroup{substr($grouporders[$group], $i-1,1)})));
			    }
			    $j=0;
			    while ($trialspergroup-($#correctans+1)){
				push(@correctans, substr($stimuligroup{substr($grouporders[$group], $i-1,1)},$j++,1));
			    }
			    @list2=();
			    push(@list2, splice(@correctans, rand @correctans, 1)) while @correctans;
			    @correctans=@list2;
			    print "Here: @correctans\n";
			}else{
			    for ($j=1;$j<=$trialspergroup;$j++){
				$kk=rand(1);
				push(@correctans, substr($stimuligroup{substr($grouporders[$group],$i-1,1)}, int($kk*(length($stimuligroup{substr($grouporders[$group], $i-1, 1)}))) ,1));
			        
				print "Here: @correctans\n";
			    }
			}    
#			print "@correctans\n";
			$tt=0;
			for ($j=1;$j<=$trialspergroup;$j++) {
				$itemcounter++;
				$itemline="";
#				$correct=substr($keymapping, index($stimuligroup{substr($grouporders[$group], $i-1,1)}, substr($stimuligroup{substr($grouporders[$group],$i-1,1)}, int(rand(1)*(length($stimuligroup{substr($grouporders[$group], $i-1, 1)})))+1,1)),1);				
				$correct=substr($keymapping, index($stimuligroup{substr($grouporders[$group], $i-1,1)},$correctans[$tt++]),1);

				$corrects[$correct]++;
				$itemline="+".(1000+$itemcounter)." ";	
				$itemline.="<umnr><umpr>";
				if ($stimulitype==1) {
					$itemline.="<bmp><XYJustification $bmpjustification>";
					if ($movearound) {$itemline.="<XY ".(320+&randomdigit(-$movearoundpixels[0],$movearoundpixels[0])).",".(240+&randomdigit(-$movearoundpixels[1],$movearoundpixels[1])).">";}				
				}
				if ($stimulitype==2) {
					$itemline.="<wav>";
				}
				for ($k=0;$k<=length($stimuligroup{substr($grouporders[$group], $i-1, 1)})-1;$k++){
					if (substr($keymapping, $k, 1) eq $correct){
					#if (substr($keymapping, index($stimuligroup{substr($grouporders[$group], $i-1, 1)} ,&digitbase36(substr($stimuligroup{substr($grouporders[$group], $i-1, 1)}, $k,1))),1) eq $correct){
						$itemline.="<mpr +".$correct.">";
					}else{
						$itemline.="<mnr +".substr($keymapping, $k, 1).">";
						#$itemline.="<mnr +".substr($keymapping,index($stimuligroup{substr($grouporders[$group], $i-1,1)}, &digitbase36(substr($stimuligroup{substr($grouporders[$group], $i-1, 1)}, $k, 1))),1).">";
					}
				}
				#$itemline.="<CorrectFeedback \"Correct\"><WrongFeedback \"$correct\">";
				$itemline.=" * ";
				
				$itemline.="\"$stimulisource[&digitbase36(substr($stimuligroup{substr($grouporders[$group], $i-1,1)}, index($keymapping,$correct),1))]\" <Call -".((index($keymapping, $correct)+1)*10).">;\n";
				print ITEMFILE $itemline;
				print OUTFILE (1000+$itemcounter).",".$correct.",".&digitbase36(substr($stimuligroup{substr($grouporders[$group], $i-1,1)}, index($keymapping,$correct),1))."\n";
			}	
		}else{
			if (substr($grouporders[$group],$i-1,1) eq '.'){
			print OUTFILE "*BREAK*\n";
			print ITEMFILE &createbreak;
			}
			if (substr($grouporders[$group],$i-1,1) eq '|'){
                        print OUTFILE "*WARNING*\n";
                        print ITEMFILE &createwarn;
                        } 

		}	
		
					


	}
	print ITEMFILE $ending;
	print "Corrects: @corrects\n";
	return $itemcounter;

}

sub digitbase36{
	my $digit;
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



sub createwarn{		
	$warn="0 \"Next set: ".$nextgroupname." - Press space.\";\n";
	return $warn;
}














