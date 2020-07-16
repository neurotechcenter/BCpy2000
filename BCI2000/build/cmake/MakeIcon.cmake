###########################################################################
## $Id$
## Authors: mellinger@neurotechcenter.org
## Description: CMake script to automate creation of an icon from template
##   plus overlay. Parameters are encoded into template name as follows:
##   <template-name>_<size>_<overlay-x>_<overlay-y>.png
##   overlay-x and overlay-y specify coordinates of the overlay's center within
##   the template when rendered at the given size.

if( NOT STYLE )
  set( STYLE GRAY )
endif()

set( STYLE_EGGSHELL
  BACKGROUND # Transparent
  TEMPLATE # Set opaque pixels to eggshell white
  SIZE 1
  "H:=46; S:=0.11; V:=A*0.94;" # quote to escape parentheses
  OVERLAY  # Replace white with eggshell
  SIZE 0.64 # Overlay edge length relative to template
  "H:=46; S:=0.11; V:=0.94*0.25*(1+3*V);"
)

set( STYLE_GRAY
  BACKGROUND # Transparent
  TEMPLATE # Desaturate and darken
  SIZE 1
  "S:=0; V:=A*0.71;"
  OVERLAY # Desaturate
  SIZE 0.64
  "S:=0;"
)

set( STYLE_LINEART
  BACKGROUND # Transparent
  TEMPLATE # Make bright pixels transparent
  SIZE 1
  "S:=0; V:=V*1.2; A:=A*(1-V);"
  OVERLAY # Make white pixels transparent
  SIZE 0.6
  "A:=A*(1-V);"
)

set( STYLE_RETRO
  BACKGROUND # White
  "V:=1; A:=1;"
  TEMPLATE # 80s Apple logo-like fully saturated color stripes
  SIZE 1
  "H:=floor((120-Y/Height*240)/40)*40+20;"
  "S:=1; V:=A;"
  OVERLAY # Make white pixels transparent
  SIZE 0.71
  "A:=A*(1-V);"
)

if( PREFLIGHT )
  set( VERBOSE TRUE )
endif()

if( VERBOSE )
  macro( status msg_ )
    message( STATUS "${msg_}" )
  endmacro()
else()
  macro( status )
  endmacro()
endif()

string( TOUPPER ${STYLE} STYLE )
if( NOT DEFINED STYLE_${STYLE} )
  set( styles_ )
  get_cmake_property( vars_ VARIABLES )
  foreach( var_ ${vars_} )
    if( var_ MATCHES "STYLE_.*" )
      list( APPEND styles_ ${var_} )
    endif()
  endforeach()
  string( REPLACE "STYLE_" ""  styles_ "${styles_}" )
  string( REPLACE ";" " "  styles_ "${styles_}" )
  string( TOLOWER ${styles_} styles_ )
  status( "Unknown style ${STYLE}, available styles are: ${styles_}" )
  message( FATAL_ERROR "" )
endif()

set( BACKGROUND_transform_ )
set( TEMPLATE_transform_ )
set( TEMPLATE_transform_size_ 1 )
set( OVERLAY_transform_ )
set( OVERLAY_transform_size_ 0.5 )
set( transform_ TEMPLATE_transform_ )
set( size_ )
foreach( statement_ ${STYLE_${STYLE}} )
  if( statement_ STREQUAL TEMPLATE
      OR statement_ STREQUAL OVERLAY
      OR statement_ STREQUAL BACKGROUND )
    set( transform_ ${statement_}_transform_ )
  elseif( statement_ STREQUAL SIZE )
    set( size_ ${transform_}size_ )
  elseif( size_ )
    set( ${size_} ${statement_} )
    set( size_ )
  else()
    string( REPLACE " " "" statement_ ${statement_} )
    set( ${transform_} "${${transform_}}${statement_};" )
  endif()
endforeach()

get_filename_component( tempdir_ ${OUTPUT} DIRECTORY )

if( NOT EXISTS PROJECT_ICON_TEMPLATE )
  file( GLOB files_ LIST_DIRECTORIES FALSE ${PROJECT_ICON_TEMPLATE}*.png )
  if( files_ )
    list( GET files_ 0 PROJECT_ICON_TEMPLATE )
  endif()
endif()

if( NOT EXISTS ${ICON} )
  if( NOT ICON MATCHES ".*\\.png" )
    set( ICON "${ICON}.png" )
  endif()
  if( NOT EXISTS ${ICON} )
    get_filename_component( dir_ ${PROJECT_ICON_TEMPLATE} DIRECTORY )
    if( EXISTS "${dir_}/${ICON}" )
      set( ICON "${dir_}/${ICON}" )
    elseif( EXISTS "${dir_}/overlays/${ICON}" )
      set( ICON "${dir_}/overlays/${ICON}" )
    endif()
  endif()
endif()
if( NOT EXISTS ${ICON} )
  status( "icon not found: ${ICON}" )
  message( FATAL_ERROR "" )
endif()

if( PREFLIGHT )
  return()
endif()

if( NOT ICON MATCHES ".*/overlays/.*" )
  status( "copying ${ICON} to ${OUTPUT}" )
  execute_process( COMMAND ${CMAKE_COMMAND} -E copy ${ICON} ${OUTPUT} )
else()
  get_filename_component( iconame_ ${ICON} NAME_WE )
  get_filename_component( name_ ${PROJECT_ICON_TEMPLATE} NAME_WE )
  string( REPLACE "_" ";" args_ ${name_} )
  list( GET args_ 0 name_ )
  list( GET args_ 1 size_ )
  list( GET args_ 2 xpos_ )
  list( GET args_ 3 ypos_ )
  math( EXPR halfsize_ "${size_}/2" )
  set( temp_ ${tempdir_}/${name_}_${iconame_}_tmp )
  status( "creating background: ${BACKGROUND_transform_}" )
  execute_process( COMMAND ${BUILDUTILS_DIR}/transform_png
    --width=${size_} --height=${size_}
    "--process=${BACKGROUND_transform_}"
    --no-input ${temp_}1
  )
  status( "transforming template: ${TEMPLATE_transform_}" )
  execute_process( COMMAND ${BUILDUTILS_DIR}/transform_png
    --width*=${TEMPLATE_transform_size_} --height*=${TEMPLATE_transform_size_}
    "--process=${TEMPLATE_transform_}"
    ${PROJECT_ICON_TEMPLATE} ${temp_}2
  )
  status( "transforming overlay: ${OVERLAY_transform_}" )
  execute_process( COMMAND ${BUILDUTILS_DIR}/transform_png
    --width*=${OVERLAY_transform_size_} --height*=${OVERLAY_transform_size_}
    "--process=${OVERLAY_transform_}"
    ${ICON} ${temp_}3
  )
  execute_process( COMMAND ${BUILDUTILS_DIR}/combine_png
    --x=${halfsize_} --y=${halfsize_} ${temp_}1
    --x=${halfsize_} --y=${halfsize_} ${temp_}2
    --x=${xpos_} --y=${ypos_} ${temp_}3
    ${OUTPUT}
  )
  file( REMOVE ${temp_}1 ${temp_}2 ${temp_}3 )
endif()

