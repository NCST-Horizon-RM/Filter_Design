#include "main.h"
#include "iir.h"

const int NL = 9;
const double NUM[9] = {
  5.285919273356e-12,4.228735418685e-11, 1.48005739654e-10,2.960114793079e-10,
  3.700143491349e-10,2.960114793079e-10, 1.48005739654e-10,4.228735418685e-11,
  5.285919273356e-12
};
const int DL = 9;
const double DEN[9] = {
                   1,   -7.590687094437,    25.21812014273,   -47.89335424121,
      56.87002612068,   -43.23484983982,    20.55056514782,    -5.58382515593,
     0.6640049215241
};

typedef struct
{
    double xhist[9];  // 改为double保持精度
    double yhist[9];  // 改为double保持精度
    double y;
} Filter_t;

Filter_t filter = {
    .xhist = {0},
    .yhist = {0}
};

// /*  hz=DEN/NUM
//     使用直接型差分方程实现：
//  *  y[n] = sum_{k=0..NL-1} NUM[k]*x[n-k] - sum_{k=1..DL-1} DEN[k]*y[n-k]
//  */
double butterOrdF(float input)
{
    double input_d = (double)input;  // 转换为double
    
    // 更新历史缓冲区
    for (int i = NL-1; i > 0; i--)
    {
        filter.xhist[i] = filter.xhist[i-1];
    }
    filter.xhist[0] = input_d;
    
    for (int i = DL-1; i > 0; i--)
    {
        filter.yhist[i] = filter.yhist[i-1];
    }
    
    // 计算分子部分 (与输入历史卷积)
    double numerator = 0.0;
    for (int k = 0; k < NL; k++)
    {
        numerator += NUM[k] * filter.xhist[k];
    }
    
    // 计算分母部分 (与输出历史卷积)
    double denominator = 0.0;
    for (int k = 1; k < DL; k++)  // 从1开始，因为DEN[0]=1
    {
        denominator += DEN[k] * filter.yhist[k];
    }
    
    // 计算当前输出: y[n] = numerator - denominator
    filter.yhist[0] = numerator - denominator;
    filter.y = filter.yhist[0];
    
    return filter.y;
}