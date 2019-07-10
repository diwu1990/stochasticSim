static void convolution2d_same(const float* input, const float* kernel, const float* bias, float* output,
        const size_t in, const size_t ic, const size_t iw, const size_t ih,
        const size_t kn, const size_t kw, const size_t kh, const size_t kws, const size_t khs,
        const size_t ow, const size_t oh)
    {
        const DataSize inputSize(in, ic, iw, ih);
        const DataSize kernelSize(kn, ic, kw, kh);
        const DataSize outputSize(in, kn, ow, oh);
        for (size_t nn = 0; nn < in; nn++)  // batch number
        {
            for (size_t nc = 0; nc < outputSize.channels; nc++)  // output channel number
            {
                for (size_t nh = 0; nh < outputSize.height; nh++)  // output height
                {
                    for (size_t nw = 0; nw < outputSize.width; nw++)  // output width
                    {
                        const int inStartX = nw - kw/2;  // horizontal
                        const int inStartY = nh - kh / 2;  // vertical
                        float sum = 0;
                        for (size_t kc = 0; kc < kernelSize.channels; kc++)  // input channel number
                        {
                            for (size_t kh = 0; kh < kernelSize.height; kh++)  // kernel height
                            {
                                for (size_t kw = 0; kw < kernelSize.width; kw++)  // kernel width
                                {
                                    const int inY = inStartY + kh;
                                    const int inX = inStartX + kw;
                                    if (inY >= 0 && inY<(int)inputSize.height && inX >= 0 && inX<(int)inputSize.width)
                                    {
                                        const size_t prevIdx = inputSize.getIndex(nn, kc, inY, inX);
                                        const size_t kernelIdx = kernelSize.getIndex(nc, kc, kh, kw);
                                        sum += input[prevIdx] * kernel[kernelIdx];
                                    }                                   
                                }
                            }
                        }
                        if (bias)
                        {
                            const size_t biasIdx = nc;
                            sum += bias[biasIdx];
                        }
                        const size_t nextIdx = outputSize.getIndex(nn, nc, nh, nw);
                        output[nextIdx] = sum;
                    }
                }
            }
        }
    }