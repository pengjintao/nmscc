#pragma once

#ifndef _NMS_CORE_CPP_H_
#define _NMS_CORE_CPP_H_

//$50,$49,$48,$47,$46,$45,$44,$43,$42,$41,$40,$39,$38,$37,$36,$35,$34,$33,$32,$31,$30,$29,$28,$27,$26,$25,$24,$23,$22,$21,$20,$19,$18,$17,$16,$15,$14,$13,$12,$11,$10,$9,$8,$7,$6,$5,$4,$3,$2,$1,$0
// 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0
//  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1

#define NMSCPP_EX(...)              __VA_ARGS__

#define NMSCPP_DO(f, ...)           NMSCPP_DO_(f, (__VA_ARGS__))
#define NMSCPP_DO_(f, arg)          f##arg

/* cast x to cstring */
#define NMSCPP_STR(x)               NMSCPP_STR1((x))
#define NMSCPP_STR1(...)            NMSCPP_STR2 __VA_ARGS__
#define NMSCPP_STR2(...)            #__VA_ARGS__

#ifdef NMS_CC_INTELLISENSE
/*! get element at index `id` */
#define NMSCPP_AT(id, val, ...)     val
#else
#define NMSCPP_AT(id, ...)          NMSCPP_DO(NMSCPP_AT_##id, __VA_ARGS__)
#endif

