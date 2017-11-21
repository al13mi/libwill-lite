#include "LayerCombine.h"

LayerCombine::LayerCombine()
{
}


LayerCombine::~LayerCombine()
{
    //safe_delete(as_concat_);
    safe_delete(prevDA);
}

//合并层如果仅有一个上层，前向执行浅复制，即相当于直连
void LayerCombine::init2()
{
    combine_type_ = option_->getEnumFromSection(layer_name_, "combine_type", COMBINE_ADD);

    out_width_ = prev_layer_->getOutWidth();
    out_height_ = prev_layer_->getOutHeight();

    switch (combine_type_)
    {
    case COMBINE_CONCAT:
    {
        //待修改，输出较大时m辅助矩阵过大
        out_channel_ = 0;
        for (auto& l : prev_layers_)
        {
            out_channel_ += l->getOutChannel();
        }
        int current_index = 0;
        out_total_ = out_width_ * out_height_ * out_channel_;

        for (int i = 0; i < prev_layers_.size(); i++)
        {
            prevA.push_back(prev_layers_[i]->getA());
            prevDA.push_back(prev_layers_[i]->getDA()->clone());
            /*auto m = new Matrix(out_total_, prev_layers_[i]->getOutTotal(), MATRIX_DATA_INSIDE, CUDA_CPU);
            m->initData(0);
            for (int k = 0; k < prev_layers_[i]->getOutTotal(); k++)
            {
                m->getData(current_index + k, k) = 1;
            }
            current_index += prev_layers_[i]->getOutTotal();
            m->toGPU();
            as_concat_.push_back(m);*/
        }
    }
    break;
    case COMBINE_ADD:
        out_channel_ = prev_layer_->getOutChannel();
        out_total_ = out_width_ * out_height_ * out_channel_;
        break;
    default:
        break;
    }
    //out_total_ = out_width_ * out_height_ * out_channel_;
    if (prev_layers_.size() == 1)
    {
        X_ = prev_layer_->getA()->cloneShared();
    }
    else
    {
        X_ = new Matrix(out_width_, out_height_, out_channel_, batch_);
    }
    dX_ = new Matrix(X_);

    std::vector<real> factors;
    convert::findNumbers(option_->getStringFromSection(layer_name_, "factors", ""), &factors);
    factors_.resize(prev_layers_.size());
    for (int i = 0; i < factors_.size(); i++)
    {
        factors_[i] = 1;
        if (i < factors.size()) { factors_[i] = factors[i]; }
    }
}

void LayerCombine::updateX()
{
    switch (combine_type_)
    {
    case COMBINE_CONCAT:
        if (prev_layers_.size() == 1)
        {
            Matrix::copyData(prev_layer_->getA(), X_);
        }
        else
        {
            Matrix::concatByChannel(prevA, X_);
            /*X_->initData(0);
            for (int i = 0; i < prev_layers_.size(); i++)
            {
                Matrix::mul(as_concat_[i], prev_layers_[i]->getA(), X_, 1, 1);
            }*/
        }
        break;
    case COMBINE_ADD:
        X_->initData(0);
        for (int i = 0; i < prev_layers_.size(); i++)
        {
            auto& l = prev_layers_[i];
            Matrix::add(X_, l->getA(), X_, 1, factors_[i]);
        }
        break;
    default:
        break;
    }
}

void LayerCombine::updatePrevLayerDA()
{
    switch (combine_type_)
    {
    case COMBINE_CONCAT:
        if (prev_layers_.size() == 1)
        {
            Matrix::copyData(dX_, prev_layer_->getDA());
        }
        else
        {
            Matrix::splitByChannel(dX_, prevDA);
            for (int i = 0; i < prev_layers_.size(); i++)
            {
                Matrix::add(prev_layers_[i]->getDA(), prevDA[i], prev_layers_[i]->getDA());
                //Matrix::mul(as_concat_[i], dX_, prev_layers_[i]->getDA(), 1, 1, MATRIX_TRANS);
            }
        }
        break;
    case COMBINE_ADD:
        for (int i = 0; i < prev_layers_.size(); i++)
        {
            auto& l = prev_layers_[i];
            Matrix::add(dX_, l->getDA(), l->getDA(), 1, 1 / factors_[i]);
        }
        break;
    default:
        break;
    }
}

