#!/bin/bash

set -eo pipefail

SCRIPT_ROOT=$(dirname $0)/wasm/build-scripts

run() {
  for name in $@; do
    $SCRIPT_ROOT/$name.sh
  done
}

run-all() {
  SCRIPTS=(
    # install dependencies
    install-deps
		build-zlib
		build-x264
		build-x265
		build-libvpx
		build-wavpack
		build-lame
		build-fdk-aac
		build-ogg
		build-vorbis
		build-theora
		build-opus
		build-libwebp
		build-freetype2
		build-fribidi
		build-harfbuzz
		build-libass
		#build-aom # disabled as it is extremely slow
		configure-ffmpeg
    build-ffmpeg
    # configure-ffmpeg.mp4-scale
    # build-ffmpeg.mp4-scale
    # configure-ffmpeg.png-to-mp4
    # build-ffmpeg.png-to-mp4
  )
  run ${SCRIPTS[@]}
}


run-lite() {
  SCRIPTS=(
    # install dependencies
    install-deps
		build-zlib
		build-x264
		build-x265
		build-libvpx
		build-wavpack
		build-lame
		build-fdk-aac
		build-ogg
		build-vorbis
		build-theora
		build-opus
		configure-ffmpeg
    build-ffmpeg
  )
  run ${SCRIPTS[@]}
}

run-ffmpeg() {
  SCRIPTS=(
		configure-ffmpeg
    build-ffmpeg
  )
  run ${SCRIPTS[@]}
}

main() {
  # verify Emscripten version
  emcc -v
  if [[ "$@" == "" ]]; then
    echo "---------------run-lite---------------"
    run-lite
  else
    echo "---------------run-$@---------------"
    run-"$@"
  fi
}

main "$@"