#define NMSCPP_AT_0(_,...)          _
#define NMSCPP_AT_1(_,...)          NMSCPP_AT_0(__VA_ARGS__,_)
#define NMSCPP_AT_2(_,...)          NMSCPP_DO(NMSCPP_AT_1,  __VA_ARGS__)
#define NMSCPP_AT_3(_,...)          NMSCPP_DO(NMSCPP_AT_2,  __VA_ARGS__)
#define NMSCPP_AT_4(_,...)          NMSCPP_DO(NMSCPP_AT_3,  __VA_ARGS__)
#define NMSCPP_AT_5(_,...)          NMSCPP_DO(NMSCPP_AT_4,  __VA_ARGS__)
#define NMSCPP_AT_6(_,...)          NMSCPP_DO(NMSCPP_AT_5,  __VA_ARGS__)
#define NMSCPP_AT_7(_,...)          NMSCPP_DO(NMSCPP_AT_6,  __VA_ARGS__)
#define NMSCPP_AT_8(_,...)          NMSCPP_DO(NMSCPP_AT_7,  __VA_ARGS__)
#define NMSCPP_AT_9(_,...)          NMSCPP_DO(NMSCPP_AT_8,  __VA_ARGS__)
#define NMSCPP_AT_10(_,...)         NMSCPP_DO(NMSCPP_AT_9,  __VA_ARGS__)
#define NMSCPP_AT_11(_,...)         NMSCPP_DO(NMSCPP_AT_10, __VA_ARGS__)
#define NMSCPP_AT_12(_,...)         NMSCPP_DO(NMSCPP_AT_11, __VA_ARGS__)
#define NMSCPP_AT_13(_,...)         NMSCPP_DO(NMSCPP_AT_12, __VA_ARGS__)
#define NMSCPP_AT_14(_,...)         NMSCPP_DO(NMSCPP_AT_13, __VA_ARGS__)
#define NMSCPP_AT_15(_,...)         NMSCPP_DO(NMSCPP_AT_14, __VA_ARGS__)
#define NMSCPP_AT_16(_,...)         NMSCPP_DO(NMSCPP_AT_15, __VA_ARGS__)
#define NMSCPP_AT_17(_,...)         NMSCPP_DO(NMSCPP_AT_16, __VA_ARGS__)
#define NMSCPP_AT_18(_,...)         NMSCPP_DO(NMSCPP_AT_17, __VA_ARGS__)
#define NMSCPP_AT_19(_,...)         NMSCPP_DO(NMSCPP_AT_18, __VA_ARGS__)
#define NMSCPP_AT_20(_,...)         NMSCPP_DO(NMSCPP_AT_19, __VA_ARGS__)
#define NMSCPP_AT_21(_,...)         NMSCPP_DO(NMSCPP_AT_20, __VA_ARGS__)
#define NMSCPP_AT_22(_,...)         NMSCPP_DO(NMSCPP_AT_21, __VA_ARGS__)
#define NMSCPP_AT_23(_,...)         NMSCPP_DO(NMSCPP_AT_22, __VA_ARGS__)
#define NMSCPP_AT_24(_,...)         NMSCPP_DO(NMSCPP_AT_23, __VA_ARGS__)
#define NMSCPP_AT_25(_,...)         NMSCPP_DO(NMSCPP_AT_24, __VA_ARGS__)
#define NMSCPP_AT_26(_,...)         NMSCPP_DO(NMSCPP_AT_25, __VA_ARGS__)
#define NMSCPP_AT_27(_,...)         NMSCPP_DO(NMSCPP_AT_26, __VA_ARGS__)
#define NMSCPP_AT_28(_,...)         NMSCPP_DO(NMSCPP_AT_27, __VA_ARGS__)
#define NMSCPP_AT_29(_,...)         NMSCPP_DO(NMSCPP_AT_28, __VA_ARGS__)
#define NMSCPP_AT_30(_,...)         NMSCPP_DO(NMSCPP_AT_29, __VA_ARGS__)
#define NMSCPP_AT_31(_,...)         NMSCPP_DO(NMSCPP_AT_30, __VA_ARGS__)
#define NMSCPP_AT_32(_,...)         NMSCPP_DO(NMSCPP_AT_31, __VA_ARGS__)
#define NMSCPP_AT_33(_,...)         NMSCPP_DO(NMSCPP_AT_32, __VA_ARGS__)
#define NMSCPP_AT_34(_,...)         NMSCPP_DO(NMSCPP_AT_33, __VA_ARGS__)
#define NMSCPP_AT_35(_,...)         NMSCPP_DO(NMSCPP_AT_34, __VA_ARGS__)
#define NMSCPP_AT_36(_,...)         NMSCPP_DO(NMSCPP_AT_35, __VA_ARGS__)
#define NMSCPP_AT_37(_,...)         NMSCPP_DO(NMSCPP_AT_36, __VA_ARGS__)
#define NMSCPP_AT_38(_,...)         NMSCPP_DO(NMSCPP_AT_37, __VA_ARGS__)
#define NMSCPP_AT_39(_,...)         NMSCPP_DO(NMSCPP_AT_38, __VA_ARGS__)
#define NMSCPP_AT_40(_,...)         NMSCPP_DO(NMSCPP_AT_39, __VA_ARGS__)
#define NMSCPP_AT_41(_,...)         NMSCPP_DO(NMSCPP_AT_40, __VA_ARGS__)
#define NMSCPP_AT_42(_,...)         NMSCPP_DO(NMSCPP_AT_41, __VA_ARGS__)
#define NMSCPP_AT_43(_,...)         NMSCPP_DO(NMSCPP_AT_42, __VA_ARGS__)
#define NMSCPP_AT_44(_,...)         NMSCPP_DO(NMSCPP_AT_43, __VA_ARGS__)
#define NMSCPP_AT_45(_,...)         NMSCPP_DO(NMSCPP_AT_44, __VA_ARGS__)
#define NMSCPP_AT_46(_,...)         NMSCPP_DO(NMSCPP_AT_45, __VA_ARGS__)
#define NMSCPP_AT_47(_,...)         NMSCPP_DO(NMSCPP_AT_46, __VA_ARGS__)
#define NMSCPP_AT_48(_,...)         NMSCPP_DO(NMSCPP_AT_47, __VA_ARGS__)
#define NMSCPP_AT_49(_,...)         NMSCPP_DO(NMSCPP_AT_48, __VA_ARGS__)
#define NMSCPP_AT_50(_,...)         NMSCPP_DO(NMSCPP_AT_49, __VA_ARGS__)


#ifdef NMS_CC_INTELLISENSE
/* return arguments count */
#define NMSCPP_COUNT(...)           0
#else
#define NMSCPP_COUNT(...)           NMSCPP_DO(NMSCPP_AT_50, ~, ##__VA_ARGS__, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40,39,38,37,36,35,34,33,32,31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0)
#endif

#ifdef NMS_CC_INTELLISENSE
/* foreach(t: args) => f(t) */
#define NMSCPP_FOR(f, args, ...)       NMSCPP_DO(f, args)
#else
#define NMSCPP_FOR(f,...)           NMSCPP_FOR_X((f, __VA_ARGS__), (__VA_ARGS__, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40,39,38,37,36,35,34,33,32,31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0))
#endif

#define NMSCPP_FOR_X(farg, fidx)    NMSCPP_FOR_Y fidx farg
#define NMSCPP_FOR_Y($50,$49,$48,$47,$46,$45,$44,$43,$42,$41,$40,$39,$38,$37,$36,$35,$34,$33,$32,$31,$30,$29,$28,$27,$26,$25,$24,$23,$22,$21,$20,$19,$18,$17,$16,$15,$14,$13,$12,$11,$10,$9,$8,$7,$6,$5,$4,$3,$2,$1,$0, ...)  NMSCPP_FOR_##$0

