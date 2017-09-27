#pragma once

#include <nms/core/vec.h>
#include <nms/core/trait.h>

namespace nms
{

#pragma region format: pre-define
struct FormatStyle;

template<typename T>
void sformat(IString& outbuf, const FormatStyle& style, const T& value);
#pragma endregion

#pragma region scalar
template<typename T>
struct Scalar
{
    constexpr static const u32 $rank = 0;

    using Trank = u32;
    using Tsize = u32;
    using Tdata = T;
    using Tview = Scalar;
    using Tdims = Vec<Tsize, 0>;

    T val;

    __declspec(property(get=get_dims)) Tdims dims;

    Tdims get_dims() const noexcept {
        return {0};
    }

    template<typename ...I>
    const T& operator()(I .../*idx*/) const noexcept {
        return val;
    }

    template<typename ...I>
    T& operator()(I .../*idx*/) noexcept {
        return val;
    }
};
#pragma endregion

#pragma region viewinfo

struct ViewInfo
{
    char mask;
    char type;
    char size;
    char rank;

    friend bool operator==(const ViewInfo& a, const ViewInfo& b) {
        return a.mask == b.mask && a.type == b.type && a.size == b.size && a.rank == b.rank;
    }

    friend bool operator!=(const ViewInfo& a, const ViewInfo& b) {
        return !(a==b);
    }

    NMS_API void sformat(IString& buf) const;
};

#pragma endregion

template<typename T, u32 N>
struct View
{
#pragma region defines
    static constexpr const auto $rank = N;

    using Tdata     = T;
    using Tthis     = View;
    using Trank     = u32;
    using Tsize     = u32;
    using Tstep     = i32;
    using Tdims     = Vec<Tsize, $rank>;
    using Tidxs     = Vec<Tstep, $rank>;

    using Tinfo     = ViewInfo;
    using Tview     = View;

    template<typename U, u32 M>
    friend struct View;

    /*! type-info */
    constexpr static Tinfo $info ={
        '$',
        $is<$uint, T> ? 'u' : $is<$sint, T> ? 'i' : $is<$float, T> ? 'f' : '?',
        char('0' + sizeof(T)),
        char('0' + N)
    };

#pragma endregion

#pragma region constructors
    /*! default constructor */
    constexpr View() noexcept
        : data_{nullptr}, dims_{0}, step_{0}
    { }

    /*! default destructor */
    ~View() = default;

    /*! construct view with data, size, step */
    constexpr View(Tdata* data, const Tdims& size, const Tidxs& step) noexcept
        : data_{ data }, dims_{ size }, step_(step)
    {}

     /*! construct view with data, size */
    constexpr View(Tdata* data, const Tdims& size) noexcept
        : data_{ data }, dims_{ size }, step_{ _make_step_from_size(size.data) }
    {}

    /*! construct view with data, size, step */
    constexpr View(Tdata* data, const Tsize(&size)[$rank], const Tstep(&step)[$rank]) noexcept
        : data_{ data }, dims_{ Tdims::from_array(size) }, step_{ Tidxs::from_array(step) }
    {}

    /*! construct view with data, size */
    constexpr View(Tdata* data, const Tsize(&size)[$rank]) noexcept
        : data_{ data }, dims_{ Tdims::from_array(size) }, step_{ _make_step_from_size(size) }
    {}

    /*! convert to const View */
    operator View<const Tdata, $rank>() const noexcept {
        return { data_, dims_, step_ };
    }

#pragma endregion

#pragma region properties
    /*! data storage of the view */
    __declspec(property(get=get_data)) Tdata* data;

    Tdata* get_data() noexcept {
        return data_;
    }

    const Tdata* get_data() const noexcept {
        return data_;
    }

    /*! size of the view */
    __declspec(property(get=get_size)) Tdims size;

    /*! dimension of the view */
    __declspec(property(get=get_size)) Tdims dims;

    Tdims get_size() const noexcept {
        return dims_;
    }

    /*! element step of the view */
    __declspec(property(get=get_step)) Tidxs step;

    Tidxs get_step() const noexcept {
        return step_;
    }

    /*! get total elements count */
    __declspec(property(get=get_count)) Tsize count;

    Tsize get_count() const noexcept {
        return iprod($index_seq<$rank>, dims_);
    }


#pragma endregion

#pragma region access
    /*!
     * access specified element
     */
    template<typename ...Tidx>
    __forceinline const Tdata& at(Tidx ...idx) const noexcept {
        static_assert($all_is<$int,Tidx...>,    "unexpect type");
        static_assert(sizeof...(Tidx)==$rank,   "unexpect arguments count");
        return data_[_offset_of_dims($index_seq<$rank>, idx...)];
    }

