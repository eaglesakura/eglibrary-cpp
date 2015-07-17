#pragma once

#include "es/eglibrary.hpp"
#include <es/graphics/PixelFormat.hpp>
#include "es/asset/IAsset.hpp"
#include "es/memory/Buffer.hpp"

namespace es {

/**
 * 画像の基本情報
 */
struct ImageInfo {
    uint16_t width;
    uint16_t height;
    enum ImageFormat_e {
        ImageFormat_PNG,
    } imageFormat;

    /**
     * 元画像ピクセルフォーマット
     */
    PixelFormat_e srcPixelFormat;

    /**
     * 変換されたピクセルフォーマット
     */
    PixelFormat_e dstPixelFormat;
};

/**
 * 画像読み込み時にコールバックを受け取る
 */
class IImageBufferListener {
public:
    /**
     * 画像情報を読み込んだ
     *
     * infoの所有権はDecoderにあるため、ポインタを保存してはならない。
     */
    virtual void onImageInfoReceived(const ImageInfo *info) = 0;

    /**
     * 画像を指定行読み込んだ
     *
     * 引数lineは使いまわされる可能性があるため、内部的にテクスチャコピー等を行うこと。
     */
    virtual void onImageLineReceived(const ImageInfo *info, const unsafe_array<uint8_t> pixels, const uint height) = 0;
};

/**
 * PNGファイルのデコードを行う。
 *
 * libpngに依存するため、ビルドはオプションとなる。
 */
class PngFileDecoder {
public:
    PngFileDecoder();

    ~PngFileDecoder();

    /**
     * ファイルフォーマットチェック等、事前に読み込んで閉まっているバッファがあったらそれを渡す
     * 内部ではデータをコピーするため、バッファは解放しても構わない。
     */
    void setReadedBuffer(const unsafe_array<uint8_t> &buffer);

    /**
     * 一度の読み込みで読み込む行数（高さ）を指定する。
     *
     * 0を指定した場合、全データを一括で読み込む。
     * デフォルトは0になっている。
     */
    void setOnceReadHeight(const uint heightPixels);

    /**
     * ピクセルフォーマットの変換リクエストを行う。
     */
    void setConvertPixelFormat(const PixelFormat_e format);

    /**
     * アセットをPNGファイルとして読み込む
     *
     * 読み込みに成功したらtrue、それ以外はfalseを返却する。
     */
    bool load(std::shared_ptr<IAsset> asset, selection_ptr<IImageBufferListener> listener);

private:
    ByteBuffer readedBuffer;

    /**
     * 一度に読み込む高さピクセル数。
     * 小さくするほど省メモリになるが、オーバーヘッドが大きくなる。
     * 0の場合一括で全てを読み込む。
     */
    uint16_t onceReadLines = 0;

    /**
     * ピクセルフォーマット変換をリクエストする場合、指定のフォーマットを設定する。
     *
     * ただし、未サポートのピクセルフォーマットを指定しようとするとfalseを返却してロードを強制終了する。
     */
    PixelFormat_e pixelConvert = PixelFormat_RGBA8888;
};

}
