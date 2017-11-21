#pragma once
#include "Layer.h"

//数据合并
//分为2类：数据连接，数据和
class LayerCombine : public Layer
{
public:
    LayerCombine();
    virtual ~LayerCombine();

private:
    CombineType combine_type_ = COMBINE_ADD;
    //std::vector<Matrix*> as_concat_;
    std::vector<Matrix*> prevA, prevDA;

    std::vector<real> factors_;

    void init2() override;
    //void addPrevLayers(Layer* layer) override { prev_layer_ = layer; prev_layers_.push_back(layer); }

    void updateX() override;
    void updatePrevLayerDA() override;
};