    /*!
     * access specified element
     */
    template<typename ...Tidx>
    __forceinline Tdata& at(Tidx ...idx) noexcept {
        static_assert($all_is<$int, Tidx...>,  "unexpect type");
        static_assert(sizeof...(Tidx)==$rank,  "unexpect arguments count");
        return data_[_offset_of_dims($index_seq<$rank>, idx...)];
    }

    /*!
     * access specified element
     * @param idx indexs
     * @see at
     */
    template<typename ...I>
    __forceinline Tdata& operator()(I ...idx) noexcept {
        return at(idx...);
    }

    /*!
     * access specified element
     * @param idx indexs
     * @see at
     */
    template<typename ...I>
    __forceinline const Tdata& operator()(I ...idx) const noexcept {
        return at(idx...);
    }

#pragma endregion

#pragma region slice
    /*! slice */
    template<typename ...Tidx, u32 ...Icnt, u32 Idim=Tcnt<(Icnt!=1)...>::$value>
    View<const Tdata, Idim> slice(const Tidx(&...ids)[Icnt]) const noexcept {
#ifndef NMS_CC_INTELLISENSE
        auto tmp = _slice(Tseq<$rank>{}, ids...);
        auto ret = tmp._select_dims(Tindex_if<(Icnt!=1)...>{});
        return ret;
#else
        return {};
#endif
    }

    /*! slice */
    template<typename ...Tidx, u32 ...Icnt, u32 Idim=Tcnt<(Icnt!=1)...>::$value>
    View<Tdata, Idim> slice(const Tidx(&...ids)[Icnt]) noexcept {
#ifndef NMS_CC_INTELLISENSE
        auto tmp = _slice(Tindex_seq<$rank>{}, ids...);
        auto ret = tmp._select_dims(Tindex_if<(Icnt!=1)...>{});
        return ret;
#else
        return {};
#endif
    }

    /*!
     * slice the view
     * @param ids sections
     * @see slice
     */
    template<typename ...Tidx, u32 ...Icnt >
    auto operator()(const Tidx(&...ids)[Icnt]) const noexcept {
        return this->slice(ids...);
    }

    /*!
     * slice the view
     * @param ids sections
     * @see slice
     */
    template<typename ...Tidx, u32 ...Icnt >
    auto operator()(const Tidx(&...ids)[Icnt]) noexcept {
        return this->slice(ids...);
    }

#pragma endregion

#pragma region methods
    View permute(const u32(&order)[$rank]) const noexcept {
        Tdims   new_size;
        Tidxs   new_step;

        for (Trank i = 0; i < $rank; ++i) {
            new_size[i] = this->dims_[order[i]];
            new_step[i] = this->step_[order[i]];
        }

        return { data_, new_size, new_step };
    }

    template<u32 M>
    View<Tdata, M> reshape(const u32(&new_size)[M]) const noexcept {
        return { data_, new_size };
    }

    template<u32 M>
    View<Tdata, M> reshape(const u32(&new_size)[M], const u32(&new_step)[M]) const noexcept {
        return { data_, new_size, new_step };
    }

#pragma endregion

#pragma region format
    void sformat(IString& buf, const FormatStyle& style) const {
        _sformat(buf, style, *this);
    }
#pragma endregion

protected:
    Tdata*  data_;
    Tdims   dims_;
    Tidxs   step_;

#pragma region index_of
    template<u32 Idim, typename Tidx>
    __forceinline constexpr Tsize _index_of_dim(Tidx idx) const noexcept {
        return idx >= 0 ? Tsize(idx) : dims_[Idim] - Tsize(0 - idx);
    }
#pragma endregion

#pragma region _offset_of_dim
    template<u32 Idim, typename Tidx>
    __forceinline constexpr Tstep _offset_of_dim(Tidx idx) const noexcept {
        return Tstep(idx) * step_[Idim];
    }

    template<u32 ...Idim, typename ...Tidx>
    __forceinline constexpr Tstep _offset_of_dims(Tu32<Idim...>, Tidx ...idx) const noexcept {
        return sum(_offset_of_dim<Idim>(idx)...);
    }
#pragma endregion

#pragma region dims_of
    template<u32 Idim, typename Tidx>
    Tsize _size_of_dim(const Tidx(&)[1]) const noexcept {
        return Tsize(1u);
    }

    template<u32 Idim, typename Tidx>
    Tsize _size_of_dim(const Tidx(&idx)[2]) const noexcept {
        const Tsize s0 = _index_of_dim<Idim>(idx[0]);
        const Tsize s1 = _index_of_dim<Idim>(idx[1]);
        return s1 - s0 + 1;
    }
#pragma endregion

#pragma region _slice
    /* slice */
    template<u32 ...Idim, typename ...Tidx, u32 ...Icnt>
    View<Tdata, $rank> _slice(Tu32<Idim...>, const Tidx(&...s)[Icnt]) noexcept {
        static_assert($all<(Icnt <= 2)...>, "unexpect array size");
        return{ data_ + _offset_of_dims(Tu32<Idim...>{}, s[0]...), { _size_of_dim<Idim>(s)...}, step_.data };
    }

