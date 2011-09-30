Dominicus
http://games.fairserve.net/dominicus

INTRODUCTION

Dominicus is a networked, multiplayer, 3D flight simulator combat game. The
goal of the game is to defeat enemy ships by shooting them until their
capability to shield themselves is depleted. Firing bolts and shielding
against them (by far a greater power drain) draws power from a ship's energy
reservoir. When a ship's energy reservoir is entirely depleted, the ship is
defeated, and it is "bounced" into the air to recharge before landing on a
respawn position. Besides the primary firing method (bolts), ships in the
vicinity may be defeated by detonating a concussion, which is an energy wave
that requires several seconds to charge (during which the charging ship is
incapacitated) but has more devastating destructive power. Players are
assigned to "clans" indicated by their camouflage schemes of "desert,"
"forest," or "aqua." Players work together with their clan to destroy players
of enemy clans. Gameplay continues until players leave of their own accord.

Dominicus evolved out of the author's attempt to learn how to program 3D
simulation using OpenGL. It has been in development in one form or another
since March 2010. Dominicus has been designed to be OpenGL 2.0 compatible,
meaning that the game should run on most dedicated graphics cards made around
or after 2006 (assuming full operating system and driver support for the
required features), while allowing for the flexibility of modern shader-based
drawing pipelines. While the author hopes players find the game enjoyable, the
game began as and continues to be an exercise, experiment, and learning
experience for implementing new graphical effects and techniques.

Although it has been primarily created by a single author, Dominicus is free
and open-source software. See the following sections for information on how to
obtain the source code for this program.

LICENSE

Dominicus source code and binary file distributions are released under the
terms of the Simplified BSD License. See the file licenses/DOMINICUS.txt for
the text of the license.

Dominicus data files (anything in the data/ directory, which may be packaged
in the executable) are released under the terms of the Creative Commons
Attribution-ShareAlike 3.0 license. See the file licenses/DOMINICUS_DATA.txt
for information on how to obtain a copy of this license.

All Dominicus source code, binary files, data files, build files,
configuration files, and other files included in any source, build,
documentation, web site, or other distribution is Copyright 2010-2011 Joshua
Bodine, or their creators/contributors.

ONLINE RESOURCES

Game-related resources and information can be found on the following web sites:

http://games.fairserve.net (home page, game info, screenshots, videos)
http://code.google.com/p/dominicus (source code, development info, tracker)
https://www.ohloh.net/p/dominicus (code analysis and statistics)
https://groups.google.com/forum/#!forum/dominicus-development (mailing list)

OBTAINING BINARY DISTRIBUTIONS

Currently, no binary releases have been made. Please build from source code
instead.

OBTAINING SOURCE CODE

Dominicus development source code can be obtained via a git public repository
hosted courtesy of Google (http://code.google.com). Development source code
should be considered unstable, but contains new or experimental features and/or
corrections to known issues, allowing you to preview how future releases may
look or to assist with testing or development. To obtain a copy of the source
code, install your favorite git client and clone the following repository:
https://code.google.com/p/dominicus/

BUILDING DOMINICUS

Dominicus configuration and build project files are maintained for Mac OS X
(Xcode), Linux (GNU Autotools), and Windows (MSVC). See the appropriate section
below for instructions corresponding to your platform.

Dominicus has the following external library dependencies on all platforms:
	* Simple DirectMedia Layer (SDL), version 1.2
	* FreeType 2
	* zlib

Dominicus should build with the most current stable release for these
libraries. If it does not, please submit a support ticket with a full error
message and description at http://code.google.com/p/dominicus/issues/list and I
will take a look.

BUILDING DOMINICUS ON MAC OS X

Dominicus is built on Mac OS X using the Xcode IDE. The project file should be
compatible with Xcode 3.1 and later 3.x versions, which means it can be built
on Mac OS X 10.5 Leopard and later operating system versions.

The Xcode project file is set to look for required library files in certain
locations.

To set up SDL, download the Mac OS X framework from http://www.libsdl.org (make
sure you select the latest SDL 1.2 build). Place the SDL.framework file in the
/Library/Frameworks/ directory.

To set up zlib, download the zlib source code from
http://sourceforge.net/projects/libpng/files/zlib (development of zlib appears
to be finished, so the version you need is probably 1.2.5).

When you have downloaded zlib, execute the following commands (or appropriate
alternatives, as desired) in a shell (such as Terminal.app, in your
/Applications/Utilities/ directory) to build and install zlib (substitute the
appropriate library versions that you downloaded):

# cd ~/Downloads
# tar -xjf zlib-1.2.5.tar.bz2			(substitute correct version)
# sudo mv zlib-1.2.5 /usr/local/src/		(substitute correct version)
# rm zlib-1.2.5.tar.bz2				(substitute correct version)
# cd /usr/local/src/zlib-1.2.5			(substitute correct version)
# ./configure --static
# make
# sudo make install

Note: when you pass the --static argument to configure, the make install
command may generate an error; however, the installation should still have
completed successfully, which you can verify with the presence of
/usr/local/lib/libz.a.

zlib should now be installed where the Dominicus Xcode project file can find
it. Alternatively, you can install zlib elsewhere, and change the project build
settings and files to reflect that other location.

To set up Freetype 2, download the latest stable release of Freetype 2 source
code from http://sourceforge.net/projects/freetype/files/freetype2/ to your
computer. Sourceforge may suggest the latest version to download near the top
of the screen.

When you have downloaded Freetype 2, execute the following commands (or
appropriate alternatives, as desired) in a shell (such as Terminal.app, in your
/Applications/Utilities/ directory) to build and install Freetype 2 (substitute
the appropriate library versions that you downloaded):

# cd ~/Downloads
# tar -xjf freetype-2.4.4.tar.bz2		(substitute correct version)
# sudo mv freetype-2.4.4 /usr/local/src/	(substitute correct version)
# rm freetype-2.4.4.tar.bz2			(substitute correct version)
# cd /usr/local/src/freetype-2.4.4		(substitute correct version)
# ./configure --disable-shared
# make
# sudo make install

Freetype 2 should now be installed where the Dominicus Xcode project file can
find it. Alternatively, you can install Freetype 2 elsewhere, and change the
project build settings and files to reflect that other location.

To build, open the file "dominicus.xcodeproj" in the source directory. Expand
the "frameworks" group near the bottom of the files panel on the left, and
verify that the files "SDL.framework," "libz.a," and "libfreetype.a" are not
shown in red text. This will show you that you installed the dependencies
correctly. Press the keys Command-B, or select "Build" from the Build menu, to
build the project. The resulting application should appear in the build/
directory in the source code.

BUILDING DOMINICUS ON LINUX

The Autotools build files for building Dominicus on Linux and other *nix
platforms are still in development.

BUILDING DOMINICUS ON WINDOWS

The MSVC project files and dependencies for building Dominicus on Windows are
still in development.

CREDITS

Dominicus has been primarily created by Joshua Bodine.

MSVC project and various code changes contributed by Jeffery Myers.

Portions of this software are copyright (c) 2010 The FreeType Project
(www.freetype.org).  All rights reserved.

This software uses the Simple DirectMedia Layer library
(http://www.libsdl.org/).

This software uses the zlib library (http://www.zlib.net/).

DEDICATION

Dominicus is dedicated to Sergeant Sean Drenth #6894 of the Phoenix Police
Department, EOW October 18, 2010.

Miss you, boss.

CHANGELOG

Dominicus 0.x "Conception"
	.1
		* Initial version for original development.