#define NMSCPP_FOR_0( f)
#define NMSCPP_FOR_1( f, t)         NMSCPP_DO(f, t)
#define NMSCPP_FOR_2( f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_1(f, __VA_ARGS__))
#define NMSCPP_FOR_3( f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_2(f, __VA_ARGS__))
#define NMSCPP_FOR_4( f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_3(f, __VA_ARGS__))
#define NMSCPP_FOR_5( f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_4(f, __VA_ARGS__))
#define NMSCPP_FOR_6( f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_5(f, __VA_ARGS__))
#define NMSCPP_FOR_7( f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_6(f, __VA_ARGS__))
#define NMSCPP_FOR_8( f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_7(f, __VA_ARGS__))
#define NMSCPP_FOR_9( f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_8(f, __VA_ARGS__))
#define NMSCPP_FOR_10(f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_9(f, __VA_ARGS__))
#define NMSCPP_FOR_11(f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_10(f, __VA_ARGS__))
#define NMSCPP_FOR_12(f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_11(f, __VA_ARGS__))
#define NMSCPP_FOR_13(f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_12(f, __VA_ARGS__))
#define NMSCPP_FOR_14(f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_13(f, __VA_ARGS__))
#define NMSCPP_FOR_15(f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_14(f, __VA_ARGS__))
#define NMSCPP_FOR_16(f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_15(f, __VA_ARGS__))
#define NMSCPP_FOR_17(f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_16(f, __VA_ARGS__))
#define NMSCPP_FOR_18(f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_17(f, __VA_ARGS__))
#define NMSCPP_FOR_19(f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_18(f, __VA_ARGS__))
#define NMSCPP_FOR_20(f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_19(f, __VA_ARGS__))
#define NMSCPP_FOR_21(f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_20(f, __VA_ARGS__))
#define NMSCPP_FOR_22(f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_21(f, __VA_ARGS__))
#define NMSCPP_FOR_23(f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_22(f, __VA_ARGS__))
#define NMSCPP_FOR_24(f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_23(f, __VA_ARGS__))
#define NMSCPP_FOR_25(f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_24(f, __VA_ARGS__))
#define NMSCPP_FOR_26(f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_25(f, __VA_ARGS__))
#define NMSCPP_FOR_27(f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_26(f, __VA_ARGS__))
#define NMSCPP_FOR_28(f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_27(f, __VA_ARGS__))
#define NMSCPP_FOR_29(f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_28(f, __VA_ARGS__))
#define NMSCPP_FOR_30(f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_29(f, __VA_ARGS__))
#define NMSCPP_FOR_31(f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_30(f, __VA_ARGS__))
#define NMSCPP_FOR_32(f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_31(f, __VA_ARGS__))
#define NMSCPP_FOR_33(f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_32(f, __VA_ARGS__))
#define NMSCPP_FOR_34(f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_33(f, __VA_ARGS__))
#define NMSCPP_FOR_35(f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_34(f, __VA_ARGS__))
#define NMSCPP_FOR_36(f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_35(f, __VA_ARGS__))
#define NMSCPP_FOR_37(f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_36(f, __VA_ARGS__))
#define NMSCPP_FOR_38(f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_37(f, __VA_ARGS__))
#define NMSCPP_FOR_39(f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_38(f, __VA_ARGS__))
#define NMSCPP_FOR_40(f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_39(f, __VA_ARGS__))
#define NMSCPP_FOR_41(f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_40(f, __VA_ARGS__))
#define NMSCPP_FOR_42(f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_41(f, __VA_ARGS__))
#define NMSCPP_FOR_43(f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_42(f, __VA_ARGS__))
#define NMSCPP_FOR_44(f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_43(f, __VA_ARGS__))
#define NMSCPP_FOR_45(f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_44(f, __VA_ARGS__))
#define NMSCPP_FOR_46(f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_45(f, __VA_ARGS__))
#define NMSCPP_FOR_47(f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_46(f, __VA_ARGS__))
#define NMSCPP_FOR_48(f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_47(f, __VA_ARGS__))
#define NMSCPP_FOR_49(f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_48(f, __VA_ARGS__))
#define NMSCPP_FOR_50(f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_49(f, __VA_ARGS__))
#define NMSCPP_FOR_51(f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_50(f, __VA_ARGS__))
#define NMSCPP_FOR_52(f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_51(f, __VA_ARGS__))
#define NMSCPP_FOR_53(f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_52(f, __VA_ARGS__))
#define NMSCPP_FOR_54(f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_53(f, __VA_ARGS__))
#define NMSCPP_FOR_55(f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_54(f, __VA_ARGS__))
#define NMSCPP_FOR_56(f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_55(f, __VA_ARGS__))
#define NMSCPP_FOR_57(f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_56(f, __VA_ARGS__))
#define NMSCPP_FOR_58(f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_57(f, __VA_ARGS__))
#define NMSCPP_FOR_59(f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_58(f, __VA_ARGS__))
#define NMSCPP_FOR_60(f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_59(f, __VA_ARGS__))
#define NMSCPP_FOR_61(f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_60(f, __VA_ARGS__))
#define NMSCPP_FOR_62(f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_61(f, __VA_ARGS__))
#define NMSCPP_FOR_63(f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_62(f, __VA_ARGS__))
#define NMSCPP_FOR_64(f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_63(f, __VA_ARGS__))
#define NMSCPP_FOR_65(f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_64(f, __VA_ARGS__))
#define NMSCPP_FOR_66(f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_65(f, __VA_ARGS__))
#define NMSCPP_FOR_67(f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_66(f, __VA_ARGS__))
#define NMSCPP_FOR_68(f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_67(f, __VA_ARGS__))
#define NMSCPP_FOR_69(f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_68(f, __VA_ARGS__))
#define NMSCPP_FOR_70(f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_69(f, __VA_ARGS__))
#define NMSCPP_FOR_71(f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_70(f, __VA_ARGS__))
#define NMSCPP_FOR_72(f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_71(f, __VA_ARGS__))
#define NMSCPP_FOR_73(f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_72(f, __VA_ARGS__))
#define NMSCPP_FOR_74(f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_73(f, __VA_ARGS__))
#define NMSCPP_FOR_75(f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_74(f, __VA_ARGS__))
#define NMSCPP_FOR_76(f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_75(f, __VA_ARGS__))
#define NMSCPP_FOR_77(f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_76(f, __VA_ARGS__))
#define NMSCPP_FOR_78(f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_77(f, __VA_ARGS__))
#define NMSCPP_FOR_79(f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_78(f, __VA_ARGS__))
#define NMSCPP_FOR_80(f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_79(f, __VA_ARGS__))
#define NMSCPP_FOR_81(f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_80(f, __VA_ARGS__))
#define NMSCPP_FOR_82(f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_81(f, __VA_ARGS__))
#define NMSCPP_FOR_83(f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_82(f, __VA_ARGS__))
#define NMSCPP_FOR_84(f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_83(f, __VA_ARGS__))
#define NMSCPP_FOR_85(f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_84(f, __VA_ARGS__))
#define NMSCPP_FOR_86(f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_85(f, __VA_ARGS__))
#define NMSCPP_FOR_87(f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_86(f, __VA_ARGS__))
#define NMSCPP_FOR_88(f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_87(f, __VA_ARGS__))
#define NMSCPP_FOR_89(f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_88(f, __VA_ARGS__))
#define NMSCPP_FOR_90(f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_89(f, __VA_ARGS__))
#define NMSCPP_FOR_91(f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_90(f, __VA_ARGS__))
#define NMSCPP_FOR_92(f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_91(f, __VA_ARGS__))
#define NMSCPP_FOR_93(f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_92(f, __VA_ARGS__))
#define NMSCPP_FOR_94(f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_93(f, __VA_ARGS__))
#define NMSCPP_FOR_95(f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_94(f, __VA_ARGS__))
#define NMSCPP_FOR_96(f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_95(f, __VA_ARGS__))
#define NMSCPP_FOR_97(f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_96(f, __VA_ARGS__))
#define NMSCPP_FOR_98(f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_97(f, __VA_ARGS__))
#define NMSCPP_FOR_99(f, t, ...)    NMSCPP_DO(f, t) NMSCPP_EX(NMSCPP_FOR_98(f, __VA_ARGS__))

