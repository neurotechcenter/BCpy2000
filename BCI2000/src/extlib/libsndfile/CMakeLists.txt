# $Id$
# BCI2000-specific static library build of libsndfile.

SET( PACKAGE libsndfile )
SET( PACKAGE_VERSION 1.0.25 )
SET( HAVE_EXTERNAL_LIBS 0 )
ADD_DEFINITIONS(
  -DPACKAGE=\"${PACKAGE}\"
  -DVERSION=\"${PACKAGE_VERSION}\"
  -DPACKAGE_NAME=\"${PACKAGE}\"
  -DPACKAGE_VERSION=\"${PACKAGE_VERSION}\"
  -DHAVE_EXTERNAL_LIBS=${HAVE_EXTERNAL_LIBS}
)

SET( LS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${PACKAGE}-${PACKAGE_VERSION} )

SET( LS_INCLUDES
  include/sndfile.h
  config/config.h
)

SET( LS_SOURCES

  ${LS_DIR}/src/sndfile.c
  ${LS_DIR}/src/aiff.c
  ${LS_DIR}/src/au.c
  ${LS_DIR}/src/avr.c
  ${LS_DIR}/src/caf.c
  ${LS_DIR}/src/dwd.c
  ${LS_DIR}/src/flac.c  
  ${LS_DIR}/src/g72x.c
  ${LS_DIR}/src/htk.c
  ${LS_DIR}/src/ircam.c
  ${LS_DIR}/src/macbinary3.c
  ${LS_DIR}/src/macos.c
  ${LS_DIR}/src/mat4.c
  ${LS_DIR}/src/mat5.c  
  ${LS_DIR}/src/nist.c
  ${LS_DIR}/src/paf.c
  ${LS_DIR}/src/pvf.c
  ${LS_DIR}/src/raw.c
  ${LS_DIR}/src/rx2.c
  ${LS_DIR}/src/sd2.c
  ${LS_DIR}/src/sds.c
  ${LS_DIR}/src/svx.c  
  ${LS_DIR}/src/txw.c
  ${LS_DIR}/src/voc.c
  ${LS_DIR}/src/wve.c
  ${LS_DIR}/src/w64.c
  ${LS_DIR}/src/wav_w64.c
  ${LS_DIR}/src/wav.c
  ${LS_DIR}/src/xi.c
  ${LS_DIR}/src/mpc2k.c  
  ${LS_DIR}/src/rf64.c
  ${LS_DIR}/src/ogg_vorbis.c
  ${LS_DIR}/src/ogg_speex.c
  ${LS_DIR}/src/ogg_pcm.c

  ${LS_DIR}/src/common.c
  ${LS_DIR}/src/file_io.c
  ${LS_DIR}/src/command.c
  ${LS_DIR}/src/pcm.c  
  ${LS_DIR}/src/ulaw.c
  ${LS_DIR}/src/alaw.c
  ${LS_DIR}/src/float32.c
  ${LS_DIR}/src/double64.c
  ${LS_DIR}/src/ima_adpcm.c
  ${LS_DIR}/src/ms_adpcm.c  
  ${LS_DIR}/src/gsm610.c
  ${LS_DIR}/src/dwvw.c
  ${LS_DIR}/src/vox_adpcm.c
  ${LS_DIR}/src/interleave.c
  ${LS_DIR}/src/strings.c
  ${LS_DIR}/src/dither.c  
  ${LS_DIR}/src/broadcast.c
  ${LS_DIR}/src/audio_detect.c
  ${LS_DIR}/src/ima_oki_adpcm.c
  ${LS_DIR}/src/chunk.c
  ${LS_DIR}/src/ogg.c
  ${LS_DIR}/src/chanmap.c
  ${LS_DIR}/src/windows.c
  ${LS_DIR}/src/id3.c

  ${LS_DIR}/src/G72x/g721.c
  ${LS_DIR}/src/G72x/g723_16.c  
  ${LS_DIR}/src/G72x/g723_24.c
  ${LS_DIR}/src/G72x/g723_40.c
  ${LS_DIR}/src/G72x/g72x.c

  ${LS_DIR}/src/GSM610/add.c
  ${LS_DIR}/src/GSM610/code.c 
  ${LS_DIR}/src/GSM610/decode.c
  ${LS_DIR}/src/GSM610/gsm_create.c
  ${LS_DIR}/src/GSM610/gsm_decode.c 
  ${LS_DIR}/src/GSM610/gsm_destroy.c
  ${LS_DIR}/src/GSM610/gsm_encode.c 
  ${LS_DIR}/src/GSM610/gsm_option.c
  ${LS_DIR}/src/GSM610/long_term.c
  ${LS_DIR}/src/GSM610/lpc.c 
  ${LS_DIR}/src/GSM610/preprocess.c
  ${LS_DIR}/src/GSM610/rpe.c
  ${LS_DIR}/src/GSM610/short_term.c 
  ${LS_DIR}/src/GSM610/table.c
)

INCLUDE_DIRECTORIES(
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/config
  ${LS_DIR}/src
  ${LS_DIR}/src/G72x
  ${LS_DIR}/src/GSM610
)

