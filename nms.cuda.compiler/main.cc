
#include <nms/core.h>
#include <nms/cuda.h>
#include <nms/io.h>
#include <nms/cuda/kernel.h>

using namespace nms;

static void show_help() {
    io::console::writeln("usage: ");
    io::console::writeln("    nms.compiler [src.cu]... -o [dst.ptx]");
}

int main(int argc, const char* argv[]) {
    if (argc < 2) {
        show_help();
        return 0;
    }

    StrView ptx_path("default.ptx");

    cuda::Program program(nms_cuda_kernel_src);

    for (auto i = 1; i < argc; ++i) {
        const auto arg = make_str(argv[i]);

        if ((arg == "-o") && (i + 1 < argc)) {
            ptx_path = make_str(argv[i + 1]);
            i += 2;
            continue;
        }

        const auto src_path = arg;
        if (io::exists(src_path)) {
            sformat(program.src(), "#line 1 \"{}\"\n", src_path);
            const auto src_file = io::TxtFile::open_for_write(src_path);
            src_file.read(program.src(), src_file.size);
        }
        else {
            io::console::writeln("cannot find {}.", src_path);
            return 0;
        }
    }

    auto stat = program.compile();
    if (!stat) {
        return 0;
    }

    auto ptx = program.ptx();
    auto ptx_file = io::TxtFile::open_for_write(ptx_path);
    ptx_file.write(ptx);

    return 0;
}