#ifdef NMS_CC_INTELLISENSE
/*! loop(n, f, args...) => f(0, args...) ... f(n-1, args...) */
#define NMSCPP_LOOP(n, f,...)       NMSCPP_DO(f, 0, __VA_ARGS__)
#else
#define NMSCPP_LOOP(n, f,...)       NMSCPP_LOOP_##n(f, __VA_ARGS__)
#endif

#define NMSCPP_LOOP_1(f, ...)       NMSCPP_DO(f,  0, __VA_ARGS__)
#define NMSCPP_LOOP_2(f, ...)       NMSCPP_LOOP_1(f,  __VA_ARGS__) NMSCPP_DO(f,  1, __VA_ARGS__)
#define NMSCPP_LOOP_3(f, ...)       NMSCPP_LOOP_2(f,  __VA_ARGS__) NMSCPP_DO(f,  2, __VA_ARGS__)
#define NMSCPP_LOOP_4(f, ...)       NMSCPP_LOOP_3(f,  __VA_ARGS__) NMSCPP_DO(f,  3, __VA_ARGS__)
#define NMSCPP_LOOP_5(f, ...)       NMSCPP_LOOP_4(f,  __VA_ARGS__) NMSCPP_DO(f,  4, __VA_ARGS__)
#define NMSCPP_LOOP_6(f, ...)       NMSCPP_LOOP_5(f,  __VA_ARGS__) NMSCPP_DO(f,  5, __VA_ARGS__)
#define NMSCPP_LOOP_7(f, ...)       NMSCPP_LOOP_6(f,  __VA_ARGS__) NMSCPP_DO(f,  6, __VA_ARGS__)
#define NMSCPP_LOOP_8(f, ...)       NMSCPP_LOOP_7(f,  __VA_ARGS__) NMSCPP_DO(f,  7, __VA_ARGS__)
#define NMSCPP_LOOP_9(f, ...)       NMSCPP_LOOP_8(f,  __VA_ARGS__) NMSCPP_DO(f,  8, __VA_ARGS__)
#define NMSCPP_LOOP_10(f, ...)      NMSCPP_LOOP_9(f,  __VA_ARGS__) NMSCPP_DO(f,  9, __VA_ARGS__)
#define NMSCPP_LOOP_11(f, ...)      NMSCPP_LOOP_10(f, __VA_ARGS__) NMSCPP_DO(f, 10, __VA_ARGS__)
#define NMSCPP_LOOP_12(f, ...)      NMSCPP_LOOP_11(f, __VA_ARGS__) NMSCPP_DO(f, 11, __VA_ARGS__)
#define NMSCPP_LOOP_13(f, ...)      NMSCPP_LOOP_12(f, __VA_ARGS__) NMSCPP_DO(f, 12, __VA_ARGS__)
#define NMSCPP_LOOP_14(f, ...)      NMSCPP_LOOP_13(f, __VA_ARGS__) NMSCPP_DO(f, 13, __VA_ARGS__)
#define NMSCPP_LOOP_15(f, ...)      NMSCPP_LOOP_14(f, __VA_ARGS__) NMSCPP_DO(f, 14, __VA_ARGS__)
#define NMSCPP_LOOP_16(f, ...)      NMSCPP_LOOP_15(f, __VA_ARGS__) NMSCPP_DO(f, 15, __VA_ARGS__)
#define NMSCPP_LOOP_17(f, ...)      NMSCPP_LOOP_16(f, __VA_ARGS__) NMSCPP_DO(f, 16, __VA_ARGS__)
#define NMSCPP_LOOP_18(f, ...)      NMSCPP_LOOP_17(f, __VA_ARGS__) NMSCPP_DO(f, 17, __VA_ARGS__)
#define NMSCPP_LOOP_19(f, ...)      NMSCPP_LOOP_18(f, __VA_ARGS__) NMSCPP_DO(f, 18, __VA_ARGS__)
#define NMSCPP_LOOP_20(f, ...)      NMSCPP_LOOP_19(f, __VA_ARGS__) NMSCPP_DO(f, 19, __VA_ARGS__)
#define NMSCPP_LOOP_21(f, ...)      NMSCPP_LOOP_20(f, __VA_ARGS__) NMSCPP_DO(f, 20, __VA_ARGS__)
#define NMSCPP_LOOP_22(f, ...)      NMSCPP_LOOP_21(f, __VA_ARGS__) NMSCPP_DO(f, 21, __VA_ARGS__)
#define NMSCPP_LOOP_23(f, ...)      NMSCPP_LOOP_22(f, __VA_ARGS__) NMSCPP_DO(f, 22, __VA_ARGS__)
#define NMSCPP_LOOP_24(f, ...)      NMSCPP_LOOP_23(f, __VA_ARGS__) NMSCPP_DO(f, 23, __VA_ARGS__)
#define NMSCPP_LOOP_25(f, ...)      NMSCPP_LOOP_24(f, __VA_ARGS__) NMSCPP_DO(f, 24, __VA_ARGS__)
#define NMSCPP_LOOP_26(f, ...)      NMSCPP_LOOP_25(f, __VA_ARGS__) NMSCPP_DO(f, 25, __VA_ARGS__)
#define NMSCPP_LOOP_27(f, ...)      NMSCPP_LOOP_26(f, __VA_ARGS__) NMSCPP_DO(f, 26, __VA_ARGS__)
#define NMSCPP_LOOP_28(f, ...)      NMSCPP_LOOP_27(f, __VA_ARGS__) NMSCPP_DO(f, 27, __VA_ARGS__)
#define NMSCPP_LOOP_29(f, ...)      NMSCPP_LOOP_28(f, __VA_ARGS__) NMSCPP_DO(f, 28, __VA_ARGS__)
#define NMSCPP_LOOP_30(f, ...)      NMSCPP_LOOP_29(f, __VA_ARGS__) NMSCPP_DO(f, 29, __VA_ARGS__)
#define NMSCPP_LOOP_31(f, ...)      NMSCPP_LOOP_30(f, __VA_ARGS__) NMSCPP_DO(f, 30, __VA_ARGS__)
#define NMSCPP_LOOP_32(f, ...)      NMSCPP_LOOP_31(f, __VA_ARGS__) NMSCPP_DO(f, 31, __VA_ARGS__)
#define NMSCPP_LOOP_33(f, ...)      NMSCPP_LOOP_32(f, __VA_ARGS__) NMSCPP_DO(f, 32, __VA_ARGS__)
#define NMSCPP_LOOP_34(f, ...)      NMSCPP_LOOP_33(f, __VA_ARGS__) NMSCPP_DO(f, 33, __VA_ARGS__)
#define NMSCPP_LOOP_35(f, ...)      NMSCPP_LOOP_34(f, __VA_ARGS__) NMSCPP_DO(f, 34, __VA_ARGS__)
#define NMSCPP_LOOP_36(f, ...)      NMSCPP_LOOP_35(f, __VA_ARGS__) NMSCPP_DO(f, 35, __VA_ARGS__)
#define NMSCPP_LOOP_37(f, ...)      NMSCPP_LOOP_36(f, __VA_ARGS__) NMSCPP_DO(f, 36, __VA_ARGS__)
#define NMSCPP_LOOP_38(f, ...)      NMSCPP_LOOP_37(f, __VA_ARGS__) NMSCPP_DO(f, 37, __VA_ARGS__)
#define NMSCPP_LOOP_39(f, ...)      NMSCPP_LOOP_38(f, __VA_ARGS__) NMSCPP_DO(f, 38, __VA_ARGS__)
#define NMSCPP_LOOP_40(f, ...)      NMSCPP_LOOP_39(f, __VA_ARGS__) NMSCPP_DO(f, 39, __VA_ARGS__)
#define NMSCPP_LOOP_41(f, ...)      NMSCPP_LOOP_40(f, __VA_ARGS__) NMSCPP_DO(f, 40, __VA_ARGS__)
#define NMSCPP_LOOP_42(f, ...)      NMSCPP_LOOP_41(f, __VA_ARGS__) NMSCPP_DO(f, 41, __VA_ARGS__)
#define NMSCPP_LOOP_43(f, ...)      NMSCPP_LOOP_42(f, __VA_ARGS__) NMSCPP_DO(f, 42, __VA_ARGS__)
#define NMSCPP_LOOP_44(f, ...)      NMSCPP_LOOP_43(f, __VA_ARGS__) NMSCPP_DO(f, 43, __VA_ARGS__)
#define NMSCPP_LOOP_45(f, ...)      NMSCPP_LOOP_44(f, __VA_ARGS__) NMSCPP_DO(f, 44, __VA_ARGS__)
#define NMSCPP_LOOP_46(f, ...)      NMSCPP_LOOP_45(f, __VA_ARGS__) NMSCPP_DO(f, 45, __VA_ARGS__)
#define NMSCPP_LOOP_47(f, ...)      NMSCPP_LOOP_46(f, __VA_ARGS__) NMSCPP_DO(f, 46, __VA_ARGS__)
#define NMSCPP_LOOP_48(f, ...)      NMSCPP_LOOP_47(f, __VA_ARGS__) NMSCPP_DO(f, 47, __VA_ARGS__)
#define NMSCPP_LOOP_49(f, ...)      NMSCPP_LOOP_48(f, __VA_ARGS__) NMSCPP_DO(f, 48, __VA_ARGS__)
#define NMSCPP_LOOP_50(f, ...)      NMSCPP_LOOP_49(f, __VA_ARGS__) NMSCPP_DO(f, 49, __VA_ARGS__)
#define NMSCPP_LOOP_51(f, ...)      NMSCPP_LOOP_50(f, __VA_ARGS__) NMSCPP_DO(f, 50, __VA_ARGS__)
#define NMSCPP_LOOP_52(f, ...)      NMSCPP_LOOP_51(f, __VA_ARGS__) NMSCPP_DO(f, 51, __VA_ARGS__)
#define NMSCPP_LOOP_53(f, ...)      NMSCPP_LOOP_52(f, __VA_ARGS__) NMSCPP_DO(f, 52, __VA_ARGS__)
#define NMSCPP_LOOP_54(f, ...)      NMSCPP_LOOP_53(f, __VA_ARGS__) NMSCPP_DO(f, 53, __VA_ARGS__)
#define NMSCPP_LOOP_55(f, ...)      NMSCPP_LOOP_54(f, __VA_ARGS__) NMSCPP_DO(f, 54, __VA_ARGS__)
#define NMSCPP_LOOP_56(f, ...)      NMSCPP_LOOP_55(f, __VA_ARGS__) NMSCPP_DO(f, 55, __VA_ARGS__)
#define NMSCPP_LOOP_57(f, ...)      NMSCPP_LOOP_56(f, __VA_ARGS__) NMSCPP_DO(f, 56, __VA_ARGS__)
#define NMSCPP_LOOP_58(f, ...)      NMSCPP_LOOP_57(f, __VA_ARGS__) NMSCPP_DO(f, 57, __VA_ARGS__)
#define NMSCPP_LOOP_59(f, ...)      NMSCPP_LOOP_58(f, __VA_ARGS__) NMSCPP_DO(f, 58, __VA_ARGS__)
#define NMSCPP_LOOP_60(f, ...)      NMSCPP_LOOP_59(f, __VA_ARGS__) NMSCPP_DO(f, 59, __VA_ARGS__)
#define NMSCPP_LOOP_61(f, ...)      NMSCPP_LOOP_60(f, __VA_ARGS__) NMSCPP_DO(f, 60, __VA_ARGS__)
#define NMSCPP_LOOP_62(f, ...)      NMSCPP_LOOP_61(f, __VA_ARGS__) NMSCPP_DO(f, 61, __VA_ARGS__)
#define NMSCPP_LOOP_63(f, ...)      NMSCPP_LOOP_62(f, __VA_ARGS__) NMSCPP_DO(f, 62, __VA_ARGS__)
#define NMSCPP_LOOP_64(f, ...)      NMSCPP_LOOP_63(f, __VA_ARGS__) NMSCPP_DO(f, 63, __VA_ARGS__)
#define NMSCPP_LOOP_65(f, ...)      NMSCPP_LOOP_64(f, __VA_ARGS__) NMSCPP_DO(f, 64, __VA_ARGS__)
#define NMSCPP_LOOP_66(f, ...)      NMSCPP_LOOP_65(f, __VA_ARGS__) NMSCPP_DO(f, 65, __VA_ARGS__)
#define NMSCPP_LOOP_67(f, ...)      NMSCPP_LOOP_66(f, __VA_ARGS__) NMSCPP_DO(f, 66, __VA_ARGS__)
#define NMSCPP_LOOP_68(f, ...)      NMSCPP_LOOP_67(f, __VA_ARGS__) NMSCPP_DO(f, 67, __VA_ARGS__)
#define NMSCPP_LOOP_69(f, ...)      NMSCPP_LOOP_68(f, __VA_ARGS__) NMSCPP_DO(f, 68, __VA_ARGS__)
#define NMSCPP_LOOP_70(f, ...)      NMSCPP_LOOP_69(f, __VA_ARGS__) NMSCPP_DO(f, 69, __VA_ARGS__)
#define NMSCPP_LOOP_71(f, ...)      NMSCPP_LOOP_70(f, __VA_ARGS__) NMSCPP_DO(f, 70, __VA_ARGS__)
#define NMSCPP_LOOP_72(f, ...)      NMSCPP_LOOP_71(f, __VA_ARGS__) NMSCPP_DO(f, 71, __VA_ARGS__)
#define NMSCPP_LOOP_73(f, ...)      NMSCPP_LOOP_72(f, __VA_ARGS__) NMSCPP_DO(f, 72, __VA_ARGS__)
#define NMSCPP_LOOP_74(f, ...)      NMSCPP_LOOP_73(f, __VA_ARGS__) NMSCPP_DO(f, 73, __VA_ARGS__)
#define NMSCPP_LOOP_75(f, ...)      NMSCPP_LOOP_74(f, __VA_ARGS__) NMSCPP_DO(f, 74, __VA_ARGS__)
#define NMSCPP_LOOP_76(f, ...)      NMSCPP_LOOP_75(f, __VA_ARGS__) NMSCPP_DO(f, 75, __VA_ARGS__)
#define NMSCPP_LOOP_77(f, ...)      NMSCPP_LOOP_76(f, __VA_ARGS__) NMSCPP_DO(f, 76, __VA_ARGS__)
#define NMSCPP_LOOP_78(f, ...)      NMSCPP_LOOP_77(f, __VA_ARGS__) NMSCPP_DO(f, 77, __VA_ARGS__)
#define NMSCPP_LOOP_79(f, ...)      NMSCPP_LOOP_78(f, __VA_ARGS__) NMSCPP_DO(f, 78, __VA_ARGS__)
#define NMSCPP_LOOP_80(f, ...)      NMSCPP_LOOP_79(f, __VA_ARGS__) NMSCPP_DO(f, 79, __VA_ARGS__)
#define NMSCPP_LOOP_81(f, ...)      NMSCPP_LOOP_80(f, __VA_ARGS__) NMSCPP_DO(f, 80, __VA_ARGS__)
#define NMSCPP_LOOP_82(f, ...)      NMSCPP_LOOP_81(f, __VA_ARGS__) NMSCPP_DO(f, 81, __VA_ARGS__)
#define NMSCPP_LOOP_83(f, ...)      NMSCPP_LOOP_82(f, __VA_ARGS__) NMSCPP_DO(f, 82, __VA_ARGS__)
#define NMSCPP_LOOP_84(f, ...)      NMSCPP_LOOP_83(f, __VA_ARGS__) NMSCPP_DO(f, 83, __VA_ARGS__)
#define NMSCPP_LOOP_85(f, ...)      NMSCPP_LOOP_84(f, __VA_ARGS__) NMSCPP_DO(f, 84, __VA_ARGS__)
#define NMSCPP_LOOP_86(f, ...)      NMSCPP_LOOP_85(f, __VA_ARGS__) NMSCPP_DO(f, 85, __VA_ARGS__)
#define NMSCPP_LOOP_87(f, ...)      NMSCPP_LOOP_86(f, __VA_ARGS__) NMSCPP_DO(f, 86, __VA_ARGS__)
#define NMSCPP_LOOP_88(f, ...)      NMSCPP_LOOP_87(f, __VA_ARGS__) NMSCPP_DO(f, 87, __VA_ARGS__)
#define NMSCPP_LOOP_89(f, ...)      NMSCPP_LOOP_88(f, __VA_ARGS__) NMSCPP_DO(f, 88, __VA_ARGS__)
#define NMSCPP_LOOP_90(f, ...)      NMSCPP_LOOP_89(f, __VA_ARGS__) NMSCPP_DO(f, 89, __VA_ARGS__)
#define NMSCPP_LOOP_91(f, ...)      NMSCPP_LOOP_90(f, __VA_ARGS__) NMSCPP_DO(f, 90, __VA_ARGS__)
#define NMSCPP_LOOP_92(f, ...)      NMSCPP_LOOP_91(f, __VA_ARGS__) NMSCPP_DO(f, 91, __VA_ARGS__)
#define NMSCPP_LOOP_93(f, ...)      NMSCPP_LOOP_92(f, __VA_ARGS__) NMSCPP_DO(f, 92, __VA_ARGS__)
#define NMSCPP_LOOP_94(f, ...)      NMSCPP_LOOP_93(f, __VA_ARGS__) NMSCPP_DO(f, 93, __VA_ARGS__)
#define NMSCPP_LOOP_95(f, ...)      NMSCPP_LOOP_94(f, __VA_ARGS__) NMSCPP_DO(f, 94, __VA_ARGS__)
#define NMSCPP_LOOP_96(f, ...)      NMSCPP_LOOP_95(f, __VA_ARGS__) NMSCPP_DO(f, 95, __VA_ARGS__)
#define NMSCPP_LOOP_97(f, ...)      NMSCPP_LOOP_96(f, __VA_ARGS__) NMSCPP_DO(f, 96, __VA_ARGS__)
#define NMSCPP_LOOP_98(f, ...)      NMSCPP_LOOP_97(f, __VA_ARGS__) NMSCPP_DO(f, 97, __VA_ARGS__)
#define NMSCPP_LOOP_99(f, ...)      NMSCPP_LOOP_98(f, __VA_ARGS__) NMSCPP_DO(f, 98, __VA_ARGS__)

#endif