IF(MSVC)
  INCLUDE_DIRECTORIES(
    ${PROJECT_SRC_DIR}/shared/compat
  )
  ADD_DEFINITIONS(
    /D_CRT_SECURE_NO_WARNINGS
    /FIC99Defs.h
    /FIPOSIXDefs.h
    /wd4244 # conversion: possible loss of data
    /wd4267 # assignment: possible loss of data
    /wd4305 # truncation
  )
ENDIF()

ADD_LIBRARY( Sndfile STATIC
  ${LS_INCLUDES}
  ${LS_SOURCES}
)
SET_PROPERTY( TARGET Sndfile PROPERTY FOLDER "${DIR_NAME}" )

IF( SNDFILE_BUILD_TESTS )
  # Tests imported from tests/test-wrapper.sh.in
  SET( testxs_
    write_read
    lossy_comp
    peak_chunk
    header
    misc
    string
    multi_file
    aiff_rw
    raw
    stdio
    virtual_io
    pipe
    ogg
    vorbis
  )
  SET( testsrc_
    ${LS_DIR}/tests/utils.c
    ${LS_DIR}/tests/generate.c
  )
  FOREACH( test_ ${testxs_} )
    ADD_EXECUTABLE( ${test_}_test ${LS_DIR}/tests/${test_}_test.c ${testsrc_} )
    TARGET_LINK_LIBRARIES( ${test_}_test Sndfile BCI2000FrameworkCore )
    SET_PROPERTY( TARGET ${test_}_test PROPERTY FOLDER Tests/Sndfile )
  ENDFOREACH()

  SET( tests_

    "write_read_test aiff"
    "lossy_comp_test aiff_ulaw"
    "lossy_comp_test aiff_alaw"
    "lossy_comp_test aiff_gsm610"

    "peak_chunk_test aiff"
    "header_test aiff"
    "misc_test aiff"
    "string_test aiff"
    "multi_file_test aiff"
    "aiff_rw_test"

    "write_read_test au"
    "lossy_comp_test au_ulaw"
    "lossy_comp_test au_alaw"
    "lossy_comp_test au_g721"
    "lossy_comp_test au_g723"
    "header_test au"
    "misc_test au"
    "multi_file_test au"

    "write_read_test caf"
    "lossy_comp_test caf_ulaw"
    "lossy_comp_test caf_alaw"
    "header_test caf"
    "peak_chunk_test caf"
    "misc_test caf"

    "write_read_test wav"
    "lossy_comp_test wav_pcm"
    "lossy_comp_test wav_ima"
    "lossy_comp_test wav_msadpcm"
    "lossy_comp_test wav_ulaw"
    "lossy_comp_test wav_alaw"
    "lossy_comp_test wav_gsm610"
    "lossy_comp_test wav_g721"
    "peak_chunk_test wav"
    "header_test wav"
    "misc_test wav"
    "string_test wav"
    "multi_file_test wav"

    "write_read_test w64"
    "lossy_comp_test w64_ima"
    "lossy_comp_test w64_msadpcm"
    "lossy_comp_test w64_ulaw"
    "lossy_comp_test w64_alaw"
    "lossy_comp_test w64_gsm610"
    "header_test w64"
    "misc_test w64"

    "write_read_test rf64"
    "header_test rf64"
    "misc_test rf64"

    "write_read_test raw"
    "lossy_comp_test raw_ulaw"
    "lossy_comp_test raw_alaw"
    "lossy_comp_test raw_gsm610"
    "lossy_comp_test vox_adpcm"
    "raw_test"

    "write_read_test paf"
    "header_test paf"
    "misc_test paf"

    "write_read_test svx"
    "header_test svx"
    "misc_test svx"

    "write_read_test nist"
    "lossy_comp_test nist_ulaw"
    "lossy_comp_test nist_alaw"
    "header_test nist"
    "misc_test nist"

    "write_read_test ircam"
    "lossy_comp_test ircam_ulaw"
    "lossy_comp_test ircam_alaw"
    "header_test ircam"
    "misc_test ircam"

    "write_read_test voc"
    "lossy_comp_test voc_ulaw"
    "lossy_comp_test voc_alaw"
    "header_test voc"
    "misc_test voc"

    "write_read_test mat4"
    "header_test mat4"
    "misc_test mat4"

    "write_read_test mat5"
    "header_test mat5"
    "misc_test mat5"

    "write_read_test pvf"
    "header_test pvf"
    "misc_test pvf"

    "lossy_comp_test xi_dpcm"

    "write_read_test htk"
    "header_test htk"
    "misc_test htk"

    "write_read_test avr"
    "header_test avr"
    "misc_test avr"

    "write_read_test sds"
    "header_test sds"
    "misc_test sds"

    "write_read_test sd2"

    "lossy_comp_test wve"

    "write_read_test mpc2k"
    "header_test mpc2k"
    "misc_test mpc2k"

    "write_read_test flac"
    "string_test flac"

    "ogg_test"
    "vorbis_test"
    "lossy_comp_test ogg_vorbis"
    "string_test ogg"
    "misc_test ogg"

    "stdio_test"
    "pipe_test"
    "virtual_io_test"
  )

  FOREACH( test_ ${tests_} )
    STRING( REPLACE " " "__" name_ ${test_} )
    STRING( REGEX MATCHALL "[^ ]+" test_ ${test_} )
    ADD_TEST( ${name_} ${test_} )
  ENDFOREACH()

ENDIF()
