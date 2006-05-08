#!/bin/sh

########################
## MAIN CONFIGURATION ##
########################

    ## Main settings.
    PREFIX="/opt/"

    ## Set the directories.
    PTCDIR="`pwd`"
    SRCDIR="`pwd`/tmp/"
    SRCDIR="/home/coding/self/ports"

    ## Source code versions.
    GTK="gtk+-2.4.14"
    MPLAYER="MPlayer-1.0pre7try2"
    LINKS="links-2.1pre21"
    SDL="SDL-1.2.9"
    
#########################
## PARSE THE ARGUMENTS ##
#########################

    ## If no arguments are given...
    if test $# -eq 0 ; then

	## Do everything possible.
	DO_DOWNLOAD=1
	BUILD_GTK=1
	BUILD_MPLAYER=1
	BUILD_LINKS=1
	BUILD_SDL=1
    ## Else...
    else
	## Parse the arguments.
	while test $# -ge 1 ; do
	    case "$1" in
		-d | -download | --download)
		    DO_DOWNLOAD=1
		    shift;;
		-g | -gtk | --gtk)
		    BUILD_GTK=1
		    shift;;
		-m | -mplayer | --mplayer)
		    BUILD_MPLAYER=1
		    shift;;
		-l | -links | --links)
		    BUILD_LINKS=1
		    shift;;
		-s | -sdl | --sdl)
		    BUILD_SDL=1
		    shift;;
		-a | -all | --all)
		    DO_DOWNLOAD=1
		    BUILD_GTK=1
		    BUILD_MPLAYER=1
		    BUILD_SDL=1
		    BUILD_LINKS=1
		    shift;;
		-prefix | --prefix)
		    PREFIX="$2"
		    shift 2;;
		-help | --help)
		    echo "-d : download"
		    echo "-g : build $GTK"
		    echo "-m : build $MPLAYER"
		    echo "-l : build $LINKS"
		    echo "-s : build $SDL"
		    echo "-a : build all"
		    echo "-prefix path : prefix"
		    echo "-help : this text"
		    exit
	    esac
	done
    fi
    
###########################
## SOFTWARE DEPENDENCIES ##
###########################
    ## Check for make.
    if test "`gmake -v 2> /dev/null`" ; then
	MAKE="gmake"
    else
	if test "`make -v 2> /dev/null`" ; then
	    MAKE="make"
	else
	    echo "ERROR: Please make sure you have GNU 'make' installed."
	    exit
	fi
    fi
    
    ## Check for patch.
    if test "`gpatch -v 2> /dev/null`" ; then
	PATCH="gpatch -p1"
    else
	if test "`patch -v 2> /dev/null`" ; then
	    PATCH="patch -p1"
	else
	    echo "ERROR: Please make sure you have 'patch' installed."
	    exit
	fi
    fi
    
    ## Check for wget.
    if test "`wget -V 2> /dev/null`" ; then
	WGET="wget -c --passive-ftp"
    else
	echo "ERROR: Please make sure you have 'wget' installed."
	exit
    fi
    
