#if 0
//
// Generated by Microsoft (R) HLSL Shader Compiler 9.30.9200.20789
//
//
///
// Resource Bindings:
//
// Name                                 Type  Format         Dim Slot Elements
// ------------------------------ ---------- ------- ----------- ---- --------
// filters                           sampler      NA          NA    0        1
// baseTexture                       texture  float4          2d    0        1
// secondTexture                     texture  float4          2d    1        1
//
//
//
// Input signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// TEXCOORD                 0   xy          0     NONE   float   xy  
// TEXCOORD                 1     z         0     NONE   float     z 
// COLOR                    0   xyzw        1     NONE   float   xyzw
// SV_POSITION              0   xyzw        2      POS   float       
//
//
// Output signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// SV_TARGET                0   xyzw        0   TARGET   float   xyzw
//
ps_5_0
dcl_globalFlags refactoringAllowed
dcl_sampler s0, mode_default
dcl_resource_texture2d (float,float,float,float) t0
dcl_resource_texture2d (float,float,float,float) t1
dcl_input_ps linear v0.xy
dcl_input_ps linear v0.z
dcl_input_ps linear v1.xyzw
dcl_output o0.xyzw
dcl_temps 3
sample_indexable(texture2d)(float,float,float,float) r0.xyzw, v0.xyxx, t0.xyzw, s0
lt r1.x, r0.w, l(0.000001)
discard_nz r1.x
sample_indexable(texture2d)(float,float,float,float) r1.xy, v0.xyxx, t1.xwyz, s0
lt r1.y, r1.y, l(0.000001)
discard_nz r1.y
min r1.y, v0.z, l(1.000000)
lt r1.z, r1.y, l(0.000000)
add r1.y, -r1.y, l(1.000000)
movc r1.y, r1.z, l(0.999990), r1.y
add r1.x, -r1.x, r1.y
lt r2.xyzw, l(0.015000, 0.013000, 0.011000, 0.009000), r1.xxxx
lt r1.x, l(0.007000), r1.x
movc r1.x, r1.x, l(0.700000), r0.w
movc r1.x, r2.w, l(0.600000), r1.x
movc r1.x, r2.z, l(0.400000), r1.x
movc r1.x, r2.y, l(0.200000), r1.x
movc r0.w, r2.x, l(0), r1.x
mul o0.xyzw, r0.xyzw, v1.xyzw
ret 
// Approximately 20 instruction slots used
#endif

