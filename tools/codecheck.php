<?php

// letters before numbers comparison function
function customSort($a, $b) {
	if($a == $b)
		return 0;

	if(strtolower($a) == strtolower($b))
		return $a > $b ? 1 : -1;

	if(is_numeric($a) && is_numeric($b))
		return $a > $b ? 1 : -1;

	if(is_numeric($a) && ! is_numeric($b))
		return 1;
	if(! is_numeric($a) && is_numeric($b))
		return -1;

	for($i = 0; $i < strlen(strlen($a) >= strlen($b) ? $a : $b); ++$i) {
		if($i >= strlen($a))
			return -1;
		else if($i >= strlen($b))
			return 1;

		$subA = substr($a, $i, 1);
		$subB = substr($b, $i, 1);
		if($subA == $subB)
			continue;
		else if(is_numeric($subA) && ! is_numeric($subB))
			return 1;
		else if(!is_numeric($subA) && is_numeric($subB))
			return -1;
		else if(strtolower($subA) == strtolower($subB))
			return $subA > $subB ? 1 : -1;
		else
			return strtolower($subA) > strtolower($subB) ? 1 : -1;
	}
}

// define settings
$projectName = "Crucible Island";
$headerSearchDirectory = "src/";
$fileExtensions = Array(".h", ".cpp" /*, ".glsl"*/);
$ignoreFiles = Array(".git/", "src/platform/macosx/SDLMain.h", "src/platform/macosx/SDLMain.m");
$headerSectionFileWhitelist = Array("src/graphics/text/FontManager.h", "src/platform/OpenGLHeaders.h");
$fileLineWhitelist = Array(
	"src/graphics/3dgraphics/FortressRenderer.cpp:181",
	"src/graphics/3dgraphics/MissileRenderer.cpp:117",
	"src/graphics/3dgraphics/ShellRenderer.cpp:87",
	"src/graphics/3dgraphics/ShipRenderer.cpp:115",
	"src/graphics/UILayoutAuthority.h:27",
	"src/graphics/UILayoutAuthority.h:28",
	"src/graphics/UILayoutAuthority.h:46",
	"src/graphics/UILayoutAuthority.h:47",
	"src/input/Mouse.h:24",
	"src/input/Mouse.h:25",
	"src/input/Mouse.h:59",
	"src/input/Mouse.h:60",
	"src/state/GameState.cpp:644"
);
$systemHeaders = Array(
	"string" => Array("std::string"),
	"vector" => Array("std::vector"),
	"map" => Array("std::map"),
	"utility" => Array("pair"),
	"cstring" => Array("memcpy()", "memset()"), // also string.h
	"cmath" => Array("sin()", "cos()", "tan()", /* "log()" this has a name conflict,*/ "log2()", "pow()", "sqrt()", "floor()", "ceil()", "mod()", "std::isnan()"), // also math.h
	"sstream" => Array("std::stringstream"),
	"stdint.h" => Array("int8_t", "uint8_t", "int16_t", "uint16_t", "int32_t", "uint32_t", "int64_t", "uint64_t"), // also cstdint, but can't use that without libc++, which doesn't support 10.6
	"fstream" => Array("open()", "close()"),
	"cstdlib" => Array("rand()", "atof()", "atoi()", "atol()", "malloc()", "free()", "exit()", "NULL", "size_t"),
	"cstdio" => Array("sprintf()"), // also stdio.h
	"iostream" => Array("std::cout"),
	"SDL.h" => Array("SDL_OpenAudio()", "SDL_LockAudio()", "SDL_SetVideoMode()", "SDL_WarpMouse()", "SDL_EnableUNICODE()", "SDL_GetVideoInfo()", "SDL_PollEvent()", "SDL_Event", "SDL_Init()", "SDL_AudioSpec", "SDLKey"),
	"ft2build.h" => Array("FT_FREETYPE_H", "FT_Library", "FT_Face", "FT_Set_Char_Size()", "FT_UInt", "FT_Load_Glyph()", "FT_Render_Glyph()", "FT_Init_FreeType()", "FT_New_Face()")
);
$localHeaders = Array(
	$headerSearchDirectory."platform/OpenGLHeaders.h" => Array("glBindBuffer()", "glGenBuffers()", "glBufferData()", "glEnable()", "glDisable()", "glBlendFunc()", "glUseProgram()", "glShaderSource()", "glCompileShader()", "glGetString()", "glClear()", "GLfloat", "GLint", "GLuint", "GLushort", "GLchar")
);

