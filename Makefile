SRCS=chenidct.c  decode.c  huffman.c  init.c  jfif_read.c  jpeg2bmp.c  marker.c  main.c

ACCEL_NAME = jpeg
TEST_BIN = $(ACCEL_NAME)
export TRACE_OUTPUT_DIR=$(ACCEL_NAME)
ifndef WORKLOAD
  export WORKLOAD=ChenIDct_f2r_ChenIDct
endif
include ../../../SHOC/common/Makefile.common
include ../../../SHOC/common/Makefile.tracer