const BYTE MultiTexture_2D_PS[] =
{
     68,  88,  66,  67, 146, 116, 
    117,  19, 210,   3, 183,  68, 
    161,  96, 178, 118,  34, 154, 
    175, 177,   1,   0,   0,   0, 
     96,   5,   0,   0,   5,   0, 
      0,   0,  52,   0,   0,   0, 
     48,   1,   0,   0, 188,   1, 
      0,   0, 240,   1,   0,   0, 
    196,   4,   0,   0,  82,  68, 
     69,  70, 244,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   3,   0,   0,   0, 
     60,   0,   0,   0,   0,   5, 
    255, 255,   0,   1,   0,   0, 
    190,   0,   0,   0,  82,  68, 
     49,  49,  60,   0,   0,   0, 
     24,   0,   0,   0,  32,   0, 
      0,   0,  40,   0,   0,   0, 
     36,   0,   0,   0,  12,   0, 
      0,   0,   0,   0,   0,   0, 
    156,   0,   0,   0,   3,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,   1,   0, 
      0,   0, 164,   0,   0,   0, 
      2,   0,   0,   0,   5,   0, 
      0,   0,   4,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0,   1,   0,   0,   0, 
     13,   0,   0,   0, 176,   0, 
      0,   0,   2,   0,   0,   0, 
      5,   0,   0,   0,   4,   0, 
      0,   0, 255, 255, 255, 255, 
      1,   0,   0,   0,   1,   0, 
      0,   0,  13,   0,   0,   0, 
    102, 105, 108, 116, 101, 114, 
    115,   0,  98,  97, 115, 101, 
     84, 101, 120, 116, 117, 114, 
    101,   0, 115, 101,  99, 111, 
    110, 100,  84, 101, 120, 116, 
    117, 114, 101,   0,  77, 105, 
     99, 114, 111, 115, 111, 102, 
    116,  32,  40,  82,  41,  32, 
     72,  76,  83,  76,  32,  83, 
    104,  97, 100, 101, 114,  32, 
     67, 111, 109, 112, 105, 108, 
    101, 114,  32,  57,  46,  51, 
     48,  46,  57,  50,  48,  48, 
     46,  50,  48,  55,  56,  57, 
      0, 171, 171, 171,  73,  83, 
     71,  78, 132,   0,   0,   0, 
      4,   0,   0,   0,   8,   0, 
      0,   0, 104,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   3,   0,   0,   0, 
      0,   0,   0,   0,   3,   3, 
      0,   0, 104,   0,   0,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,   3,   0,   0,   0, 
      0,   0,   0,   0,   4,   4, 
      0,   0, 113,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   3,   0,   0,   0, 
      1,   0,   0,   0,  15,  15, 
      0,   0, 119,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   3,   0,   0,   0, 
      2,   0,   0,   0,  15,   0, 
      0,   0,  84,  69,  88,  67, 
     79,  79,  82,  68,   0,  67, 
     79,  76,  79,  82,   0,  83, 
     86,  95,  80,  79,  83,  73, 
     84,  73,  79,  78,   0, 171, 
     79,  83,  71,  78,  44,   0, 
      0,   0,   1,   0,   0,   0, 
      8,   0,   0,   0,  32,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   0,   0,   0,   0, 
     15,   0,   0,   0,  83,  86, 
     95,  84,  65,  82,  71,  69, 
     84,   0, 171, 171,  83,  72, 
     69,  88, 204,   2,   0,   0, 
     80,   0,   0,   0, 179,   0, 
      0,   0, 106,   8,   0,   1, 
     90,   0,   0,   3,   0,  96, 
     16,   0,   0,   0,   0,   0, 
     88,  24,   0,   4,   0, 112, 
     16,   0,   0,   0,   0,   0, 
     85,  85,   0,   0,  88,  24, 
      0,   4,   0, 112,  16,   0, 
      1,   0,   0,   0,  85,  85, 
      0,   0,  98,  16,   0,   3, 
     50,  16,  16,   0,   0,   0, 
      0,   0,  98,  16,   0,   3, 
     66,  16,  16,   0,   0,   0, 
      0,   0,  98,  16,   0,   3, 
    242,  16,  16,   0,   1,   0, 
      0,   0, 101,   0,   0,   3, 
    242,  32,  16,   0,   0,   0, 
      0,   0, 104,   0,   0,   2, 
      3,   0,   0,   0,  69,   0, 
      0, 139, 194,   0,   0, 128, 
     67,  85,  21,   0, 242,   0, 
     16,   0,   0,   0,   0,   0, 
     70,  16,  16,   0,   0,   0, 
      0,   0,  70, 126,  16,   0, 
      0,   0,   0,   0,   0,  96, 
     16,   0,   0,   0,   0,   0, 
     49,   0,   0,   7,  18,   0, 
     16,   0,   1,   0,   0,   0, 
     58,   0,  16,   0,   0,   0, 
      0,   0,   1,  64,   0,   0, 
    189,  55, 134,  53,  13,   0, 
      4,   3,  10,   0,  16,   0, 
      1,   0,   0,   0,  69,   0, 
      0, 139, 194,   0,   0, 128, 
     67,  85,  21,   0,  50,   0, 
     16,   0,   1,   0,   0,   0, 
     70,  16,  16,   0,   0,   0, 
      0,   0, 198, 121,  16,   0, 
      1,   0,   0,   0,   0,  96, 
     16,   0,   0,   0,   0,   0, 
     49,   0,   0,   7,  34,   0, 
     16,   0,   1,   0,   0,   0, 
     26,   0,  16,   0,   1,   0, 
      0,   0,   1,  64,   0,   0, 
    189,  55, 134,  53,  13,   0, 
      4,   3,  26,   0,  16,   0, 
      1,   0,   0,   0,  51,   0, 
      0,   7,  34,   0,  16,   0, 
      1,   0,   0,   0,  42,  16, 
     16,   0,   0,   0,   0,   0, 
      1,  64,   0,   0,   0,   0, 
    128,  63,  49,   0,   0,   7, 
     66,   0,  16,   0,   1,   0, 
      0,   0,  26,   0,  16,   0, 
      1,   0,   0,   0,   1,  64, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   8,  34,   0, 
     16,   0,   1,   0,   0,   0, 
     26,   0,  16, 128,  65,   0, 
      0,   0,   1,   0,   0,   0, 
      1,  64,   0,   0,   0,   0, 
    128,  63,  55,   0,   0,   9, 
     34,   0,  16,   0,   1,   0, 
      0,   0,  42,   0,  16,   0, 
      1,   0,   0,   0,   1,  64, 
      0,   0,  88, 255, 127,  63, 
     26,   0,  16,   0,   1,   0, 
      0,   0,   0,   0,   0,   8, 
     18,   0,  16,   0,   1,   0, 
      0,   0,  10,   0,  16, 128, 
     65,   0,   0,   0,   1,   0, 
      0,   0,  26,   0,  16,   0, 
      1,   0,   0,   0,  49,   0, 
      0,  10, 242,   0,  16,   0, 
      2,   0,   0,   0,   2,  64, 
      0,   0, 143, 194, 117,  60, 
    244, 253,  84,  60,  88,  57, 
     52,  60, 188, 116,  19,  60, 
      6,   0,  16,   0,   1,   0, 
      0,   0,  49,   0,   0,   7, 
     18,   0,  16,   0,   1,   0, 
      0,   0,   1,  64,   0,   0, 
     66,  96, 229,  59,  10,   0, 
     16,   0,   1,   0,   0,   0, 
     55,   0,   0,   9,  18,   0, 
     16,   0,   1,   0,   0,   0, 
     10,   0,  16,   0,   1,   0, 
      0,   0,   1,  64,   0,   0, 
     51,  51,  51,  63,  58,   0, 
     16,   0,   0,   0,   0,   0, 
     55,   0,   0,   9,  18,   0, 
     16,   0,   1,   0,   0,   0, 
     58,   0,  16,   0,   2,   0, 
      0,   0,   1,  64,   0,   0, 
    154, 153,  25,  63,  10,   0, 
     16,   0,   1,   0,   0,   0, 
     55,   0,   0,   9,  18,   0, 
     16,   0,   1,   0,   0,   0, 
     42,   0,  16,   0,   2,   0, 
      0,   0,   1,  64,   0,   0, 
    205, 204, 204,  62,  10,   0, 
     16,   0,   1,   0,   0,   0, 
     55,   0,   0,   9,  18,   0, 
     16,   0,   1,   0,   0,   0, 
     26,   0,  16,   0,   2,   0, 
      0,   0,   1,  64,   0,   0, 
    205, 204,  76,  62,  10,   0, 
     16,   0,   1,   0,   0,   0, 
     55,   0,   0,   9, 130,   0, 
     16,   0,   0,   0,   0,   0, 
     10,   0,  16,   0,   2,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0,   0,   0,  10,   0, 
     16,   0,   1,   0,   0,   0, 
     56,   0,   0,   7, 242,  32, 
     16,   0,   0,   0,   0,   0, 
     70,  14,  16,   0,   0,   0, 
      0,   0,  70,  30,  16,   0, 
      1,   0,   0,   0,  62,   0, 
      0,   1,  83,  84,  65,  84, 
    148,   0,   0,   0,  20,   0, 
      0,   0,   3,   0,   0,   0, 
      0,   0,   0,   0,   4,   0, 
      0,   0,   9,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      2,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   2,   0,   0,   0, 
      6,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0
};