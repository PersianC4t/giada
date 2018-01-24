#!/usr/bin/env bash

if [[ $TRAVIS_OS_NAME == 'osx' ]]; then

  brew update
  brew install rtmidi
  brew install jansson
  brew install libsamplerate
  brew install fltk
  brew install libsndfile
  brew install upx

  #ls Remove dynamic libraries to force static linking.

  rm -rf /usr/local/lib/librtmidi.dylib
  rm -rf /usr/local/lib/librtmidi.4.dylib
  rm -rf /usr/local/lib/libjansson.dylib
  rm -rf /usr/local/lib/libjansson.4.dylib
  rm -rf /usr/local/lib/libsamplerate.dylib
  rm -rf /usr/local/lib/libsamplerate.0.dylib
  rm -rf /usr/local/lib/libfltk.1.3.dylib
  rm -rf /usr/local/lib/libfltk.dylib
  rm -rf /usr/local/lib/libfltk_forms.1.3.dylib
  rm -rf /usr/local/lib/libfltk_forms.dylib
  rm -rf /usr/local/lib/libfltk_forms.dylib
  rm -rf /usr/local/lib/libfltk_gl.1.3.dylib
  rm -rf /usr/local/lib/libfltk_gl.dylib 
  rm -rf /usr/local/lib/libfltk_images.1.3.dylib
  rm -rf /usr/local/lib/libfltk_images.dylib 
  rm -rf /usr/local/lib/libsndfile.1.dylib 
  rm -rf /usr/local/lib/libsndfile.dylib 
  rm -rf /usr/local/lib/libFLAC++.6.dylib
  rm -rf /usr/local/lib/libFLAC++.dylib
  rm -rf /usr/local/lib/libFLAC.8.dylib
  rm -rf /usr/local/lib/libFLAC.dylib
  rm -rf /usr/local/lib/libogg.0.dylib
  rm -rf /usr/local/lib/libogg.dylib
  rm -rf /usr/local/lib/libvorbis.0.dylib
  rm -rf /usr/local/lib/libvorbis.dylib
  rm -rf /usr/local/lib/libvorbisenc.2.dylib
  rm -rf /usr/local/lib/libvorbisenc.dylib

  # TODO - what about midimaps?

elif [[ $TRAVIS_OS_NAME == 'linux' ]]; then

  sudo apt-get install -y gcc-6 g++-6 libsndfile1-dev libsamplerate0-dev \
  	libfltk1.3-dev libasound2-dev libxpm-dev libpulse-dev libjack-dev \
  	libxrandr-dev libx11-dev libxinerama-dev libxcursor-dev

  # Symlink gcc in order to use the latest version

  sudo ln -f -s /usr/bin/g++-6 /usr/bin/g++

  # Download and build latest version of RtMidi

  wget https://github.com/thestk/rtmidi/archive/master.zip
  unzip master.zip
  cd rtmidi-master && ./autogen.sh && ./configure --with-jack --with-alsa && make && sudo make install || true
  cd ..

  #wget http://www.music.mcgill.ca/~gary/rtmidi/release/rtmidi-2.1.1.tar.gz
  #tar -xvf rtmidi-2.1.1.tar.gz
  #cd rtmidi-2.1.1 && ./configure --with-jack --with-alsa && make && sudo make install || true
  #cd ..

  # Download and install latest version of Jansson
  # TODO - no longer needed! Use apt instead

  wget http://www.digip.org/jansson/releases/jansson-2.7.tar.gz
  tar -xvf jansson-2.7.tar.gz
  cd jansson-2.7 && ./configure && make && sudo make install || true
  sudo ldconfig
  cd ..

  # Download midimaps package for testing purposes

  wget https://github.com/monocasual/giada-midimaps/archive/master.zip -O giada-midimaps-master.zip
  unzip giada-midimaps-master.zip
  mkdir -p $HOME/.giada/midimaps
  cp giada-midimaps-master/midimaps/* $HOME/.giada/midimaps

  # Download vst plugin for testing purposes

  #- wget http://www.discodsp.com/download/?id=18 -O bliss-linux.zip
  #- unzip bliss-linux.zip -d bliss-linux
  #- cp bliss-linux/64-bit/Bliss64Demo.so .

fi