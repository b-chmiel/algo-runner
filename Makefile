CXX=clang++

build: configure
	cmake --build ./build --target algo

configure:
	CXX=$(CXX) cmake . -G"Ninja" -DCMAKE_LINKER=/usr/bin/lld -B ./build

install: build
	cmake --install ./build

clean:
	- rm -rf build