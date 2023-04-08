
CFLAGS = -ICoreApp
export CFLAGS

CPP_SOURCES = $(wildcard CoreApp/*.cpp)
export CPP_SOURCES

all:
	$(MAKE) -C LowLevel/CanStepDriverPowerstep

clean:
	$(MAKE) -C LowLevel/CanStepDriverPowerstep clean
