#pragma once

#include    "IApplicationFragment.h"
#include    <vector>

namespace es {

/**
 * フラグメント管理を行う
 */
class ApplicationFragmentController : public Object {
    typedef typename std::vector<SelectionPtr<IApplicationFragment> > fragment_container;

    mutable es_mutex transactionLock;

//    jcmutex lifecycleLock;

    /**
     * 管理中のフラグメント
     */
    fragment_container fragments;

    /**
     * フラグメントの追加リクエスト
     */
    fragment_container requestAddFragment;

    /**
     * フラグメントの削除リクエスト
     */
    fragment_container requestRemoveFragment;

    BaseApplication *application;
protected:
    virtual void onAddFragment(SelectionPtr<IApplicationFragment> fragment) {
        assert(fragment);

        fragment->onAttach(this);
        fragment->onResume();

        fragments.push_back(fragment);
    }

    virtual void onRemoveFragment(SelectionPtr<IApplicationFragment> fragment) {
        assert(fragment);

        fragment->onPause();
        fragment->onDetatch();

        auto itr = fragments.begin(), end = fragments.end();
        while (itr != end) {
            if ((*itr) == fragment) {
                fragments.erase(itr);
                return;
            }
            ++itr;
        }
    }

public:
    ApplicationFragmentController(BaseApplication *app) {
        this->application = app;
    }

    virtual ~ApplicationFragmentController() {
    }

    /**
     * フラグメント追加を行う
     */
    virtual void addFragment(SelectionPtr<IApplicationFragment> fragment) {
        es_mutex_lock lock(transactionLock);

        requestAddFragment.push_back(fragment);
    }

    /**
     * フラグメントの削除を行う
     */
    virtual void removeFragment(SelectionPtr<IApplicationFragment> fragment) {
        es_mutex_lock lock(transactionLock);

        requestRemoveFragment.push_back(fragment);

    }

    /**
     * 追加・削除リクエストの処理を行う
     */
    virtual void commit() {
        es_mutex_lock lock(transactionLock);

        // 追加リストを処理する
        if (!requestAddFragment.empty()) {
            auto itr = requestAddFragment.begin(), end = requestAddFragment.end();
            while (itr != end) {
                onAddFragment(*itr);
                ++itr;
            }

            requestAddFragment.clear();
        }

        // 削除リストを処理する
        if (!requestRemoveFragment.empty()) {
            auto itr = requestRemoveFragment.begin(), end = requestRemoveFragment.end();
            while (itr != end) {
                onRemoveFragment(*itr);
                ++itr;
            }

            requestRemoveFragment.clear();
        }
    }

    /**
     * フラグメント復旧を行う
     */
    virtual void onSurfaceResized(const uint width, const uint height) {
        auto itr = fragments.begin(), end = fragments.end();

        while (itr != end) {
            (*itr)->onSurfaceResized(width, height);
            ++itr;
        }
    }

    /**
     * フラグメント復旧を行う
     */
    virtual void onResume() {
        auto itr = fragments.begin(), end = fragments.end();

        while (itr != end) {
            (*itr)->onResume();
            ++itr;
        }
    }

    /**
     * 毎フレーム更新を行う
     */
    virtual void onUpdate() {
        auto itr = fragments.begin(), end = fragments.end();

        while (itr != end) {
            (*itr)->onUpdate();
            ++itr;
        }

    }

    /**
     * フラグメント休止を行う
     */
    virtual void onPause() {
        auto itr = fragments.begin(), end = fragments.end();

        while (itr != end) {
            (*itr)->onPause();
            ++itr;
        }
    }

    /**
     * 解放処理を行う
     */
    virtual void onDestroy() {
        auto itr = fragments.begin(), end = fragments.end();

        while (itr != end) {
            (*itr)->onDestroy();
            ++itr;
        }
    }

    virtual BaseApplication *getApplication() const {
        return application;
    }

    /**
     * IDを指定してFragmentを取得する
     */
    virtual SelectionPtr<IApplicationFragment> findFragmentById(const uint64_t id) const {
        es_mutex_lock lock(transactionLock);

        auto itr = fragments.begin(), end = fragments.end();
        while (itr != end) {
            if ((*itr)->getId() == id) {
                return (*itr);
            }
            ++itr;
        }
        return SelectionPtr<IApplicationFragment>();
    }

    /**
     * タグを指定してFragmentを取得する
     */
    virtual SelectionPtr<IApplicationFragment> findFragmentByTag(const std::string &tag) {
        es_mutex_lock lock(transactionLock);

        auto itr = fragments.begin(), end = fragments.end();
        while (itr != end) {
            if ((*itr)->getTag() == tag) {
                return (*itr);
            }
            ++itr;
        }
        return SelectionPtr<IApplicationFragment>();
    }
};

}
