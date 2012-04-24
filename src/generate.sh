#!/bin/bash

cat > ../miniwin.cpp <<EOF

$(cat LICENSE)

// VERSION: $(cat VERSION)


#if defined(_WIN32)

// Windows ////////////////////////////////////////////////////////////////////////////

$(cat miniwin-windows.cc)

///////////////////////////////////////////////////////////////////////////////////////

#elif defined(__linux)

// Linux //////////////////////////////////////////////////////////////////////////////

$(cat miniwin-linux.cc)

///////////////////////////////////////////////////////////////////////////////////////

#else

#error "MiniWin no funciona en esta plataforma"

#endif

EOF

cat > ../miniwin.h <<EOF

$(cat LICENSE)

// VERSION: $(cat VERSION)

$(cat miniwin.h)

EOF

cp ../miniwin.h   ../HolaMiniWin-linux/miniwin.h
cp ../miniwin.cpp ../HolaMiniWin-linux/miniwin.cpp
cp ../miniwin.h   ../HolaMiniWin-windows/miniwin.h
cp ../miniwin.cpp ../HolaMiniWin-windows/miniwin.cpp

# Crea zip y tar
pushd ..
zip HolaMiniWin-windows.zip HolaMiniWin-windows/*
tar czvf HolaMiniWin-linux.tar.gz HolaMiniWin-linux/
popd
