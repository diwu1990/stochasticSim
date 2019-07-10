static void convolution2d_validate(const float* input, const float* kernel, const float* bias, float* output,
        const size_t in, const size_t ic, const size_t iw, const size_t ih,
        const size_t kn, const size_t kw, const size_t kh, const size_t kws, const size_t khs,
        const size_t ow, const size_t oh)
    {
        const DataSize inputSize(in, ic, iw, ih);
        const DataSize kernelSize(kn, ic, kw, kh);
        const DataSize outputSize(in, kn, ow, oh);
        for (size_t nn = 0; nn < in; nn++)
        {
            for (size_t nc = 0; nc < outputSize.channels; nc++)
            {
                for (size_t nh = 0; nh < outputSize.height; nh++)
                {
                    for (size_t nw = 0; nw < outputSize.width; nw++)
                    {
                        const size_t inStartX = nw*kws;
                        const size_t inStartY = nh*khs;
                        float sum = 0;
                        for (size_t kc = 0; kc < kernelSize.channels; kc++)
                        {
                            for (size_t kh = 0; kh < kernelSize.height; kh++)
                            {
                                for (size_t kw = 0; kw < kernelSize.width; kw++)
                                {
                                    const size_t prevIdx = inputSize.getIndex(nn, kc, inStartY + kh, inStartX + kw);
                                    const size_t kernelIdx = kernelSize.getIndex(nc, kc, kh, kw);
                                    sum += input[prevIdx] * kernel[kernelIdx];
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