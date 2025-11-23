# Install Dependencies
## Required 
- C++ Compiler
- GMP


## Optional 
- OpenMP Support for the Compiler
- doxygen components 
    - dot (graphviz) 
    - mscgen
    - dia



### Ubuntu
Required

    sudo apt install build-essentials cmake libgmp-dev

Optional

    sudo apt install libomp-15-dev graphviz

### Fedora 42
Required

    sudo dnf group install c-development development-tools -y
    sudo dnf install cmake -y

    git clone https://github.com/microsoft/vcpkg.git
    cd vcpkg
    ./bootstrap-vcpkg.sh
    echo '# VCPKG' >> ~/.bashrc
    echo "export VCPKG_ROOT=\"$PWD\"" >> ~/.bashrc
    echo 'export PATH="$VCPKG_ROOT:$PATH"' >> ~/.bashrc
    echo 'export PATH' >> ~/.bashrc
    source ~/.bashrc


# Build
With self managed dependencies

    cmake -S src -b build
    cmake --build build

With vcpkg

    cmake -S src --preset <os>-<build-system>-<compiler>
    cmake --build build/<os>-<build-system>-<compiler>

For a list of available presets see

    cmake -S src --list-presets
# Test

    ctest --output-on-failure --test-dir build

With preset

    ctest --output-on-failure --test-dir build/<os>-<build-system>-<compiler>
