#pragma once

namespace es {

/**
 * 継承関係が複雑になる場合に使用するRoot Object
 *
 * vtableが生成されるため、オブジェクトサイズが増えることに注意する。
 */
class Object {
public:
    Object() {
    }

    virtual ~Object() {
    }
};

}
