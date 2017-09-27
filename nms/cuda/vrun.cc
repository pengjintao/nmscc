#include <nms/cuda/engine.h>
#include <nms/cuda/vrun.h>
#include <nms/cuda/kernel.h>
#include <nms/io/file.h>

namespace nms::cuda
{

NMS_API Program& Vrun::sProgram() {
    static Program program;

    static auto _init = false;

    if (!_init) {
        _init = true;

        static auto& srcs = program.src_;
        srcs += str(nms_cuda_kernel_src);
    }

    return program;
}

NMS_API Module& Vrun::sModule() {
    static const io::Path src_path = "~/.nms/cuda_default.cu";
    static const io::Path ptx_path = "~/.nms/cuda_default.ptx";
    static auto& program = sProgram();

    static auto _init = false;

    if (!_init) {
        _init = true;

        if (io::exists(ptx_path)) {
            auto ptx_file = io::TxtFile::open_for_read(ptx_path);
            ptx_file.readall(program.ptx_);
        }

        const auto& src_str = program.src();

        if (program.ptx().count == 0) {
            const auto ret = program.compile();

            if (ret) {
                const auto ptx_str = program.ptx();

                io::mkdir("~/.nms");
                if (src_str.count > 0) {
                    auto src_file = io::TxtFile::open_for_write(src_path);
                    src_file.write(src_str);
                }

                if (ptx_str.count > 0) {
                    auto ptx_file = io::TxtFile::open_for_write(ptx_path);
                    ptx_file.write(ptx_str);
                }
            }
        }
    }

    static Module value(program.ptx());
    return value;
}

NMS_API Module::fun_t Vrun::_get_kernel(u32 fid) {
    static auto& mod = sModule();

    char name[64];
    auto count = snprintf(name, sizeof(name), "nms_cuda_foreach_%u", fid);
    auto func  = mod.get_kernel(str(name, u32(count)));
    return func;
}

NMS_API u32 Vrun::_signin_impl(str func, str ret_type, str arg_type) {
    auto& src = sProgram().src_;
    static auto func_id = 0u;

    sformat(src, "__kernel__ void nms_cuda_foreach_{}(\n", func_id);
    sformat(src, "    {} ret,\n", ret_type);
    sformat(src, "    {} arg)\n", arg_type);
    src += "{\n";
    sformat(src, "    nms::cuda::foreach<{}>(ret, arg);\n", func);
    src += "}\n\n";

    return func_id++;
}

}
