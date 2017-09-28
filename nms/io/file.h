#pragma once

#include <nms/core.h>
#include <nms/io/path.h>

namespace nms::io
{

#ifndef NMS_BUILD
struct fid_st;
using fid_t = fid_st*;
#else
using fid_t = ::FILE*;
#endif

enum class FileMode
{
    Read    = 0x0,      /// open for read, file must exists
    Write   = 0x1,      /// open for write, file will trunk.
    Append  = 0x2,      /// open for append
};

enum class FileType
{
    Binary,
    Text
};

class Exception: public Iexception
{};

class ENotEnough: public Exception
{};

struct IFile
{
#pragma region data
    fid_t _thrd = nullptr;   // the FILE* object
#pragma endregion

#pragma region property
    using Tid = i32;
    NMS_API Tid get_id() const;

    /*! file descriptor */
    __declspec(property(get=get_id)) Tid id;

    using Tsize = u64;
    NMS_API Tsize get_size() const;

    /*! file size in bytes. */
    __declspec(property(get=get_size)) Tsize size;

    operator bool() const noexcept {
        return _thrd != nullptr;
    }
#pragma endregion

#pragma region method
    NMS_API void reopen(const Path& path, FileMode mode);
    NMS_API void close();
    NMS_API void flush() const;
#pragma endregion

#pragma region for-subclass
protected:
    NMS_API Tsize _read (void*       buffer, Tsize size, Tsize count) const;
    NMS_API Tsize _write(const void* buffer, Tsize size, Tsize count);
#pragma endregion
};

class File: public IFile
{
public:
    using base = IFile;

#pragma region constructors
private:
    File(const File&)             = delete;
    File& operator= (const File&) = delete;

public:
    File() noexcept {
        _thrd = nullptr;
    }

    File(File&& rhs) noexcept : base(rhs) {
        rhs._thrd = nullptr;
    }

    virtual ~File() {
        base::close();
    }

    static File open_for_read(const Path& path) {
        File file;
        file.reopen(path, FileMode::Read);
        return file;
    }

    static File open_for_write(const Path& path) {
        File file;
        file.reopen(path, FileMode::Write);
        return file;
    }
#pragma endregion

#pragma region method
public:
    template<typename T>
    Tsize read(T data[], Tsize count) const {
        static_assert($is<$pod, T>, "nms.io.File.read: unexpect tparam, should be POD");
        const auto ret  = base::_read(data, sizeof(T), count);
        return ret;
    }

    template<typename T>
    Tsize write(const T data[], Tsize count) {
        static_assert($is<$pod, T>, "nms.io.File.read: unexpect tparam, should be POD");
        const auto ret = base::_write(data, sizeof(T), count);
        return ret;
    }
#pragma endregion
};

class TxtFile: public IFile
{
public:
    using base = IFile;

#pragma region constructors
private:
    TxtFile(const TxtFile&)             = delete;
    TxtFile& operator= (const TxtFile&) = delete;

public:
    TxtFile() noexcept {
        _thrd = nullptr;
    }

    TxtFile(TxtFile&& rhs) noexcept: base(rhs) {
        rhs._thrd = nullptr;
    }

    virtual ~TxtFile() {
        base::close();
    }

    static TxtFile open_for_read(const Path& path) {
        TxtFile file;
        file.reopen(path, FileMode::Read);
        return file;
    }

    static TxtFile open_for_write(const Path& path) {
        TxtFile file;
        file.reopen(path, FileMode::Write);
        return file;
    }

#pragma endregion

#pragma region method
public:
    void reopen(const io::Path& path, FileMode mode) {
        base::reopen(path, mode);
        this->set_txt_mode();
    }

    Tsize read(IString& outbuf, Tsize count) const {
        const auto oldcnt = outbuf.count;
        outbuf.resize(u32(oldcnt+count));

        const auto ret  = base::_read(outbuf.data + oldcnt, 1, count);
        const auto newcnt = oldcnt + u32(ret);
        outbuf._resize(newcnt);

        return ret;
    }

    Tsize readall(IString& outbuf) {
        const auto file_size = base::size;
        return read(outbuf, file_size);
    }

    Tsize write(const str& inbuf) {
        return base::_write(inbuf.data, 1, inbuf.count);
    }

    template<class ...U>
    Tsize writef(const str& fmt, const U& ...args) {
        auto s = format(fmt, args...);
        return write(s);
    }

private:
    NMS_API void set_txt_mode();
#pragma endregion
};

using BinFile = File;

NMS_API u64   fsize(const Path& path);
NMS_API u64   fsize(int fid);

}
