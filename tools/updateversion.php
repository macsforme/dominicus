<?php

// ensure argument is specified
if($argc < 2)
	 exit("Usage: php tools/updateversion.php <version>\n");

// ensure argument is in correct format
$matches = Array();
if(! preg_match("/^(\d+)(d|a|b|s)(\d*)$/", $argv[1], $matches) || (($matches[2] == "a" || $matches[2] == "b") && $matches[3] == "") || (($matches[2] == "d" || $matches[2] == "s") && $matches[3] != ""))
     exit("Version number must be in format: #d|(a#)|(b#)|s\n");

// ensure we are in correct directory
if(! is_file("src/core/GameSystem.h") || ! is_file("COPYING"))
     exit("Please run this script from the main source directory\n");

// ensure no uncommitted changes to any files
if(exec("git status") != "nothing to commit, working directory clean")
     exit("Source tree has uncommitted changes\n");

$gameSystemFile = file_get_contents("src/core/GameSystem.h");

// substitute main version number into GameSystem.h
$replacementCount = 0;
$gameSystemFile = preg_replace("/^\#define\sPROGRAM_VERSION\s\"\d+\"$/m", "#define PROGRAM_VERSION \"".$matches[1]."\"", $gameSystemFile, -1, $replacementCount);
if(! $replacementCount)
     exit("Unable to substitute primary version number into source file\n");

// substitute build status and iteration into GameSystem.h
$replacementCount = 0;
$buildStatus = "";
if($matches[2] == "d") $buildStatus = "DEVEL";
else if($matches[2] == "s") $buildStatus = "STABLE";
else if($matches[2] == "a") $buildStatus = "ALPHA".$matches[3];
else $buildStatus = "BETA".$matches[3];
$gameSystemFile = preg_replace("/^\#define\sPROGRAM_BUILDSTRING\s\"\w+\"$/m", "#define PROGRAM_BUILDSTRING \"".$buildStatus."\"", $gameSystemFile, -1, $replacementCount);
if(! $replacementCount)
     exit("Unable to substitute build status and iteration into source file\n");

// write out the file
if(! file_put_contents("src/core/GameSystem.h", $gameSystemFile))
     exit("Unable to write updated source file\n");

$macPlistFile = file_get_contents("crucible island.plist");

// replace mac plist short version number
$replacementCount = 0;
$macPlistFile = preg_replace("/\<key\>CFBundleShortVersionString\<\/key\>(\s*)\<string\>[\d|\.]+\<\/string\>/", "<key>CFBundleShortVersionString</key>$1<string>".$matches[1].".0.0</string>", $macPlistFile, -1, $replacementCount);
if(! $replacementCount)
     exit("Unable to substitute short version number into mac plist file\n");

// replace mac plist whole version number
$replacementCount = 0;
$macPlistFile = preg_replace("/\<key\>CFBundleVersion\<\/key\>(\s*)\<string\>\w+\<\/string\>/", "<key>CFBundleVersion</key>$1<string>".$matches[0]."</string>", $macPlistFile, -1, $replacementCount);
if(! $replacementCount)
     exit("Unable to substitute whole version number into mac plist file\n");

// write out the file
if(! file_put_contents("crucible island.plist", $macPlistFile))
	exit("Unable to write updated mac plist file\n");

$copyingFile = file_get_contents("COPYING");

// replace main copyright notice with current year
$replacementCount = 0;
date_default_timezone_set('UTC');
$copyingFile = preg_replace("/^Copyright\s\(c\)\s(\d+)\-\d+\,\sJoshua\sBodine$/m", "Copyright (c) $1-".date("Y", time()).", Joshua Bodine", $copyingFile, -1, $replacementCount);
if(! $replacementCount)
     exit("Unable to substitute current year into copyrights file\n");

// write out the file
if(! file_put_contents("COPYING", $copyingFile))
	exit("Unable to write updated copyrights file\n");

// prompt to commit the updates and the tag to git
$prettyVersion = "";
if($matches[2] == "d") $prettyVersion = $matches[1]." Development";
else if($matches[2] == "a") $prettyVersion = $matches[1]." Alpha ".$matches[3];
else if($matches[2] == "b") $prettyVersion = $matches[1]." Beta ".$matches[3];
else $prettyVersion = $matches[1]." Stable";
echo "Source code version updated to ".$prettyVersion."\n";
if(exec("git status src/core/GameSystem.h crucible\ island.plist COPYING") == "nothing to commit, working directory clean") {
	echo "However, no changes resulted from this update.\n";
} else {
	$commitCommand = "git commit src/core/GameSystem.h crucible\ island.plist COPYING -m \"Version updated to ".$prettyVersion.".\"\n";
	$tagCommand = "git tag -a v".$matches[0]." -m \"Version ".$prettyVersion;
	if($matches[2] == "s") {
		echo "This is a stable commit/tag. Please enter a list of changes to be logged.\n";
		echo "Capitalize the first letter of each line, and omit the period at the end of each line.\n";
		echo "When all lines have been entered, enter a blank line to proceed:\n";
		echo "======================================================================================\n";
		$logLines = Array();
		do {
			array_push($logLines, trim(fgets(STDIN)));
		} while(count($logLines) > 0 && $logLines[count($logLines) - 1] != "");
		if(count($logLines) > 0 && $logLines[0] != "") {
			$tagCommand .= "\n\n";
			for($i = 0; $i < count($logLines) - 1; ++$i)
				$tagCommand .= "* ".$logLines[$i].($i < count($logLines) - 2 ? "\n" : "");
		}
	}
	$tagCommand .= "\"\n";
	echo "The following command".($matches[2] != "d" ? "s" : "")." should be executed to commit ".($matches[2] != "d" ? "and tag " : "")."the changes:\n";
	echo "\n";
	echo $commitCommand;
	if($matches[2] != "d")
		echo $tagCommand;
	echo "\n";
	echo "Execute? [yes/NO]: ";
	$execution = trim(fgets(STDIN));
	if(strtolower($execution) == "yes" || strtolower($execution) == "y") {
		exec($commitCommand);
		if($matches[2] != "d")
			exec($tagCommand);
		echo "Command".($matches[2] != "d" ? "s" : "")." executed. Repository is ready to be pushed to origin and for binaries to be built.\n";
	} else {
		echo "Aborted. Please execute commit ".($matches[2] != "d" ? "and tag " : "")."command".($matches[2] != "d" ? "s" : "")." manually.\n";
	}
}

?>