// build list of source files
$files = "";
foreach($fileExtensions as $extension)
	$files .= `find . -name *$extension`;

$files = trim($files);
$fileArray = preg_split("/\n/", $files);
sort($fileArray);

// exclude ignored directories
$fileArrayWithoutIgnored = Array();
for($i = 0; $i < count($fileArray); ++$i) {
	$isIgnored = FALSE;

	for($p = 0; $p < count($ignoreFiles); ++$p) {
		if(substr($ignoreFiles[$p], strlen($ignoreFiles[p]) - 1) == "/")
			if(substr($fileArray[$i], 0, strlen("./".$ignoreFiles[$p])) == "./".$ignoreFiles[$p])
				$isIgnored = TRUE;

		if($fileArray[$i] == "./".$ignoreFiles[$p])
			$isIgnored = TRUE;
	}

	if(! $isIgnored)
		array_push($fileArrayWithoutIgnored, $fileArray[$i]);
}
$fileArray = $fileArrayWithoutIgnored;

// create dictionary of local types from headers
for($i = 0; $i < count($fileArray); ++$i) {
	if(preg_match("/\.h$/", $fileArray[$i])) {
		$fileContents = file_get_contents($fileArray[$i]);
		$fileTypes = Array();

		$matches = Array();

		// function definitions or straight implementations
		if(preg_match_all("/\n(?:\w+\s)+(\w+)\((?:\w+\*?\s)+\*?\w+(?:\[\d*\])?(?:\,\s?(?:\w+\*?\s)+\w+(?:\[\d*\])?)*\)\s?(?:;|\{)/", $fileContents, $matches))
			for($p = 0; $p < count($matches[1]); ++$p)
				if(! in_array($matches[1][$p]."()", $fileTypes))
					array_push($fileTypes, $matches[1][$p]."()");

		// class names
		if(preg_match_all("/class\s([\w|\d]+)(\s:\s(public|protected|private)\s[\w|\d]+)?\s?\{/", $fileContents, $matches))
			for($p = 0; $p < count($matches[1]); ++$p)
				array_push($fileTypes, $matches[1][$p]);

		// struct names within classes
		if(preg_match_all("/class\s(\w+)(?:\s:\s(?:public|private|protected)\s\w+)?\s?\{.+?\n\};/s", $fileContents, $matches))
			for($p = 0; $p < count($matches[0]); ++$p)
				if(preg_match_all("/struct\s(\w+)\s\{/", $matches[$p][0], $matches2))
					for($j = 0; $j < count($matches2[0]); ++$j)
						array_push($fileTypes, $matches[1][$p]."::".$matches2[1][$j]);

		// struct names outside of classes; unimplemented, we don't have them

		// typedefs within classes
		if(preg_match_all("/class\s(\w+)(?:\s:\s(?:public|private|protected)\s\w+)?\s?\{.+?\n\};/s", $fileContents, $matches))
			for($p = 0; $p < count($matches[0]); ++$p)
				if(preg_match_all("/typedef\s[^;]+\s(\w+);/", $matches[$p][0], $matches2))
					for($j = 0; $j < count($matches2[0]); ++$j)
						array_push($fileTypes, $matches[1][$p]."::".$matches2[1][$j]);

		// typedefs outside of classes
		if(preg_match_all("/\ntypedef\s[^;]+\s(\w+);/", $fileContents, $matches))
			for($p = 0; $p < count($matches[1]); ++$p)
				array_push($fileTypes, $matches[1][$p]);

		// enum names within classes
		if(preg_match_all("/class\s(\w+)(?:\s:\s(?:public|private|protected)\s\w+)?\s?\{.+?\n\};/s", $fileContents, $matches)) {
			for($p = 0; $p < count($matches[0]); ++$p) {
				if(preg_match_all("/enum(?:\s(\w+))?\s?\{((?:\w|\s|\,)+)\}(?:\s?\w+)?;/", $matches[$p][0], $matches2)) {
					for($j = 0; $j < count($matches2[0]); ++$j) {
						if(strlen(trim($matches2[1][$j])))
							array_push($fileTypes, $matches[1][$p]."::".$matches2[1][$j]);
						$enums = preg_split("/\,/", $matches2[2][$j]);
						for($k = 0; $k < count($enums); ++$k)
							array_push($fileTypes, $matches[1][$p]."::".trim($enums[$k]));
					}
				}
			}
		}

		// enum names outside of classes; unimplemented, we don't have them

		sort($fileTypes);

		if(count($fileTypes))
			if(! array_key_exists(preg_replace("/^\.\//", "", $fileArray[$i]), $localHeaders))
				$localHeaders[preg_replace("/^\.\//", "", $fileArray[$i])] = $fileTypes;
	}
}