    /* slice */
    template<u32 ...Idim, typename ...Tidx, u32 ...Icnt>
    View<const Tdata, $rank> _slice(Tu32<Idim...>, const Tidx(&...s)[Icnt]) const noexcept {
        static_assert($all<(Icnt <= 2)...>, "unexpect array size");
        return { data_ + _offset_of_dims(Tu32<Idim...>{}, s[0]...), { _size_of_dim<Idim>(s)...}, step_.data };
    }

    /* select dim */
    template<u32 ...I>
    auto _select_dims(Tu32<I...>) const noexcept {
        return View<Tdata, u32(sizeof...(I))>{ data_, { dims_[I]... }, { step_[I]... } };
    }
#pragma endregion


private:
    static constexpr Tidxs _make_step_from_size(const Tsize(&size)[$rank]) noexcept {
        return _make_step_from_dims_seq($index_seq<$rank>, size);
    }

    template<u32 ...Idim>
    static constexpr Tidxs _make_step_from_dims_seq(Tu32<Idim...>, const Tsize(&size)[$rank]) noexcept {
        return { Tstep(iprod($index_seq<Idim>, size))... };
    }

    template<typename Tbuff, typename Tstyle>
    static void _sformat(Tbuff& outbuf, const Tstyle& style, const View<Tdata, 1>& view) {
        const auto view_dims = view.dims;

        outbuf += "\n";
        for (Tsize nx = 0; nx < view_dims[0]; nx++) {
            nms::sformat(outbuf, FormatStyle{ '>', 0, 3, 0 }, nx);
            outbuf += "| ";
            const auto& val = view(nx);
            nms::sformat(outbuf, style, val);
            outbuf += "\n";
        }
    }

    template<typename Tbuff, typename Tstyle>
    static void _sformat(Tbuff& outbuf, const Tstyle& style, const View<Tdata, 2>& view) {
        const auto view_dims = view.dims;

        outbuf += "\n";
        for (Tsize nx = 0; nx < view_dims[0]; nx++) {
            nms::sformat(outbuf, FormatStyle{ '>', 0, 3, 0 }, nx);
            outbuf += "| ";
            for (Tsize ny = 0; ny < view_dims[1]; ny++) {
                const auto& val = view(nx, ny);
                nms::sformat(outbuf, style, val);
                if (ny + 1 != view_dims[1]) outbuf += ", ";
            }
            outbuf += "\n";
        }
    }
};

template<typename T>
struct View<T, 0>
{
#pragma region defines
    constexpr static auto $rank = 1u;

    using Tdata = T;
    using Trank = u32;
    using Tsize = u32;
    using Tstep = i32;
    using Tinfo = ViewInfo;

    /*! type-info */
    constexpr static Tinfo $info ={
        '$',
        $is<$uint, T> ? 'u' : $is<$sint, T> ? 'i' : $is<$float, T> ? 'f' : '?',
        char('0' + sizeof(T)),
        char('0' + 0)
    };

    template<typename U, u32 M>
    friend struct View;
#pragma endregion

#pragma region constructors
    /* default constructor */
    __forceinline constexpr View() noexcept
        : data_{nullptr}, size_{0}, capacity_{0}
    { }

    /* default destructor */
    __forceinline ~View()
    { }

    /*! construct view with data, size*/
    __forceinline constexpr View(Tdata* data, Tsize size) noexcept
        : data_{ data }, size_{ size }, capacity_{0}
    {}

    /*! construct view with array */
    template<u32 Isize>
    __forceinline constexpr View(Tdata(&data)[Isize]) noexcept
        : data_{ data }, size_{ ($is<char, T> || $is<const char, T>) ? Isize - 1 : Isize }, capacity_{0}
    {}

#pragma endregion

#pragma region properties
    /*! data pointer */
    __declspec(property(get=get_data)) Tdata* data;

    __forceinline Tdata* get_data() noexcept {
        return data_;
    }

    __forceinline constexpr const Tdata* get_data() const noexcept {
        return data_;
    }

    /*! get n-dim size */
    __declspec(property(get=get_size)) Tsize size;

    __forceinline constexpr Tsize get_size() const noexcept {
        return size_;
    }

    /*! get total elements count */
    __declspec(property(get=get_size)) Tsize count;

    /*! get capicity */
    __declspec(property(get=get_capacity)) Tsize capacity;

