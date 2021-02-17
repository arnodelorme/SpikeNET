Warning if using jcc to recompile SpikeNET: 
subdirectory must be set menually into jcc
For dependancies, they must also be set internally

MYCODEDIR := -I. -I./sources-imgs -I./sources-readImgs -I./IO -I./Maps -I/usr/X11R6/include -I./SpikeNETcore

# Directories to search for header files

SEARCHDIRS := -I- ${MYCODEDIR}

# makemake variables
# -Wall -Werror

DEPENDFLAGS := -g -w -fno-inline ${SEARCHDIRS}
