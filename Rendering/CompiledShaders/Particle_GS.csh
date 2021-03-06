#if 0
//
// Generated by Microsoft (R) HLSL Shader Compiler 9.30.9200.20789
//
//
///
// Buffer Definitions: 
//
// cbuffer SCENE
// {
//
//   row_major float4x4 Scene_worldMatrix;// Offset:    0 Size:    64
//   row_major float4x4 projectionMatrix;// Offset:   64 Size:    64
//
// }
//
//
// Resource Bindings:
//
// Name                                 Type  Format         Dim Slot Elements
// ------------------------------ ---------- ------- ----------- ---- --------
// SCENE                             cbuffer      NA          NA    1        1
//
//
//
// Input signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// SV_POSITION              0   xyzw        0      POS   float   xyzw
// PSIZE                    0   x           1     NONE   float   x   
// COLOR                    0   xyzw        2     NONE   float   xyzw
//
//
// Output signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// TEXCOORD                 0   xy          0     NONE   float   xy  
// SV_POSITION              0   xyzw        1      POS   float   xyzw
// COLOR                    0   xyzw        2     NONE   float   xyzw
//
gs_5_0
dcl_globalFlags refactoringAllowed
dcl_constantbuffer cb1[8], immediateIndexed
dcl_input_siv v[1][0].xyzw, position
dcl_input v[1][1].x
dcl_input v[1][2].xyzw
dcl_temps 4
dcl_inputprimitive point 
dcl_stream m0
dcl_outputtopology trianglestrip 
dcl_output o0.xy
dcl_output_siv o1.xyzw, position
dcl_output o2.xyzw
dcl_maxout 4
mov o0.xy, l(1.000000,0,0,0)
mul r0.xyzw, cb1[1].yyyy, cb1[5].xyzw
mad r0.xyzw, cb1[1].xxxx, cb1[4].xyzw, r0.xyzw
mad r0.xyzw, cb1[1].zzzz, cb1[6].xyzw, r0.xyzw
mad r0.xyzw, cb1[1].wwww, cb1[7].xyzw, r0.xyzw
mul r0.xyzw, r0.xyzw, v[0][0].yyyy
mul r1.xyzw, cb1[0].yyyy, cb1[5].xyzw
mad r1.xyzw, cb1[0].xxxx, cb1[4].xyzw, r1.xyzw
mad r1.xyzw, cb1[0].zzzz, cb1[6].xyzw, r1.xyzw
mad r1.xyzw, cb1[0].wwww, cb1[7].xyzw, r1.xyzw
mad r0.xyzw, v[0][0].xxxx, r1.xyzw, r0.xyzw
mul r1.xyzw, cb1[2].yyyy, cb1[5].xyzw
mad r1.xyzw, cb1[2].xxxx, cb1[4].xyzw, r1.xyzw
mad r1.xyzw, cb1[2].zzzz, cb1[6].xyzw, r1.xyzw
mad r1.xyzw, cb1[2].wwww, cb1[7].xyzw, r1.xyzw
mad r0.xyzw, v[0][0].zzzz, r1.xyzw, r0.xyzw
mul r1.xyzw, cb1[3].yyyy, cb1[5].xyzw
mad r1.xyzw, cb1[3].xxxx, cb1[4].xyzw, r1.xyzw
mad r1.xyzw, cb1[3].zzzz, cb1[6].xyzw, r1.xyzw
mad r1.xyzw, cb1[3].wwww, cb1[7].xyzw, r1.xyzw
add r0.xyzw, r0.xyzw, r1.xyzw
mul r1.x, l(0.500000), v[0][1].x
sincos r2.x, r3.x, v[0][0].w
mul r1.y, r1.x, r2.x
mad r1.z, -r1.x, r3.x, -r1.y
add r1.z, r0.x, r1.z
mov o1.x, r1.z
mul r1.z, -r1.x, r2.x
mad r1.w, r1.x, r3.x, r1.z
add r1.w, r0.y, r1.w
mov o1.y, r1.w
mov o1.zw, r0.zzzw
mov o2.xyzw, v[0][2].xyzw
emit_stream m0
mov o0.xy, l(1.000000,1.000000,0,0)
mad r1.w, r1.x, r3.x, -r1.y
add r1.w, r0.x, r1.w
mov o1.x, r1.w
mad r1.w, r1.x, r3.x, r1.y
mad r1.y, -r1.x, r3.x, r1.y
add r1.yw, r0.yyyy, r1.yyyw
mov o1.y, r1.w
mov o1.zw, r0.zzzw
mov o2.xyzw, v[0][2].xyzw
emit_stream m0
mov o0.xy, l(0,0,0,0)
mad r1.w, -r1.x, r3.x, -r1.z
add r1.w, r0.x, r1.w
mov o1.x, r1.w
mad r1.w, -r1.x, r3.x, r1.z
mad r1.x, r1.x, r3.x, -r1.z
add r0.xy, r0.xyxx, r1.xwxx
mov o1.y, r0.y
mov o1.zw, r0.zzzw
mov o2.xyzw, v[0][2].xyzw
emit_stream m0
mov o0.xy, l(0,1.000000,0,0)
mov o1.x, r0.x
mov o1.y, r1.y
mov o1.zw, r0.zzzw
mov o2.xyzw, v[0][2].xyzw
emit_stream m0
cut_stream m0
ret 
// Approximately 64 instruction slots used
#endif

