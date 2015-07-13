/*
 * Object.hpp
 *
 *  Created on: 2014/08/23
 *      Author: eaglesakura
 */

#ifndef es_system_OBJECT_HPP_
#define es_system_OBJECT_HPP_

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

#endif /* OBJECT_HPP_ */
