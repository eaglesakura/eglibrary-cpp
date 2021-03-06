//
// Created by @eaglesakura on 15/05/08.
//

#ifndef ASSEMBLE_MMD_HPP
#define ASSEMBLE_MMD_HPP



/**
 * フィギュアの頂点はソフトウェア or GPGPUによってスキニングされ、動的に生成される。
 *
 * スキニングをハードウェアからソフトウェアに移行したのは、大量にレンダリングするよりもモーフ等の処理のほうが重要度が高いため。
 * また、ボーン数の制限によるフィギュアデータの複雑性を下げる狙いがある。
 *
 * スキニングの対象ではない頂点データはstaticに扱われるため、結果的に動的(及び計算用Meta)/静的の複数頂点バッファを扱うことになる。
 *
 * レンダリング時に必要であれば2つの頂点をブレンドした配列を生成するほうが良い。
 *
 * PMXフォーマット
 *
 * http://www18.ocn.ne.jp/~atrenas/MMD/step_0002.html
 */
#include "PmxHeader.hpp"
#include "PmxMesh.h"
#include "PmxFigure.h"


#endif //ASSEMBLE_MMD_HPP
