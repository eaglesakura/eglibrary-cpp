#pragma once

#include    <stdint.h>

namespace es {

enum InterfaceId_e {
    /**
     * es::Texture
     */
            InterfaceId_OpenGL_Texture = 0x01000000,
};

/**
 * 継承関係が複雑になる場合に使用するRoot Object
 *
 * vtableが生成されるため、オブジェクトサイズが増えることに注意する。
 */
class Object {
public:
    enum QueryResult_e {
        QueryResult_Failed = 0,
        QueryResult_Success = 1,
    };

    Object() = default;

    virtual ~Object() = default;

    /**
     * dynamic_castのオーバーヘッドを避けたい場合にインターフェース変換を行わせることが出来る。
     *
     * ただし、片安全ではないため、利用箇所には注意をすること。
     */
    virtual QueryResult_e queryInterface(const int64_t interfaceId, void **resultInterfacePtr) const { return QueryResult_Failed; }

protected:
    template<typename T>
    static QueryResult_e query_cast(const T *self, void **resultInterfacePtr) {
        *((T **) resultInterfacePtr) = (T *) self;
        return QueryResult_Success;
    }

};

}
