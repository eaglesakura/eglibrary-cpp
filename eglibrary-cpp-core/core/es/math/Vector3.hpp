#ifndef es_math_VECTOR3_HPP_
#define es_math_VECTOR3_HPP_

#include    "es/eglibrary.hpp"
#include    <math.h>

namespace es {

template<typename T>
struct _Vector3 {
    /**
     * X座標を示す
     */
    T x;

    /**
     * Y座標を示す
     */
    T y;

    /**
     * Z座標を示す
     */
    T z;

    /**
     * 各座標を指定する
     */
    _Vector3(const T _x = 0, const T _y = 0, const T _z = 0) {
        x = _x;
        y = _y;
        z = _z;
    }

    _Vector3(const _Vector3<T> &origin) {
        x = origin.x;
        y = origin.y;
        z = origin.z;
    }

    /**
     * 加算を行う
     */
    _Vector3<T>& operator+=(const _Vector3 &v) {
        x += v.x;
        y += v.y;
        z += v.z;
        return (*this);
    }

    /**
     * 減算を行う
     */
    _Vector3<T>& operator-=(const _Vector3 &v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return (*this);
    }

    /**
     * スカラー演算を行う
     */
    _Vector3<T>& operator*=(const T scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return (*this);
    }

    /**
     * 加算を行う
     */
    _Vector3<T> operator+(const _Vector3 &v) const {
        return _Vector3<T>(x + v.x, y + v.y, z + v.z);
    }

    /**
     * 減算を行う
     */
    _Vector3<T> operator-(const _Vector3<T> &v) const {
        return _Vector3<T>(x - v.x, y - v.y, z - v.z);
    }

    /**
     * 除算を行う
     */
    _Vector3<T> operator/=(const T div) {
        x /= div;
        y /= div;
        z /= div;
        return (*this);
    }

    /**
     * 符号反転を行う
     */
    _Vector3<T> operator-() const {
        return _Vector3<T>(-x, -y, -z);
    }

    void set(const T _x, const T _y, const T _z) {
        x = _x;
        y = _y;
        z = _z;
    }

    /**
     * 同一性チェック
     */
    bool operator==(const _Vector3<T> &v) const {
        return equals(x, v.x) && equals(y, v.y) && equals(z, v.z);
    }

    /**
     * 同一性チェック
     */
    bool operator!=(const _Vector3<T> &v) const {
        return !equals(x, v.x) || !equals(y, v.y) || !equals(z, v.z);
    }

    /**
     * 長さを取得する。
     *
     * @return
     */
    T length() const {
        return (T) sqrt(((double) x * (double) x) + ((double) y * (double) y) + ((double) z * (double) z));
    }

    /**
     * 外積を取得する。
     *
     *
     * @param v
     * @param result
     * @return
     *
     */
    _Vector3<T>* cross(const _Vector3<T> &v, _Vector3<T> *result) const {
        result->set((y * v.z) - (z * v.y), (z * v.x) - (x * v.z), (x * v.y) - (y * v.x));
        return result;
    }

    /**
     * 内積を取得する。
     *
     *
     * @param v
     * @return
     *
     */
    T dot(const _Vector3<T> &v) const {
        return (x * v.x) + (y * v.y) + (z * v.z);
    }

    /**
     * ベクトルの長さを正規化する
     */
    void normalize() {
        const T _len = length();

        x /= _len;
        y /= _len;
        z /= _len;
    }

    /**
     * 正規化したベクトルを生成する
     */
    _Vector3<T> createNormalized() const {
        const T _len = length();
        return _Vector3(x / _len, y / _len, z / _len);
    }

    /**
     *
     *
     * @param p
     * @return
     */
    T length(const _Vector3<T> &p) const {
        const double tx = x - p.x;
        const double ty = y - p.y;
        const double tz = z - p.z;
        return (T) sqrt((tx * tx) + (ty * ty) + (tz * tz));
    }

    /**
     *
     *
     * @param p
     * @return
     */
    T length(const T px, const T py, const T pz) const {
        const double tx = x - px;
        const double ty = y - py;
        const double tz = z - pz;
        return (T) sqrt((tx * tx) + (ty * ty) + (tz * tz));
    }

    /**
     * 配列風のアクセスを提供する
     */
    T& operator[](const uint index) {
        assert(index < 3);
        return ((T*) this)[index];
    }

    /**
     * 外積を生成する。
     *
     * @return
     *
     */
    static _Vector3<T> createCross(const _Vector3<T> &v0, const _Vector3<T> &v1) {
        return _Vector3<T>((v0.y * v1.z) - (v0.z * v1.y), (v0.z * v1.x) - (v0.x * v1.z), (v0.x * v1.y) - (v0.y * v1.x));
    }

};

/**
 * signed int のベクトル
 */
typedef _Vector3<int> Vector3i;

/**
 * signed int のベクトル
 */
typedef _Vector3<int16_t> Vector3i16;

/**
 * 浮動小数のベクトル
 */
typedef _Vector3<float> Vector3f;

inline bool equals(const Vector3f &v0, const Vector3f &v1) {
    return es::equals(v0.x, v1.x) && es::equals(v0.y, v1.y) && es::equals(v0.z, v1.z);
}

inline bool equals(const Vector3f &v0, const Vector3f &v1, const float _check) {
    return es::equals(v0.x, v1.x, _check) && es::equals(v0.y, v1.y, _check) && es::equals(v0.z, v1.z, _check);
}

/**
 * ２ベクトルを線形補間する
 */
template<typename T>
inline _Vector3<T>* lerp(const _Vector3<T> &before, const _Vector3<T> &after, const float weight, _Vector3<T> *result) {

    const T x = (after.x * weight) + (before.x * (1.0f - weight));
    const T y = (after.y * weight) + (before.y * (1.0f - weight));
    const T z = (after.z * weight) + (before.z * (1.0f - weight));

    result->x = x;
    result->y = y;
    result->z = z;

    return result;
}

}

#endif /* VECTOR3_HPP_ */
