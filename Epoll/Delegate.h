#ifndef _DELEGATE_H_
#define _DELEGATE_H_

#define PARAM_JOIN(a, b) a##b

#define MAKE_PARAM1_1(T)  T##1
#define MAKE_PARAM1_2(T)  MAKE_PARAM1_1(T), T##2
#define MAKE_PARAM1_3(T)  MAKE_PARAM1_2(T), T##3
#define MAKE_PARAM1_4(T)  MAKE_PARAM1_3(T), T##4
#define MAKE_PARAM1_5(T)  MAKE_PARAM1_4(T), T##5

#define MAKE_PARAM2_1(T1, T2)  T1##1 T2##1
#define MAKE_PARAM2_2(T1, T2)  MAKE_PARAM2_1(T1, T2), T1##2 T2##2
#define MAKE_PARAM2_3(T1, T2)  MAKE_PARAM2_2(T1, T2), T1##3 T2##3
#define MAKE_PARAM2_4(T1, T2)  MAKE_PARAM2_3(T1, T2), T1##4 T2##4
#define MAKE_PARAM2_5(T1, T2)  MAKE_PARAM2_4(T1, T2), T1##5 T2##5

#define MAKE_PARAM1_N(N, T)      PARAM_JOIN(MAKE_PARAM1_, N)(T)
#define MAKE_PARAM2_N(N, T1, T2) PARAM_JOIN(MAKE_PARAM2_, N)(T1, T2)

template<class T>
class Delegate;

#define DELEGATE_PARAM_NUM 1
#include "DelegateImpl.h"
#undef DELEGATE_PARAM_NUM

#define DELEGATE_PARAM_NUM 2
#include "DelegateImpl.h"
#undef DELEGATE_PARAM_NUM

#define DELEGATE_PARAM_NUM 3
#include "DelegateImpl.h"
#undef DELEGATE_PARAM_NUM

#define DELEGATE_PARAM_NUM 4
#include "DelegateImpl.h"
#undef DELEGATE_PARAM_NUM

#define DELEGATE_PARAM_NUM 5
#include "DelegateImpl.h"
#undef DELEGATE_PARAM_NUM


#endif