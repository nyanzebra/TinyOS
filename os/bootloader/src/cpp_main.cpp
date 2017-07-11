#include "__output"

extern "C" {

int cpp_main(int argc, const char** argv) {
    
    io::console::clear();
    io::console::printf("\n\n\nHi, and welcome to ROBOS, date: %d-%s-%s", 2017, "07", "04");
    io::console::printf("\n\ntesting hex %x", 2017);

while(true); // For debugging
    return 0;
}

}

