.PHONY: all opt unit clean debug release test unittest allunit docs doxygen format sqlite imdb amal

all: release
opt: release

GENERATOR=
FORCE_COLOR=

ifeq ($(GEN),ninja)
	GENERATOR=-G "Ninja"
	FORCE_COLOR=-DFORCE_COLORED_OUTPUT=1
endif

clean:
	rm -rf build

debug:
	mkdir -p build/debug && \
	cd build/debug && \
	cmake $(GENERATOR) $(FORCE_COLOR) -DCMAKE_BUILD_TYPE=Debug ../.. && \
	cmake --build .

release:
	mkdir -p build/release && \
	cd build/release && \
	cmake $(GENERATOR) $(FORCE_COLOR) -DCMAKE_BUILD_TYPE=Release ../.. && \
	cmake --build .

unit: debug
	build/debug/minimal_icu_example
