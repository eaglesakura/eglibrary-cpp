#pragma once

#include    "es/eglibrary.hpp"

namespace es {

/*
 * スマートポインタのダウンキャストを行う
 */
template<typename T, typename T2>
::std::shared_ptr<T> downcast(const ::std::shared_ptr<T2> &obj) {
    if (!obj) {
        return ::std::shared_ptr<T>();
    }

    try {
        return ::std::dynamic_pointer_cast < T > (obj);
    } catch (...) {
        eslog("catch downcast error(%0xx)", obj.get());
        return ::std::shared_ptr<T>();
    }
}

/**
 * 通常のポインタ、スマートポインタ、ウィークポインタのいずれかを選んで設定する。
 * jc_sp/jc_wpを利用する。
 * 生ポインタは、例えば最上位クラス等、オブジェクトの生存を設計上完全にコントロール可能な場合に利用する。
 *
 * 利用する側は通常のポインタのように振る舞う。
 * 例えば、ライブラリ内のリスナポインタ保持等、個々のアプリごとに設計思想が違う場合に利用する。
 */
template<typename T>
class selection_ptr {
    /**
     * 生ポインタ
     */
    T *raw;

    /**
     *
     */
    ::std::shared_ptr<T> shared;

    /**
     *
     */
    std::weak_ptr<T> weak;

public:
    selection_ptr() {
        raw = NULL;
    }

    /**
     * コピーコンストラクタ
     */
    selection_ptr(const selection_ptr &origin) {
        this->raw = origin.raw;
        this->shared = origin.shared;
        this->weak = origin.weak;
    }

    /**
     * 生ポインタで生成
     */
    template<typename T2>
    selection_ptr(const T2 *p) {
        this->raw = (T*) p;
    }

    /**
     *
     */
    template<typename T2>
    selection_ptr(const ::std::shared_ptr<T2> &shared) {
        this->raw = nullptr;
        this->shared = shared;
    }

    /**
     *
     */
    template<typename T2>
    selection_ptr(const std::weak_ptr<T2> &weak) {
        this->raw = nullptr;
        this->weak = weak;
    }

    /**
     * ポインタが有効であればtrueを返す
     */
    bool exist() const {
        return (raw || (bool) shared || !weak.expired());
    }

    /**
     * 生ポインタを取得する
     */
    T* get() {
        if (raw) {
            return raw;
        } else if (shared) {
            return shared.get();
        } else {
            return weak.lock().get();
        }
    }

    /**
     * 生ポインタを取得する
     */
    T* get() const {
        if (raw) {
            return raw;
        } else if (shared) {
            return shared.get();
        } else {
            return weak.lock().get();
        }
    }

    /**
     * アロー演算子
     */
    T* operator->() {
        T* result = get();
        assert(result != NULL);
        return result;
    }

    /**
     * アロー演算子
     */
    const T* operator->() const {
        T* result = get();
        assert(result != NULL);
        return result;
    }

    /**
     * ポインタをリセットする
     */
    void reset() {
        raw = NULL;
        weak.reset();
        shared.reset();
    }

    /**
     * ポインタをリセットする
     */
    void reset(const T *p) {
        reset();
        raw = p;
    }

    /**
     * ポインタをリセットする
     */
    void reset(const ::std::shared_ptr<T> &p) {
        reset();
        shared = p;
    }

    /**
     * ポインタをリセットする
     */
    void reset(const std::weak_ptr<T> &p) {
        reset();
        weak = p;
    }

    /**
     * 特定の型へダウンキャストする
     */
    template<typename DC>
    selection_ptr<DC> downcast() const {
        selection_ptr<DC> result;

        if (raw) {
            result.raw = dynamic_cast<DC>(raw);
        } else if (shared) {
            result.shared = es::downcast<DC>(shared);
        } else {
            result.weak = es::downcast<DC>(weak.lock());
        }

        return result;
    }

    /**
     * 同値である場合はtrue
     */
    bool operator==(const selection_ptr &p) const {
        return get() == dynamic_cast<T*>(p.get());
    }

    /**
     * 異なる値である場合はtrue
     */
    bool operator!=(const selection_ptr &p) const {
        return get() != dynamic_cast<T*>(p.get());
    }

    operator bool() const {
        return exist();
    }

    bool operator!() const {
        return !exist();
    }

    selection_ptr<T>& operator=(const selection_ptr<T> &cpy) {
        this->raw = cpy.raw;
        this->shared = cpy.shared;
        this->weak = cpy.weak;
        return (*this);
    }
};

}