    __forceinline constexpr Tsize get_capacity() const noexcept {
        return capacity_;
    }
#pragma endregion

#pragma region access
    /*! access specified element */
    template<typename Tidx>
    __forceinline const Tdata& at(Tidx idx) const noexcept {
        return data_[idx];
    }

    /*! access specified element */
    template<typename Tidx>
    __forceinline Tdata& at(Tidx idx) noexcept {
        return data_[idx];
    }

    /*! access specified element */
    template<typename Tidx>
    __forceinline auto& operator()(Tidx idx) noexcept {
        return at(idx);
    }

    /*! access specified element */
    template<typename Tidx>
    __forceinline auto& operator()(Tidx idx) const noexcept {
        return at(idx);
    }

    /*! access specified element */
    template<typename Tidx>
    __forceinline auto& operator[](Tidx idx) noexcept {
        return at(idx);
    }

    /*! access specified element */
    template<typename Tidx>
    __forceinline auto& operator[](Tidx idx) const noexcept {
        return at(idx);
    }
#pragma endregion

#pragma region iterator
    // iterator type
    using Titr = Tdata*;

    // const-iterator type
    using Kitr = const Tdata*;

    __forceinline Titr begin() {
        return data_;
    }

    __forceinline Titr end() {
        return data_ + size_;
    }

    __forceinline Kitr begin() const {
        return data_;
    }

    __forceinline Kitr end() const {
        return data_ + size_;
    }

    /*! contains */
    template<typename U>
    bool contains(const U& val) const {
        const auto idx = find(val);
        return idx != end();
    }

    /*! find */
    template<typename U>
    Titr find(const U& val) {
        for (auto itr = begin(); itr != end(); ++itr) {
            if (*itr == val) {
                return itr;
            }
        }
        return end();
    }

    /*! find */
    template<typename U>
    Kitr find(const U& val) const {
        for (auto itr = begin(); itr != end(); ++itr) {
            if (*itr == val) {
                return itr;
            }
        }
        return end();
    }

    /*! replace */
    template<typename Tsrc, typename Tdst>
    void replace(const Tsrc& src, Tdst&& dst) {
        for (auto& element: *this) {
            if (element == src) {
                element = fwd<Tdst>(dst);
            }
        }
    }

#pragma endregion

#pragma region slice
    /*! slice */
    template<typename Tidx>
    View<Tdata> slice(Tidx first, Tidx last) noexcept {
        return { data_ + index_of(first), _size_of_dim(first, last) };
    }

    /*! slice */
    template<typename Tidx>
    View<const Tdata> slice(Tidx first, Tidx last) const noexcept {
        return { data_ + index_of(first), _size_of_dim(first, last) };
    }

    /*! slice */
    template<typename Tidx>
    View<Tdata> operator()(Tidx first, Tidx last) noexcept {
        return slice(first, last);
    }

    /*! slice */
    template<typename Tidx>
    View<const Tdata> operator()(Tidx first, Tidx last) const noexcept {
        return slice(first, last);
    }
#pragma endregion

#pragma region method
    int compare(const View<T>& b) const {
        auto& a = *this;

        if (&a == &b) {
            return 0;
        }

        const auto na = a.count;
        const auto nb = b.count;

        if (na != nb) {
            return na > nb ? +1 : -1;
        }

        for (Tsize i = 0; i < na; ++i) {
            if (a[i] != b[i]) {
                return a[i] > b[i] ? +1 : -1;
            }
        }
        return 0;
    }

    bool operator== (const View& v) const {
        return compare(v) == 0;
    }

    bool operator!= (const View& v) const {
        return compare(v) != 0;
    }

#pragma endregion

#pragma region format
    void sformat(IString& buf, const FormatStyle& style) const {
        _sformat(buf, style);
    }
#pragma endregion

protected:
    Tdata*  data_;
    Tsize   size_;
    Tsize   capacity_;

    template<typename Tidx>
    Tsize index_of(Tidx idx) const {
        return idx >= 0 ? Tsize(idx) : size_ - Tsize(0-idx);
    }

    template<typename Tidx>
    Tsize _size_of_dim(Tidx i0 , Tidx i1) const {
        const auto s0 = index_of(i0);
        const auto s1 = index_of(i1);
        return s1 - s0 + 1;
    }

    template<typename Tbuff, typename Tstyle>
    void _sformat(Tbuff& outbuf, const Tstyle& style) const {
        outbuf += '[';

        for (Tsize i = 0u; i < this->size_; ++i) {
            nms::sformat(outbuf, style, data_[i]);
            if (i + 1 != this->size_) {
                outbuf += ", ";
            }
        }

        outbuf += ']';
    }
};

}