const BYTE Particle_GS[] =
{
     68,  88,  66,  67,  34, 247, 
    130, 133, 170,  70,  28,  97, 
     45, 251,  70, 173, 237,  66, 
    209,  22,   1,   0,   0,   0, 
     92,  11,   0,   0,   5,   0, 
      0,   0,  52,   0,   0,   0, 
    140,   1,   0,   0, 252,   1, 
      0,   0, 124,   2,   0,   0, 
    192,  10,   0,   0,  82,  68, 
     69,  70,  80,   1,   0,   0, 
      1,   0,   0,   0, 100,   0, 
      0,   0,   1,   0,   0,   0, 
     60,   0,   0,   0,   0,   5, 
     83,  71,   0,   1,   0,   0, 
     29,   1,   0,   0,  82,  68, 
     49,  49,  60,   0,   0,   0, 
     24,   0,   0,   0,  32,   0, 
      0,   0,  40,   0,   0,   0, 
     36,   0,   0,   0,  12,   0, 
      0,   0,   0,   0,   0,   0, 
     92,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
      1,   0,   0,   0,   1,   0, 
      0,   0,  83,  67,  69,  78, 
     69,   0, 171, 171,  92,   0, 
      0,   0,   2,   0,   0,   0, 
    124,   0,   0,   0, 128,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0, 204,   0, 
      0,   0,   0,   0,   0,   0, 
     64,   0,   0,   0,   2,   0, 
      0,   0, 232,   0,   0,   0, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0,  12,   1,   0,   0, 
     64,   0,   0,   0,  64,   0, 
      0,   0,   2,   0,   0,   0, 
    232,   0,   0,   0,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
     83,  99, 101, 110, 101,  95, 
    119, 111, 114, 108, 100,  77, 
     97, 116, 114, 105, 120,   0, 
    102, 108, 111,  97, 116,  52, 
    120,  52,   0, 171,   2,   0, 
      3,   0,   4,   0,   4,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
    222,   0,   0,   0, 112, 114, 
    111, 106, 101,  99, 116, 105, 
    111, 110,  77,  97, 116, 114, 
    105, 120,   0,  77, 105,  99, 
    114, 111, 115, 111, 102, 116, 
     32,  40,  82,  41,  32,  72, 
     76,  83,  76,  32,  83, 104, 
     97, 100, 101, 114,  32,  67, 
    111, 109, 112, 105, 108, 101, 
    114,  32,  57,  46,  51,  48, 
     46,  57,  50,  48,  48,  46, 
     50,  48,  55,  56,  57,   0, 
     73,  83,  71,  78, 104,   0, 
      0,   0,   3,   0,   0,   0, 
      8,   0,   0,   0,  80,   0, 
      0,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,   3,   0, 
      0,   0,   0,   0,   0,   0, 
     15,  15,   0,   0,  92,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   1,   0,   0,   0, 
      1,   1,   0,   0,  98,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   2,   0,   0,   0, 
     15,  15,   0,   0,  83,  86, 
     95,  80,  79,  83,  73,  84, 
     73,  79,  78,   0,  80,  83, 
     73,  90,  69,   0,  67,  79, 
     76,  79,  82,   0,  79,  83, 
     71,  53, 120,   0,   0,   0, 
      3,   0,   0,   0,   8,   0, 
      0,   0,   0,   0,   0,   0, 
     92,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   0,   0, 
      0,   0,   3,  12,   0,   0, 
      0,   0,   0,   0, 101,   0, 
      0,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,   3,   0, 
      0,   0,   1,   0,   0,   0, 
     15,   0,   0,   0,   0,   0, 
      0,   0, 113,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   3,   0,   0,   0, 
      2,   0,   0,   0,  15,   0, 
      0,   0,  84,  69,  88,  67, 
     79,  79,  82,  68,   0,  83, 
     86,  95,  80,  79,  83,  73, 
     84,  73,  79,  78,   0,  67, 
     79,  76,  79,  82,   0, 171, 
     83,  72,  69,  88,  60,   8, 
      0,   0,  80,   0,   2,   0, 
     15,   2,   0,   0, 106,   8, 
      0,   1,  89,   0,   0,   4, 
     70, 142,  32,   0,   1,   0, 
      0,   0,   8,   0,   0,   0, 
     97,   0,   0,   5, 242,  16, 
     32,   0,   1,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,  95,   0,   0,   4, 
     18,  16,  32,   0,   1,   0, 
      0,   0,   1,   0,   0,   0, 
     95,   0,   0,   4, 242,  16, 
     32,   0,   1,   0,   0,   0, 
      2,   0,   0,   0, 104,   0, 
      0,   2,   4,   0,   0,   0, 
     93,   8,   0,   1, 143,   0, 
      0,   3,   0,   0,  17,   0, 
      0,   0,   0,   0,  92,  40, 
      0,   1, 101,   0,   0,   3, 
     50,  32,  16,   0,   0,   0, 
      0,   0, 103,   0,   0,   4, 
    242,  32,  16,   0,   1,   0, 
      0,   0,   1,   0,   0,   0, 
    101,   0,   0,   3, 242,  32, 
     16,   0,   2,   0,   0,   0, 
     94,   0,   0,   2,   4,   0, 
      0,   0,  54,   0,   0,   8, 
     50,  32,  16,   0,   0,   0, 
      0,   0,   2,  64,   0,   0, 
      0,   0, 128,  63,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,  56,   0, 
      0,   9, 242,   0,  16,   0, 
      0,   0,   0,   0,  86, 133, 
     32,   0,   1,   0,   0,   0, 
      1,   0,   0,   0,  70, 142, 
     32,   0,   1,   0,   0,   0, 
      5,   0,   0,   0,  50,   0, 
      0,  11, 242,   0,  16,   0, 
      0,   0,   0,   0,   6, 128, 
     32,   0,   1,   0,   0,   0, 
      1,   0,   0,   0,  70, 142, 
     32,   0,   1,   0,   0,   0, 
      4,   0,   0,   0,  70,  14, 
     16,   0,   0,   0,   0,   0, 
     50,   0,   0,  11, 242,   0, 
     16,   0,   0,   0,   0,   0, 
    166, 138,  32,   0,   1,   0, 
      0,   0,   1,   0,   0,   0, 
     70, 142,  32,   0,   1,   0, 
      0,   0,   6,   0,   0,   0, 
     70,  14,  16,   0,   0,   0, 
      0,   0,  50,   0,   0,  11, 
    242,   0,  16,   0,   0,   0, 
      0,   0, 246, 143,  32,   0, 
      1,   0,   0,   0,   1,   0, 
      0,   0,  70, 142,  32,   0, 
      1,   0,   0,   0,   7,   0, 
      0,   0,  70,  14,  16,   0, 
      0,   0,   0,   0,  56,   0, 
      0,   8, 242,   0,  16,   0, 
      0,   0,   0,   0,  70,  14, 
     16,   0,   0,   0,   0,   0, 
     86,  21,  32,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     56,   0,   0,   9, 242,   0, 
     16,   0,   1,   0,   0,   0, 
     86, 133,  32,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
     70, 142,  32,   0,   1,   0, 
      0,   0,   5,   0,   0,   0, 
     50,   0,   0,  11, 242,   0, 
     16,   0,   1,   0,   0,   0, 
      6, 128,  32,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
     70, 142,  32,   0,   1,   0, 
      0,   0,   4,   0,   0,   0, 
     70,  14,  16,   0,   1,   0, 
      0,   0,  50,   0,   0,  11, 
    242,   0,  16,   0,   1,   0, 
      0,   0, 166, 138,  32,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,  70, 142,  32,   0, 
      1,   0,   0,   0,   6,   0, 
      0,   0,  70,  14,  16,   0, 
      1,   0,   0,   0,  50,   0, 
      0,  11, 242,   0,  16,   0, 
      1,   0,   0,   0, 246, 143, 
     32,   0,   1,   0,   0,   0, 
      0,   0,   0,   0,  70, 142, 
     32,   0,   1,   0,   0,   0, 
      7,   0,   0,   0,  70,  14, 
     16,   0,   1,   0,   0,   0, 
     50,   0,   0,  10, 242,   0, 
     16,   0,   0,   0,   0,   0, 
      6,  16,  32,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     70,  14,  16,   0,   1,   0, 
      0,   0,  70,  14,  16,   0, 
      0,   0,   0,   0,  56,   0, 
      0,   9, 242,   0,  16,   0, 
      1,   0,   0,   0,  86, 133, 
     32,   0,   1,   0,   0,   0, 
      2,   0,   0,   0,  70, 142, 
     32,   0,   1,   0,   0,   0, 
      5,   0,   0,   0,  50,   0, 
      0,  11, 242,   0,  16,   0, 
      1,   0,   0,   0,   6, 128, 
     32,   0,   1,   0,   0,   0, 
      2,   0,   0,   0,  70, 142, 
     32,   0,   1,   0,   0,   0, 
      4,   0,   0,   0,  70,  14, 
     16,   0,   1,   0,   0,   0, 
     50,   0,   0,  11, 242,   0, 
     16,   0,   1,   0,   0,   0, 
    166, 138,  32,   0,   1,   0, 
      0,   0,   2,   0,   0,   0, 
     70, 142,  32,   0,   1,   0, 
      0,   0,   6,   0,   0,   0, 
     70,  14,  16,   0,   1,   0, 
      0,   0,  50,   0,   0,  11, 
    242,   0,  16,   0,   1,   0, 
      0,   0, 246, 143,  32,   0, 
      1,   0,   0,   0,   2,   0, 
      0,   0,  70, 142,  32,   0, 
      1,   0,   0,   0,   7,   0, 
      0,   0,  70,  14,  16,   0, 
      1,   0,   0,   0,  50,   0, 
      0,  10, 242,   0,  16,   0, 
      0,   0,   0,   0, 166,  26, 
     32,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,  70,  14, 
     16,   0,   1,   0,   0,   0, 
     70,  14,  16,   0,   0,   0, 
      0,   0,  56,   0,   0,   9, 
    242,   0,  16,   0,   1,   0, 
      0,   0,  86, 133,  32,   0, 
      1,   0,   0,   0,   3,   0, 
      0,   0,  70, 142,  32,   0, 
      1,   0,   0,   0,   5,   0, 
      0,   0,  50,   0,   0,  11, 
    242,   0,  16,   0,   1,   0, 
      0,   0,   6, 128,  32,   0, 
      1,   0,   0,   0,   3,   0, 
      0,   0,  70, 142,  32,   0, 
      1,   0,   0,   0,   4,   0, 
      0,   0,  70,  14,  16,   0, 
      1,   0,   0,   0,  50,   0, 
      0,  11, 242,   0,  16,   0, 
      1,   0,   0,   0, 166, 138, 
     32,   0,   1,   0,   0,   0, 
      3,   0,   0,   0,  70, 142, 
     32,   0,   1,   0,   0,   0, 
      6,   0,   0,   0,  70,  14, 
     16,   0,   1,   0,   0,   0, 
     50,   0,   0,  11, 242,   0, 
     16,   0,   1,   0,   0,   0, 
    246, 143,  32,   0,   1,   0, 
      0,   0,   3,   0,   0,   0, 
     70, 142,  32,   0,   1,   0, 
      0,   0,   7,   0,   0,   0, 
     70,  14,  16,   0,   1,   0, 
      0,   0,   0,   0,   0,   7, 
    242,   0,  16,   0,   0,   0, 
      0,   0,  70,  14,  16,   0, 
      0,   0,   0,   0,  70,  14, 
     16,   0,   1,   0,   0,   0, 
     56,   0,   0,   8,  18,   0, 
     16,   0,   1,   0,   0,   0, 
      1,  64,   0,   0,   0,   0, 
      0,  63,  10,  16,  32,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,  77,   0,   0,   8, 
     18,   0,  16,   0,   2,   0, 
      0,   0,  18,   0,  16,   0, 
      3,   0,   0,   0,  58,  16, 
     32,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,  56,   0, 
      0,   7,  34,   0,  16,   0, 
      1,   0,   0,   0,  10,   0, 
     16,   0,   1,   0,   0,   0, 
     10,   0,  16,   0,   2,   0, 
      0,   0,  50,   0,   0,  11, 
     66,   0,  16,   0,   1,   0, 
      0,   0,  10,   0,  16, 128, 
     65,   0,   0,   0,   1,   0, 
      0,   0,  10,   0,  16,   0, 
      3,   0,   0,   0,  26,   0, 
     16, 128,  65,   0,   0,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   7,  66,   0,  16,   0, 
      1,   0,   0,   0,  10,   0, 
     16,   0,   0,   0,   0,   0, 
     42,   0,  16,   0,   1,   0, 
      0,   0,  54,   0,   0,   5, 
     18,  32,  16,   0,   1,   0, 
      0,   0,  42,   0,  16,   0, 
      1,   0,   0,   0,  56,   0, 
      0,   8,  66,   0,  16,   0, 
      1,   0,   0,   0,  10,   0, 
     16, 128,  65,   0,   0,   0, 
      1,   0,   0,   0,  10,   0, 
     16,   0,   2,   0,   0,   0, 
     50,   0,   0,   9, 130,   0, 
     16,   0,   1,   0,   0,   0, 
     10,   0,  16,   0,   1,   0, 
      0,   0,  10,   0,  16,   0, 
      3,   0,   0,   0,  42,   0, 
     16,   0,   1,   0,   0,   0, 
      0,   0,   0,   7, 130,   0, 
     16,   0,   1,   0,   0,   0, 
     26,   0,  16,   0,   0,   0, 
      0,   0,  58,   0,  16,   0, 
      1,   0,   0,   0,  54,   0, 
      0,   5,  34,  32,  16,   0, 
      1,   0,   0,   0,  58,   0, 
     16,   0,   1,   0,   0,   0, 
     54,   0,   0,   5, 194,  32, 
     16,   0,   1,   0,   0,   0, 
    166,  14,  16,   0,   0,   0, 
      0,   0,  54,   0,   0,   6, 
    242,  32,  16,   0,   2,   0, 
      0,   0,  70,  30,  32,   0, 
      0,   0,   0,   0,   2,   0, 
      0,   0, 117,   0,   0,   3, 
      0,   0,  17,   0,   0,   0, 
      0,   0,  54,   0,   0,   8, 
     50,  32,  16,   0,   0,   0, 
      0,   0,   2,  64,   0,   0, 
      0,   0, 128,  63,   0,   0, 
    128,  63,   0,   0,   0,   0, 
      0,   0,   0,   0,  50,   0, 
      0,  10, 130,   0,  16,   0, 
      1,   0,   0,   0,  10,   0, 
     16,   0,   1,   0,   0,   0, 
     10,   0,  16,   0,   3,   0, 
      0,   0,  26,   0,  16, 128, 
     65,   0,   0,   0,   1,   0, 
      0,   0,   0,   0,   0,   7, 
    130,   0,  16,   0,   1,   0, 
      0,   0,  10,   0,  16,   0, 
      0,   0,   0,   0,  58,   0, 
     16,   0,   1,   0,   0,   0, 
     54,   0,   0,   5,  18,  32, 
     16,   0,   1,   0,   0,   0, 
     58,   0,  16,   0,   1,   0, 
      0,   0,  50,   0,   0,   9, 
    130,   0,  16,   0,   1,   0, 
      0,   0,  10,   0,  16,   0, 
      1,   0,   0,   0,  10,   0, 
     16,   0,   3,   0,   0,   0, 
     26,   0,  16,   0,   1,   0, 
      0,   0,  50,   0,   0,  10, 
     34,   0,  16,   0,   1,   0, 
      0,   0,  10,   0,  16, 128, 
     65,   0,   0,   0,   1,   0, 
      0,   0,  10,   0,  16,   0, 
      3,   0,   0,   0,  26,   0, 
     16,   0,   1,   0,   0,   0, 
      0,   0,   0,   7, 162,   0, 
     16,   0,   1,   0,   0,   0, 
     86,   5,  16,   0,   0,   0, 
      0,   0,  86,  13,  16,   0, 
      1,   0,   0,   0,  54,   0, 
      0,   5,  34,  32,  16,   0, 
      1,   0,   0,   0,  58,   0, 
     16,   0,   1,   0,   0,   0, 
     54,   0,   0,   5, 194,  32, 
     16,   0,   1,   0,   0,   0, 
    166,  14,  16,   0,   0,   0, 
      0,   0,  54,   0,   0,   6, 
    242,  32,  16,   0,   2,   0, 
      0,   0,  70,  30,  32,   0, 
      0,   0,   0,   0,   2,   0, 
      0,   0, 117,   0,   0,   3, 
      0,   0,  17,   0,   0,   0, 
      0,   0,  54,   0,   0,   8, 
     50,  32,  16,   0,   0,   0, 
      0,   0,   2,  64,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,  50,   0, 
      0,  11, 130,   0,  16,   0, 
      1,   0,   0,   0,  10,   0, 
     16, 128,  65,   0,   0,   0, 
      1,   0,   0,   0,  10,   0, 
     16,   0,   3,   0,   0,   0, 
     42,   0,  16, 128,  65,   0, 
      0,   0,   1,   0,   0,   0, 
      0,   0,   0,   7, 130,   0, 
     16,   0,   1,   0,   0,   0, 
     10,   0,  16,   0,   0,   0, 
      0,   0,  58,   0,  16,   0, 
      1,   0,   0,   0,  54,   0, 
      0,   5,  18,  32,  16,   0, 
      1,   0,   0,   0,  58,   0, 
     16,   0,   1,   0,   0,   0, 
     50,   0,   0,  10, 130,   0, 
     16,   0,   1,   0,   0,   0, 
     10,   0,  16, 128,  65,   0, 
      0,   0,   1,   0,   0,   0, 
     10,   0,  16,   0,   3,   0, 
      0,   0,  42,   0,  16,   0, 
      1,   0,   0,   0,  50,   0, 
      0,  10,  18,   0,  16,   0, 
      1,   0,   0,   0,  10,   0, 
     16,   0,   1,   0,   0,   0, 
     10,   0,  16,   0,   3,   0, 
      0,   0,  42,   0,  16, 128, 
     65,   0,   0,   0,   1,   0, 
      0,   0,   0,   0,   0,   7, 
     50,   0,  16,   0,   0,   0, 
      0,   0,  70,   0,  16,   0, 
      0,   0,   0,   0, 198,   0, 
     16,   0,   1,   0,   0,   0, 
     54,   0,   0,   5,  34,  32, 
     16,   0,   1,   0,   0,   0, 
     26,   0,  16,   0,   0,   0, 
      0,   0,  54,   0,   0,   5, 
    194,  32,  16,   0,   1,   0, 
      0,   0, 166,  14,  16,   0, 
      0,   0,   0,   0,  54,   0, 
      0,   6, 242,  32,  16,   0, 
      2,   0,   0,   0,  70,  30, 
     32,   0,   0,   0,   0,   0, 
      2,   0,   0,   0, 117,   0, 
      0,   3,   0,   0,  17,   0, 
      0,   0,   0,   0,  54,   0, 
      0,   8,  50,  32,  16,   0, 
      0,   0,   0,   0,   2,  64, 
      0,   0,   0,   0,   0,   0, 
      0,   0, 128,  63,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     54,   0,   0,   5,  18,  32, 
     16,   0,   1,   0,   0,   0, 
     10,   0,  16,   0,   0,   0, 
      0,   0,  54,   0,   0,   5, 
     34,  32,  16,   0,   1,   0, 
      0,   0,  26,   0,  16,   0, 
      1,   0,   0,   0,  54,   0, 
      0,   5, 194,  32,  16,   0, 
      1,   0,   0,   0, 166,  14, 
     16,   0,   0,   0,   0,   0, 
     54,   0,   0,   6, 242,  32, 
     16,   0,   2,   0,   0,   0, 
     70,  30,  32,   0,   0,   0, 
      0,   0,   2,   0,   0,   0, 
    117,   0,   0,   3,   0,   0, 
     17,   0,   0,   0,   0,   0, 
    118,   0,   0,   3,   0,   0, 
     17,   0,   0,   0,   0,   0, 
     62,   0,   0,   1,  83,  84, 
     65,  84, 148,   0,   0,   0, 
     64,   0,   0,   0,   4,   0, 
      0,   0,   0,   0,   0,   0, 
      6,   0,   0,   0,  15,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,   4,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   2,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
      5,   0,   0,   0,   4,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0
};