// build reverse lookup dictionary for types
$headersByType = Array();
foreach($systemHeaders as $header => $types)
	foreach($types as $type)
		if(array_key_exists($type, $headersByType))
			exit("ERROR: Type ".$type." has duplicate definitions in ".$headersByType[$type]." and ".$header."\n");
		else
			$headersByType[$type] = "<".$header.">";

foreach($localHeaders as $header => $types)
	foreach($types as $type)
		if(array_key_exists($type, $headersByType))
			exit("ERROR: Type ".$type." has duplicate definitions in ".$headersByType[$type]." and ".$header."\n");
		else
			$headersByType[$type] = "\"".$header."\"";

ksort($headersByType);

// process files
$totalCodeLineCount = 0;
$totalCommentLineCount = 0;
$totalWhiteSpaceLineCount = 0;

foreach($fileArray as $file) {
	$fileContentsArray = file($file);

	// check for proper filename header
	if(preg_match("/(\w+\.\w+)$/", $file, $matches)) {
		$filename = $matches[1];
		if(! preg_match("/(\w+\.\w+)\n$/", $fileContentsArray[0], $matches) || ! preg_match("/^\/\/\s(".$projectName.")\n$/", $fileContentsArray[1], $matches2) || $matches[1] != $filename)
			echo $file." does not have proper filename header.\n";//.$fileContentsArray[0].$fileContentsArray[1].$matches[1].$filename."\n";
	}

	// check for proper header inclusion (including order, format, all required, and only required)
	if(! in_array(substr($file, 2), $headerSectionFileWhitelist)) {
		$includes = Array();
		$firstIncludeLine = 0;
		$lastIncludeLine = 0;
		for($i = 0; $i < count($fileContentsArray); ++$i) {
			if(preg_match("/^\s*(\#include\s(?:\<|\")\S+(?:\>|\"))\s*$/", $fileContentsArray[$i], $matches)) {
				array_push($includes, $matches[1]);
				if(count($includes) == 1)
					$firstIncludeLine = $i;
				$lastIncludeLine = $i;
			}
		}
		$improperIncludes = FALSE;
		if(count($includes) > 0) {
			// check for an associated header in the right spot
			$hasAssocHeader = FALSE;
			$assocHeader = "";
			if(! preg_match("/\\.h\s*$/", $file) && preg_match("/^\.\/".preg_replace("/\//", "\/", $headerSearchDirectory)."(.*)\.\w+$/", $file, $matches))
				$assocHeader = $matches[1].".h";

			if($assocHeader) {
				for($i = 0; $i < count($includes); ++$i) {
					if($includes[$i] == "#include \"".$assocHeader."\"") {
						if($i != 0) {
							echo $file." has associated header in wrong place.\n";
							$improperIncludes = TRUE;
						}
						$hasAssocHeader = TRUE;
						array_splice($includes, $i, 1);
						break;
					}
				}
			}

			// check for system/library headers before local headers
			$localHeadersBeginning = count($includes);
			for($i = 0; $i < count($includes); ++$i) {
				if(preg_match("/^\#include\s\"/", $includes[$i])) {
					$localHeadersBeginning = $i;
					break;
				}
			}
			for($i = $localHeadersBeginning + 1; $i < count($includes) && $localHeadersBeginning < count($includes); ++$i) {
				if(preg_match("/^\#include\s\</", $includes[$i])) {
					echo $file." does not list all system/library headers before local headers.\n";
					$improperIncludes = TRUE;
					break;
				}
			}

			// check for alphabetical listing of headers
			$fileSystemHeaders = Array();
			$fileLocalHeaders = Array();
			for($i = 0; $i < count($includes); ++$i)
				if(preg_match("/\#include\s\</", $includes[$i]))
					array_push($fileSystemHeaders, $includes[$i]);
				else
					array_push($fileLocalHeaders, $includes[$i]);
			$unsortedFileSystemHeaders = $fileSystemHeaders;
			$unsortedFileLocalHeaders = $fileLocalHeaders;
			usort($fileSystemHeaders, "customSort");
			usort($fileLocalHeaders, "customSort");

			for($i = 0; $i < count($fileSystemHeaders); ++$i) {
				if($fileSystemHeaders[$i] != $unsortedFileSystemHeaders[$i]) {
					echo $file." does not list system/library headers in alphabetical order.\n";
					$improperIncludes = TRUE;
					break;
				}
			}
			for($i = 0; $i < count($fileLocalHeaders); ++$i) {
				if($fileLocalHeaders[$i] != $unsortedFileLocalHeaders[$i]) {
					echo $file." does not list local headers in alphabetical order.\n";
					$improperIncludes = TRUE;
					break;
				}
			}

			// check that all required headers (and only required headers) are included
			for($i = 0; $i < count($fileSystemHeaders); ++$i)
				$fileSystemHeaders[$i] = preg_replace("/^\#include\s\<(.*)\>\s*$/", "<$1>", $fileSystemHeaders[$i]);
			for($i = 0; $i < count($fileLocalHeaders); ++$i)
				$fileLocalHeaders[$i] = preg_replace("/^\#include\s\"(.*)\"\s*$/", "\"".$headerSearchDirectory."$1\"", $fileLocalHeaders[$i]);

			$assocHeaderSystemHeaders = Array();
			$assocHeaderLocalHeaders = Array();
			if($hasAssocHeader) {
				$assocHeaderContents = file_get_contents("./".$headerSearchDirectory.$assocHeader);
				$matches = Array();
				if(preg_match_all("/\s*\#include\s(\<.*\>)\s*$/m", $assocHeaderContents, $matches))
					$assocHeaderSystemHeaders = $matches[1];
				if(preg_match_all("/\s*\#include\s(\".*\")\s*$/m", $assocHeaderContents, $matches))
					foreach($matches[1] as $header)
						array_push($assocHeaderLocalHeaders, "\"".$headerSearchDirectory.preg_replace("/\"(.*)\"/", "$1", $header)."\"");

				// check for duplicate includes and splice them out of the primary file
				foreach($assocHeaderSystemHeaders as $header) {
					if(in_array($header, $fileSystemHeaders)) {
						echo $file." duplicates inclusion of file ".$header." with it's associated header file.\n";
						array_splice($fileSystemHeaders, array_search($header, $fileSystemHeaders), 1);
					}
				}
				foreach($assocHeaderLocalHeaders as $header) {
					if(in_array($header, $fileLocalHeaders)) {
						echo $file." duplicates inclusion of file ".$header." with it's associated header file.\n";
						array_splice($fileLocalHeaders, array_search($header, $fileLocalHeaders), 1);
					}
				}
			}

			// remove comments and whitespace
			$fileContents = implode("", $fileContentsArray);
			$fileContents = preg_replace("/\/\/.*\n/", "\n", $fileContents);
			$matches = Array(); $matches2 = Array();
			while(preg_match_all("/(\/\*.*?\*\/)/s", $fileContents, $matches)) {
				$newLines = preg_match_all("/\n/", $matches[1][0], $matches2);
				$replacementString = ""; for($i = 0; $i < $newLines; ++$i) $replacementString .= "\n";
				$fileContents = preg_replace("/(\/\*.*?\*\/)/s", $replacementString, $fileContents, 1);
			}
			$fileContents = preg_replace("/\".*?\"/", "\"\"", $fileContents);

			// check by type
			foreach($headersByType as $type => $header) {
				$escapedType = preg_replace("/\:/", "\:", $type);
				$escapedType = preg_replace("/\(\)/", "\(", $escapedType);
				if(preg_match("/[^\(]$/", $escapedType)) $escapedType .= "[^\w]";
				$escapedType = "[^\w]".$escapedType;

				if(preg_match("/".$escapedType."/", $fileContents)) {
					if(
						! (substr($header, 0, 1) == "<" && (in_array($header, $fileSystemHeaders) || in_array($header, $assocHeaderSystemHeaders))) && // doesn't include system header
						! (substr($header, 0, 1) == "\"" && (in_array($header, $fileLocalHeaders) || in_array($header, $assocHeaderLocalHeaders))) && // doesn't include local header
						! ($hasAssocHeader && $header == "\"".$headerSearchDirectory.$assocHeader."\"") && // isn't our own associated header
						$file != "./".preg_replace("/\"(.*)\"/", "$1", $header) // doesn't define the type within
					) {
						echo $file." needs to incorporate header ".$header." for type ".$type."\n";
						if(substr($header, 0, 1) == "<") array_push($fileSystemHeaders, $header);
						else array_push($fileLocalHeaders, $header);
						$improperIncludes = TRUE;
					}
				}
			}
			usort($fileSystemHeaders, "customSort");
			usort($fileLocalHeaders, "customSort");

			// check by existing headers
			$unneededHeaders = Array();
			foreach(array_merge($fileSystemHeaders, $fileLocalHeaders) as $header) {
				$types = array_keys($headersByType, $header);
				$needsHeader = FALSE;

				// skip headers that don't define any types (they must serve some other purpose)
				if(! count($types))
					continue;

				foreach($types as $type) {
					$escapedType = preg_replace("/\:/", "\:", $type);
					$escapedType = preg_replace("/\(\)/", "\(", $escapedType);
					if(preg_match("/[^\(]$/", $escapedType)) $escapedType .= "[^\w]";
					$escapedType = "[^\w]".$escapedType;
				
					if(preg_match("/".$escapedType."/", preg_replace("/^\#.*$/m", "", $fileContents))) {
						$needsHeader = TRUE;
						break;
					}
				}

				if(! $needsHeader) {
					echo $file." incorporates header ".$header." unnecessarily.\n";
					array_push($unneededHeaders, $header);
					$improperIncludes = TRUE;
				}
			}

			foreach($unneededHeaders as $header)
				if(preg_match("/^\<.*\>$/", $header))
					array_splice($fileSystemHeaders, array_search($header, $fileSystemHeaders), 1);
				else
					array_splice($fileLocalHeaders, array_search($header, $fileLocalHeaders), 1);

			// compare header section to what it should be if no problems yet
			$properIncludeSection = Array();
			if($hasAssocHeader) {
				array_push($properIncludeSection, "#include \"".$assocHeader."\"\n");
				if(count($fileSystemHeaders))
					array_push($properIncludeSection, "\n");
			}
			for($i = 0; $i < count($fileSystemHeaders); ++$i) {
				array_push($properIncludeSection, "#include ".$fileSystemHeaders[$i]."\n");
			}
			if((count($fileSystemHeaders) || $hasAssocHeader) && count($fileLocalHeaders))
				array_push($properIncludeSection, "\n");
			for($i = 0; $i < count($fileLocalHeaders); ++$i)
				array_push($properIncludeSection, "#include ".preg_replace("/^\"".preg_replace("/\//", "\/", $headerSearchDirectory)."(.*)\"$/", "\"$1\"", $fileLocalHeaders[$i])."\n");

			if(! $improperIncludes) {
				for($i = 0; $i < count($properIncludeSection) && $i + $firstIncludeLine < count($fileContentsArray); ++$i) {
					if($fileContentsArray[$firstIncludeLine + $i] != $properIncludeSection[$i]) {
						echo $file." header section has other incorrect formatting.\n";
						$improperIncludes = TRUE;
						break;
					}
				}
			}

			// if there are issues, suggest the proper set
			if($improperIncludes) {
				echo $file." has malformed header inclusion section. Recommended (fix headers before source files!):\n";
				$maxLength = 10;
				for($i = 0; $i < count($properIncludeSection); ++$i)
					if(strlen($properIncludeSection[$i]) > $maxLength)
						$maxLength = strlen($properIncludeSection[$i]);
				$borderString = ""; for($i = 0; $i < $maxLength; ++$i) $borderString .= "="; $borderString .= "\n";
				echo $borderString;
				for($i = 0; $i < count($properIncludeSection); ++$i)
					echo $properIncludeSection[$i];
				echo $borderString;
			}
		}
	}

	// check for double spaces
	for($i = 0; $i < count($fileContentsArray); ++$i)
		if(preg_match("/  /", $fileContentsArray[$i]))
			echo $file.":".($i + 1)." contains a double space.\n";

	// check for trailing whitespace
	for($i = 0; $i < count($fileContentsArray); ++$i)
		if(preg_match("/\s+\n$/", $fileContentsArray[$i]))
			echo $file.":".($i + 1)." has trailing whitespace.\n";

	// check for single newline at end of file
	if(! preg_match("/\S\n$/", $fileContentsArray[count($fileContentsArray) - 1]))
		if(! in_array(substr($file, 2).":".($i + 1), $fileLineWhitelist))
			echo $file." does not end in single newline.\n";

	// check for spaces before opening parenthesis during if/for/while call
	for($i = 0; $i < count($fileContentsArray); ++$i)
		if(substr($fileContentsArray[$i], 0, 2) != "//" && preg_match("/(if|for|while)\s+\(/", $fileContentsArray[$i]))
			echo $file.":".($i + 1)." has extra space prior to opening parenthesis.\n";

	// check for lack of space before opening brace
	for($i = 0; $i < count($fileContentsArray); ++$i)
		if(preg_match("/\S\{/", $fileContentsArray[$i]))
			echo $file.":".($i + 1)." does not have a space prior to an opening brace.\n";

	// check for double initialization of objects
	for($i = 0; $i < count($fileContentsArray); ++$i)
		if(preg_match("/(\w+)\s\w+\s=\s\\1\(/", $fileContentsArray[$i]))
			if(! in_array(substr($file, 2).":".($i + 1), $fileLineWhitelist))
				echo $file.":".($i + 1)." may have an improper object initialization.\n";

	// check for forward declarations
	for($i = 0; $i < count($fileContentsArray); ++$i)
		if(preg_match("/^\s*class\s\w+\;/", $fileContentsArray[$i]))
			echo $file.":".($i + 1)." has a forward declaration.\n";

	// check for externs in headers
	if(preg_match("/\.h$/", $file))
		for($i = 0; $i < count($fileContentsArray); ++$i)
			if(preg_match("/^\s*extern/", $fileContentsArray[$i]))
				echo $file.":".($i + 1)." has an extern declaration and is a header file.\n";

	// check for proper header guards
	if(preg_match("/\.h$/", $file)) {
		$fileContents = implode("", $fileContentsArray);

		$matches = Array();
		preg_match("/(\w+)\.H$/m", strtoupper($file), $matches);

		if(! preg_match("/^\#ifndef\s".$matches[1]."_H\n\#define\s".$matches[1]."_H$"."/m", $fileContents))
			echo $file." does not have proper upper header guard.\n";

		if(! preg_match("/^\#endif\s\/\/\s".$matches[1]."_H$/m", $fileContents))
			echo $file." does not have proper lower header guard.\n";
	}

	$fileContents = implode("", $fileContentsArray);

	// count total lines
	$originalLineCount = count(preg_split("/\n/", $fileContents));

	// remove blank/empty lines and re-count
	$fileContents = preg_replace("/\n\s*\n/", "\n", $fileContents);
	$whiteSpaceLineCount = $originalLineCount - (count(preg_split("/\n/", $fileContents)) - 1);

	// remove comments and re-count
	$fileContents = preg_replace("/^\s*\/\/.*$/m", "", $fileContents); // "//" comments
	$fileContents = preg_replace("/\/\*.*?\*\//s", "", $fileContents); // /* */ comments
	$fileContents = preg_replace("/\n\s*\n/", "\n", $fileContents); // whitespace again
	$fileContents = trim($fileContents)."\n";
	$commentLineCount = $originalLineCount - $whiteSpaceLineCount - (count(preg_split("/\n/", $fileContents)) - 1);

	$totalCodeLineCount += count(preg_split("/\n/", $fileContents)) - 1;
	$totalCommentLineCount += $commentLineCount;
	$totalWhiteSpaceLineCount += $whiteSpaceLineCount;
}

echo "Code: ".$totalCodeLineCount.", Comments: ".$totalCommentLineCount.", WhiteSpace: ".$totalWhiteSpaceLineCount.", Total: ".($totalCodeLineCount + $totalCommentLineCount + $totalWhiteSpaceLineCount)."\n";
?>