################################
## DOWNLOAD, UNPACK AND PATCH ##
################################

    ## Create and enter the temp directory.
    mkdir -p "$SRCDIR"; cd "$SRCDIR"

    ## If we've been told to download...
    if test "$DO_DOWNLOAD" = "1" ; then
	## Download the gtk source.
	if test "$BUILD_GTK" = "1" ; then
	    if test ! -f "$GTK.tar.bz2" ; then
		$WGET ftp://ftp.gtk.org/pub/gtk/v2.4/$GTK.tar.bz2 || { echo "ERROR DOWNLOADING GTK"; exit; }
	    fi
	fi
	## Download the mplayer source.
	if test "$BUILD_MPLAYER" = "1" ; then
	    if test ! -f "$MPLAYER.tar.bz2" ; then
		$WGET ftp://ftp.mplayerhq.hu/MPlayer/releases/$MPLAYER.tar.bz2 || { echo "ERROR DOWNLOADING MPLAYER"; exit; }
	    fi
	fi
	## Download the newlib source.
	if test "$BUILD_SDL" = "1" ; then
	    if test ! -f "$SDL.tar.gz" ; then
		$WGET http://www.libsdl.org/release/$SDL.tar.gz || { echo "ERROR DOWNLOADING SDL"; exit; }
	    fi
	fi
	if test "$BUILD_LINKS" = "1" ; then
	    if test ! -f "$LINKS.tar.bz2" ; then
		$WGET http://links.twibright.com/download/$LINKS.tar.bz2 || { echo "ERROR DOWNLOADING LINKS"; exit; }
	    fi
	fi
    fi
    
    ## Unpack and patch the gtk source.
    if test "$BUILD_GTK" = "1" ; then
	rm -Rf $GTK; bzip2 -cd "$GTK.tar.bz2" | tar xvf -
	cd $GTK; cat "$PTCDIR/gtk/$GTK-xynth.diff" | $PATCH || { echo "ERROR PATCHING GTK"; exit; }
	cd ..
    fi
    
    ## Unpack and patch the mplayer source.
    if test "$BUILD_MPLAYER" = "1" ; then
	rm -Rf $MPLAYER; bzip2 -cd "$MPLAYER.tar.bz2" | tar xvf -
	cd $MPLAYER; cat "$PTCDIR/mplayer/$MPLAYER-xynth.diff" | $PATCH || { echo "ERROR PATCHING MPLAYER"; exit; }
	cd ..
    fi

    ## Unpack and patch the links source.
    if test "$BUILD_LINKS" = "1" ; then
	rm -Rf $LINKS; bzip2 -cd "$LINKS.tar.bz2" | tar xvf -
	cd $LINKS; cat "$PTCDIR/links/$LINKS-xynth.diff" | $PATCH || { echo "ERROR PATCHING LINKS"; exit; }
	cd ..
    fi

    ## Unpack and patch the sdl source.
    if test "$BUILD_SDL" = "1" ; then
	rm -Rf $SDL; gzip -cd "$SDL.tar.gz" | tar xvf -
	cd $SDL; cat "$PTCDIR/sdl/$SDL-xynth.diff" | $PATCH || { echo "ERROR PATCHING SDL"; exit; }
	cd ..
    fi

#######################
## BUILD AND INSTALL ##
#######################

    ## If we've been told to build gtk...
    if test "$BUILD_GTK" = "1" ; then
	## Enter the source directory.
	cd $GTK
	## Create and enter the build directory.
	mkdir -p build-xynth; cd build-xynth
	## Configure the source.
	../configure --prefix=$PREFIX --with-gdktarget=linux-fb || { echo "ERROR CONFIGURING GTK"; exit; }
	## Build the source.
	$MAKE clean; $MAKE || { echo "ERROR BUILDING GTK"; exit; }
	## Install the result.
#	$MAKE install || { echo "ERROR INSTALLING GTK"; exit; }
	## Clean up the result.
#	$MAKE clean
	## Exit the build and source directories.
	cd ..; cd ..
    fi
    
    ## If we've been told to build mplayer...
    if test "$BUILD_MPLAYER" = "1" ; then
	## Enter the source directory.
	cd $MPLAYER
	## Configure the source.
	./configure --prefix=$PREFIX || { echo "ERROR CONFIGURING MPLAYER"; exit; }
	## Build the source.
	$MAKE clean; $MAKE || { echo "ERROR BUILDING MPLAYER"; exit; }
	## Install the result.
	$MAKE install || { echo "ERROR INSTALLING MPLAYER"; exit; }
	## Clean up the result.
#	$MAKE clean
	## Exit the build and source directories.
	cd ..
    fi
    
    ## If we've been told to build mplayer...
    if test "$BUILD_LINKS" = "1" ; then
	## Enter the source directory.
	cd $LINKS
	## Create and enter the build directory.
	mkdir build-xynth; cd build-xynth
	## Configure the source.
	../configure --prefix=$PREFIX --enable-graphics --enable-javascript|| { echo "ERROR CONFIGURING LINKS"; exit; }
	## Build the source.
	$MAKE clean; $MAKE || { echo "ERROR BUILDING LINKS"; exit; }
	## Install the result.
	$MAKE install || { echo "ERROR INSTALLING LINKS"; exit; }
	## Clean up the result.
#	$MAKE clean
	## Exit the build and source directories.
	cd ..; cd ..
    fi
    
    ## If we've been told to build mplayer...
    if test "$BUILD_SDL" = "1" ; then
	## Enter the source directory.
	cd $SDL
	## Create and enter the build directory.
	mkdir build-xynth; cd build-xynth
	## Configure the source.
	../configure --prefix=$PREFIX || { echo "ERROR CONFIGURING SDL"; exit; }
	## Build the source.
	$MAKE clean; $MAKE || { echo "ERROR BUILDING SDL"; exit; }
	## Install the result.
	$MAKE install || { echo "ERROR INSTALLING SDL"; exit; }
	## Clean up the result.
#	$MAKE clean
	## Exit the build and source directories.
	cd ..; cd ..
    fi
