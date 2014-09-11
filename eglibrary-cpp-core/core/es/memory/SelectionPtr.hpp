#ifndef es_SELECTIONPTR_HPP_
#define es_SELECTIONPTR_HPP_

#include    "es/eglibrary.hpp"

namespace es {

/*
 * スマートポインタのダウンキャストを行う
 */
template<typename T, typename T2>
std_shared_ptr<T> downcast(const std_shared_ptr<T2> &obj) {
    if (!obj) {
        return std_shared_ptr<T>();
    }

    try {
        return ::std::dynamic_pointer_cast < T > (obj);
    } catch (...) {
        jcalertf("catch downcast error(%0xx)", obj.get());
        return std_shared_ptr<T>();
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
class SelectionPtr {
    /**
     * 生ポインタ
     */
    T *raw;

    /**
     *
     */
    std_shared_ptr<T> shared;

    /**
     *
     */
    std_weak_ptr<T> weak;

public:
    SelectionPtr() {
        raw = NULL;
    }

    /**
     * コピーコンストラクタ
     */
    SelectionPtr(const SelectionPtr &origin) {
        this->raw = origin.raw;
        this->shared = origin.shared;
        this->weak = origin.weak;
    }

    /**
     * 生ポインタで生成
     */
    template<typename T2>
    SelectionPtr(const T2 *p) {
        this->raw = (T*) p;
    }

    /**
     *
     */
    template<typename T2>
    SelectionPtr(const std_shared_ptr<T2> &sp) {
        this->raw = nullptr;
        this->shared = sp;
    }

    /**
     *
     */
    template<typename T2>
    SelectionPtr(const std_weak_ptr<T2> &wp) {
        this->raw = nullptr;
        this->weak = wp;
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
    void reset(const std_shared_ptr<T> &p) {
        reset();
        shared = p;
    }

    /**
     * ポインタをリセットする
     */
    void reset(const std_weak_ptr<T> &p) {
        reset();
        weak = p;
    }

    /**
     * 特定の型へダウンキャストする
     */
    template<typename DC>
    SelectionPtr<DC> downcast() const {
        SelectionPtr<DC> result;

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
    bool operator==(const SelectionPtr &p) const {
        return get() == dynamic_cast<T*>(p.get());
    }

    /**
     * 異なる値である場合はtrue
     */
    bool operator!=(const SelectionPtr &p) const {
        return get() != dynamic_cast<T*>(p.get());
    }

    operator bool() const {
        return exist();
    }

    bool operator!() const {
        return !exist();
    }

    SelectionPtr<T>& operator=(const SelectionPtr<T> &cpy) {
        this->raw = cpy.raw;
        this->shared = cpy.shared;
        this->weak = cpy.weak;
        return (*this);
    }
};

/**
 * selection_ptr
 */
#define es_selection_ptr     ::es::SelectionPtr

}

#endif /* SELECTIONPTR_HPP_ */
