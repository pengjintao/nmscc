
#include <nms/test.h>
#include <nms/io/file.h>
#include <nms/io/log.h>

namespace nms::io
{

#pragma region file

NMS_API void IFile::reopen(const Path& path, FileMode mode) {
    // close
    this->close();

    auto smod = "rb";
    switch (mode) {
    case FileMode::Read:      smod = "rb"; break;
    case FileMode::Write:     smod = "wb"; break;
    case FileMode::Append:    smod = "ab"; break;
    default: break;
    }

    const auto real_path = path.path();
    auto ret = ::fopen(real_path.data, smod);

    if (ret == nullptr) {
        auto dir = cwd();
        log::error("nms.io.File: fopen failed\n"
                   "    dir : {}\n"
                   "    path: {}", dir, path);
        NMS_THROW(Esystem{errno});
    }

    _fobj = ret;
}

NMS_API void IFile::close() {
    if (_fobj == nullptr) {
        return;
    }
    ::fclose(_fobj);
    _fobj = nullptr;
}


NMS_API void IFile::flush() const {
    if (_fobj == nullptr) {
        return;
    }
    ::fflush(_fobj);
}

NMS_API IFile::Tid IFile::get_id() const {
#ifdef NMS_OS_WINDOWS
    auto fid = ::_fileno(_fobj);
#else
    auto fid = ::fileno(_fobj);
#endif
    return fid;
}

NMS_API IFile::Tsize IFile::get_size() const {
    if (_fobj == nullptr) {
        return 0;
    }

    const auto file_id   = this->id;
    const auto file_size = fsize(file_id);
    return file_size;
}

NMS_API IFile::Tsize IFile::_read(void* dat, Tsize size, Tsize count) const {
    if (_fobj == nullptr || size == 0 || count == 0) {
        return 0;
    }
    const auto ret = ::fread(dat, size, count, _fobj);
    return ret;
}

NMS_API IFile::Tsize IFile::_write(const void* dat, Tsize size, Tsize n) {
    if (_fobj == nullptr || size == 0 || n == 0) {
        return 0;
    }
    const auto ret = ::fwrite(dat, size, n, _fobj);
    return ret;
}
#pragma endregion

#pragma region TxtFile
NMS_API void TxtFile::set_txt_mode() {
#ifdef NMS_OS_WINDOWS
    auto fid = this->id;
    (void)_setmode(fid, _O_TEXT);
#endif
}
#pragma endregion

#pragma region file-ctl
NMS_API u64 fsize(const Path& path) {
    auto path_str = path.path();
    ::stat_t st;
    ::stat(path_str.data, &st);
    return u64(st.st_size);
}

NMS_API u64 fsize(int fid) {
    ::stat_t st;
    ::fstat(fid, &st);
    return u64(st.st_size);
}
#pragma endregion


#pragma region unittest
nms_test(file) {
    List<u32, 10> data_in;
    List<u32, 10> data_out;

    const str path = "nms.io.file.nums.dat";

    // write
    {
        for (u32 i = 0; i < data_in.$capacity; ++i) {
            data_in += i;
        }

        auto file = File::open_for_write(path);
        file.write(data_in.data, data_in.count);
    }

    // read
    {
        test::assert_true(exists(path));

        auto file = File::open_for_read(path);
        test::assert_true(file.size == data_out.$capacity *sizeof(u32));

        file.read(data_out.data, data_out.$capacity);
        data_out._resize(data_out.$capacity);

        for (u32 i = 0; i < data_out.$capacity; ++i) {
            test::assert_eq(data_in[i], data_out[i]);
        }
    }

    io::remove(path);

}
#pragma endregion

}
