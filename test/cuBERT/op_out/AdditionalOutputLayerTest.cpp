#include "gtest/gtest.h"

#include "cuBERT/common.h"
#include "cuBERT/op_out/AdditionalOutputLayer.h"
using namespace cuBERT;

class AdditionalOutputLayerTest : public ::testing::Test {
protected:
    void SetUp() override {
        cuBERT::initialize();
        handle = cuBERT::blas_create();
    }

    void TearDown() override {
        cuBERT::blas_destroy(handle);
        cuBERT::finalize();
    }

    void* handle;
};


TEST_F(AdditionalOutputLayerTest, compute) {
    size_t hidden_size = 3;
    float output_weights[3] = {-1, 0, 1};

    AdditionalOutputLayer aol(handle, hidden_size, output_weights);

    float in[6] = {
            2, 8, 3,
            -4, 1, 2,
    };
    float out[2];

    float* in_gpu = (float*) cuBERT::malloc(sizeof(float) * 6);
    float* out_gpu = (float*) cuBERT::malloc(sizeof(float) * 2);

    cuBERT::memcpy(in_gpu, in, sizeof(float) * 6, 1);

    aol.compute(2, in_gpu, out_gpu);

    cuBERT::memcpy(out, out_gpu, sizeof(float) * 2, 2);
    cuBERT::free(in_gpu);
    cuBERT::free(out_gpu);

    EXPECT_FLOAT_EQ(out[0], 1);
    EXPECT_FLOAT_EQ(out[1], 6);
}

class AdditionalOutputLayerCPUTest : public ::testing::Test {
protected:
    void SetUp() override {
        cuBERT::initialize(true);
    }

    void TearDown() override {
        cuBERT::finalize();
    }
};

TEST_F(AdditionalOutputLayerCPUTest, compute_cpu) {
    size_t hidden_size = 3;
    float output_weights[3] = {-1, 0, 1};

    AdditionalOutputLayer aol(nullptr, hidden_size, output_weights);

    float in[6] = {
            2, 8, 3,
            -4, 1, 2,
    };
    float out[2];

    aol.compute(2, in, out);

    EXPECT_FLOAT_EQ(out[0], 1);
    EXPECT_FLOAT_EQ(out[1], 6);
}