#pragma once

#include <nms/math/base.h>

namespace nms::io
{
class File;
class Path;
}

namespace nms::math
{

template<class T, u32 N>
class Array: public View<T, N>
{
public:
    using base  = View<T, N>;
    using Tthis = Array;
    using Tsize = typename base::Tsize;
    using Tdims = typename base::Tdims;
    using Tinfo = typename base::Tinfo;

    static const auto $rank = base::$rank;

#pragma region constructors
    /* default constructor */
    constexpr Array() = default;

    /* constructor */
    explicit Array(const Tdims& dims) : base{ nullptr, dims } {
        const auto cnt = base::count;
        if (cnt != 0) {
            // SIMD require memory aligned (128*8=1024)
            // SSE(64bit~128bit), AVX(256bit~512bit)
            this->data_ = anew<T>(cnt, 256);
        }
    }

    /* constructor */
    explicit Array(const Tsize(&dims)[$rank])
        : Array{ Tdims::from_array(dims) }
    {}

    /* destructor */
    ~Array() {
        // try delete
        if (this->data_ != nullptr) {
            adel(this->data_);
            this->data_ = nullptr;
        }
    }

    /* move constructor */
    Array(Array&& rhs) noexcept
        : base{ rhs }
    {
        rhs.data_ = nullptr;
    }

    /* copy assign operator is disabled */
    Array& operator=(const Array&) = delete;
    Array& operator=(Array&&     ) = delete;

    /* get copies */
    Array dup() const {
        const auto data_dims = this->dims;
        Array tmp(data_dims);

        const auto data_cnt = this->count;
        const auto src_data = this->data;
        const auto dst_data = tmp.data;
        if (data_cnt != 0) {
            mcpy(dst_data, src_data, data_cnt);
        }
        return tmp;
    }
#pragma endregion

#pragma region methods
    Array& resize(const Tsize(&dims)[$rank]) {
        const auto olddims = this->dims;
        const auto newdims = Tdims::from_array(dims);

        if (olddims != newdims) {
            this->~Array();
            new(this)Array({ newdims });
        }
        return *this;
    }

    Array& clear() {
        // if not empty:
        if (this->data_ != nullptr) {
            adel(this->data_);
            this->base::operator=(base{});
        }
        return *this;
    }
#pragma endregion

#pragma region save/load
    void save(io::File& file) const {
        return this->saveFile(file);
    }

    static auto load(const io::File& file) {
        return Array::loadFile(file);
    }

    void save(const io::Path& path) const {
        return this->savePath<io::File>(path);
    }

    static auto load(const io::Path& path) {
        return Array::loadPath<io::File>(path);
    }
#pragma endregion

protected:
    Array(const Array& rhs)
        : Array{ rhs.size() } {
        *this <<= rhs;
    }

private:
    template<class File>
    void saveFile(File& file) const {
        const typename base::Tinfo data_info = this->$info;
        const typename base::Tdims data_dims = this->dims;

        file.write(&data_info, 1);
        file.write(&data_dims, 1);
        file.write(this->data, this->count);
    }

    template<class File>
    static auto loadFile(const File& file) {
        typename base::Tinfo data_info;
        typename base::Tdims data_dims;

        file.read(&data_info, 1);
        if (data_info != base::$info) {
            NMS_THROW(Eunexpect<Tinfo>{base::$info, data_info});
        }

        file.read(&data_dims, 1);

        Array tmp(data_dims);
        file.read(tmp.data, tmp.count);
        return tmp;
    }

    template<class File, class Path>
    void savePath(const Path& path) const {
        auto file = File::open_for_write(path);
        saveFile(file);
    }

    template<class File, class Path>
    static auto loadPath(const Path& path) {
        auto file = File::open_for_read(path);
        return Array::loadFile(file);
    }
};

}
