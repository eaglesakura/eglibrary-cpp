#ifndef es_grahpics_gl_IRENDERINGSURFACE_HPP_
#define es_grahpics_gl_IRENDERINGSURFACE_HPP_

#include    "es/Graphics.hpp"
#include    <vector>
#include    "VirtualDisplay.hpp"

namespace es {

/**
 * VGA = 640x480
 */
static const Vector2i16 DISPLAYSIZE_VGA(640, 480);

/**
 * WVGA = 800x480
 */
static const Vector2i16 DISPLAYSIZE_WVGA(800, 480);

/**
 * iPhone5世代のRetinaディスプレイ
 */
static const Vector2i16 DISPLAYSIZE_RETINA_iP5(1136, 640);

/**
 * HD = 1280x720
 */
static const Vector2i16 DISPLAYSIZE_720p(1280, 720);

/**
 * FULL-HD = 1920x1020
 */
static const Vector2i16 DISPLAYSIZE_1080p(1920, 1020);

/**
 * レンダリング用サーフェイスを定義する
 */
class IRenderingSurface {
public:
    IRenderingSurface() {

    }

    virtual ~IRenderingSurface() {
    }

    /**
     * サーフェイス幅を取得する
     */
    virtual int getWidth() const = 0;

    /**
     * サーフェイス高さを取得する
     */
    virtual int getHeight() const = 0;
};

/**
 * managed
 */
typedef std_shared_ptr<IRenderingSurface> MRenderingSurface;

#if 0
/**
 * 現在レンダリング対象のサーフェイスをスタックする。
 * 一番下は必ずEGLSurfaceとなる。
 *
 * width/height等のサーフェイス情報は仮想ディスプレイが基本となる。
 * 仮想ディスプレイはpush/popで自動的に設定できるが、明示的に修正することも可能。
 */
class RenderingStack: public Object {
    std::vector<MRenderingSurface> surfaceStack;

    /**
     * 現在レンダリング対象となるサーフェイス
     */
    MRenderingSurface currentSurface;

    /**
     * 仮想ディスプレイスタック
     */
    std::vector<MVirtualDisplay> displayStack;

    /**
     * 現在のレンダリング対象の仮想ディスプレイ
     */
    MVirtualDisplay currentDisplay;
public:
    RenderingStack() {

    }

    virtual ~RenderingStack() {
    }

    /**
     * レンダリングサーフェイスを一つ追加する
     * 自動的に仮想ディスプレイがひとつ積まれ、ディスプレイがセットされる。
     *
     * @return 古いレンダリングサーフェイス
     */
    virtual MRenderingSurface pushSurface(const MRenderingSurface surface) {
        // 現在の値を保存する
        MRenderingSurface result = currentSurface;

        // push
        {
            surfaceStack.push_back(surface);
            currentSurface = surface;
        }
        {
            MVirtualDisplay display(new VirtualDisplay());
            display->setRealDisplaySize(surface->getWidth(), surface->getHeight());
            display->setVirtualDisplaySize(surface->getWidth(), surface->getHeight());
            display->updateViewport(VirtualDisplay::FitType_Auto);

            currentDisplay = display;
            displayStack.push_back(display);
        }

        return result;
    }

    /**
     * レンダリングサーフェイスを一つ戻す
     * 自動的に仮想ディスプレイがひとつ戻される
     *
     * @return 古いレンダリングサーフェイス
     */
    virtual MRenderingSurface popSurface() {
        // 現在の値を保存する
        MRenderingSurface result = currentSurface;

        // pop
        {
            surfaceStack.pop_back();
            assert(!surfaceStack.empty()); // emptyになってはならない（最下層にはEGLSurfaceが存在するはずである）
            currentSurface = surfaceStack[surfaceStack.size() - 1];
        }
        {
            displayStack.pop_back();
            assert(!displayStack.empty()); // emptyになってはならない（最下層にはEGLSurfaceが存在するはずである）
            currentDisplay = displayStack[displayStack.size() - 1];
        }

        return result;
    }

    /**
     * 現在の描画先を取得する
     */
    virtual MRenderingSurface getCurrentRenderingTarget() const {
        return currentSurface;
    }

    /**
     * 現在の仮想ディスプレイを取得する。
     * 初期値は必ずサーフェイス値と一致している。
     */
    virtual MVirtualDisplay getCurrentVirtualDisplay() const {
        return currentDisplay;
    }

    /**
     * サーフェイスサイズの階層数を取得する
     */
    virtual int getSurfaceDepth() const {
        return surfaceStack.size();
    }
};
#endif
}

#endif /* IRENDERINGSURFACE_HPP_ */
