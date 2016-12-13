#if 0
//
// Generated by Microsoft (R) HLSL Shader Compiler 9.30.9200.20789
//
//
///
// Buffer Definitions: 
//
// cbuffer OBJECT
// {
//
//   row_major float4x4 Local_worldMatrix;// Offset:    0 Size:    64
//
// }
//
// cbuffer SCENE
// {
//
//   row_major float4x4 ViewMatrix;     // Offset:    0 Size:    64
//   row_major float4x4 projectionMatrix;// Offset:   64 Size:    64
//
// }
//
// cbuffer TRANSPARENCY
// {
//
//   float4 colorRGBA;                  // Offset:    0 Size:    16
//
// }
//
// cbuffer BINDPOS_JOINTS
// {
//
//   row_major float4x4 BindPos[50];    // Offset:    0 Size:  3200
//
// }
//
// cbuffer INTERPOLATED_JOINTS
// {
//
//   row_major float4x4 Interpolated[50];// Offset:    0 Size:  3200
//
// }
//
// cbuffer UV_SCROLLING
// {
//
//   float4 uv_offsets;                 // Offset:    0 Size:    16
//
// }
//
//
// Resource Bindings:
//
// Name                                 Type  Format         Dim Slot Elements
// ------------------------------ ---------- ------- ----------- ---- --------
// OBJECT                            cbuffer      NA          NA    0        1
// SCENE                             cbuffer      NA          NA    1        1
// TRANSPARENCY                      cbuffer      NA          NA    2        1
// BINDPOS_JOINTS                    cbuffer      NA          NA    3        1
// INTERPOLATED_JOINTS               cbuffer      NA          NA    4        1
// UV_SCROLLING                      cbuffer      NA          NA    5        1
//
//
//
// Input signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// POSITION                 0   xyz         0     NONE   float   xyz 
// NORMAL                   0   xyz         1     NONE   float   xyz 
// TEXCOORD                 0   xy          2     NONE   float   xy  
// BLENDINDICES             0   xyzw        3     NONE    uint   xyzw
// BLENDWEIGHT              0   xyzw        4     NONE   float   xyzw
//
//
// Output signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// SV_POSITION              0   xyzw        0      POS   float   xyzw
// COLOR                    0   xyzw        1     NONE   float   xyzw
// POSITION                 0   xyz         2     NONE   float   xyz 
// TEXTURECOORD             0   xyz         3     NONE   float   xyz 
// NORMAL                   0   xyz         4     NONE   float   xyz 
//
vs_5_0
dcl_globalFlags refactoringAllowed
dcl_constantbuffer cb0[4], immediateIndexed
dcl_constantbuffer cb1[8], immediateIndexed
dcl_constantbuffer cb2[1], immediateIndexed
dcl_constantbuffer cb3[200], dynamicIndexed
dcl_constantbuffer cb4[200], dynamicIndexed
dcl_constantbuffer cb5[1], immediateIndexed
dcl_input v0.xyz
dcl_input v1.xyz
dcl_input v2.xy
dcl_input v3.xyzw
dcl_input v4.xyzw
dcl_output_siv o0.xyzw, position
dcl_output o1.xyzw
dcl_output o2.xyz
dcl_output o3.xyz
dcl_output o4.xyz
dcl_temps 4
ishl r0.x, v3.x, l(2)
mul r1.xyzw, v0.yyyy, cb3[r0.x + 1].xyzw
mad r1.xyzw, v0.xxxx, cb3[r0.x + 0].xyzw, r1.xyzw
mad r1.xyzw, v0.zzzz, cb3[r0.x + 2].xyzw, r1.xyzw
add r1.xyzw, r1.xyzw, cb3[r0.x + 3].xyzw
mul r0.yzw, r1.yyyy, cb4[r0.x + 1].xxyz
mad r0.yzw, r1.xxxx, cb4[r0.x + 0].xxyz, r0.yyzw
mad r0.yzw, r1.zzzz, cb4[r0.x + 2].xxyz, r0.yyzw
mad r0.xyz, r1.wwww, cb4[r0.x + 3].xyzx, r0.yzwy
mul r0.xyz, r0.xyzx, v4.xxxx
lt r1.xyz, l(0.000000, 0.000000, 0.000000, 0.000000), v4.yzwy
if_nz r1.x
  ishl r0.w, v3.y, l(2)
  mul r2.xyzw, v0.yyyy, cb3[r0.w + 1].xyzw
  mad r2.xyzw, v0.xxxx, cb3[r0.w + 0].xyzw, r2.xyzw
  mad r2.xyzw, v0.zzzz, cb3[r0.w + 2].xyzw, r2.xyzw
  add r2.xyzw, r2.xyzw, cb3[r0.w + 3].xyzw
  mul r3.xyz, r2.yyyy, cb4[r0.w + 1].xyzx
  mad r3.xyz, r2.xxxx, cb4[r0.w + 0].xyzx, r3.xyzx
  mad r2.xyz, r2.zzzz, cb4[r0.w + 2].xyzx, r3.xyzx
  mad r2.xyz, r2.wwww, cb4[r0.w + 3].xyzx, r2.xyzx
  mad r0.xyz, v4.yyyy, r2.xyzx, r0.xyzx
endif 
if_nz r1.y
  ishl r0.w, v3.z, l(2)
  mul r2.xyzw, v0.yyyy, cb3[r0.w + 1].xyzw
  mad r2.xyzw, v0.xxxx, cb3[r0.w + 0].xyzw, r2.xyzw
  mad r2.xyzw, v0.zzzz, cb3[r0.w + 2].xyzw, r2.xyzw
  add r2.xyzw, r2.xyzw, cb3[r0.w + 3].xyzw
  mul r1.xyw, r2.yyyy, cb4[r0.w + 1].xyxz
  mad r1.xyw, r2.xxxx, cb4[r0.w + 0].xyxz, r1.xyxw
  mad r1.xyw, r2.zzzz, cb4[r0.w + 2].xyxz, r1.xyxw
  mad r1.xyw, r2.wwww, cb4[r0.w + 3].xyxz, r1.xyxw
  mad r0.xyz, v4.zzzz, r1.xywx, r0.xyzx
endif 
if_nz r1.z
  ishl r0.w, v3.w, l(2)
  mul r1.xyzw, v0.yyyy, cb3[r0.w + 1].xyzw
  mad r1.xyzw, v0.xxxx, cb3[r0.w + 0].xyzw, r1.xyzw
  mad r1.xyzw, v0.zzzz, cb3[r0.w + 2].xyzw, r1.xyzw
  add r1.xyzw, r1.xyzw, cb3[r0.w + 3].xyzw
  mul r2.xyz, r1.yyyy, cb4[r0.w + 1].xyzx
  mad r2.xyz, r1.xxxx, cb4[r0.w + 0].xyzx, r2.xyzx
  mad r1.xyz, r1.zzzz, cb4[r0.w + 2].xyzx, r2.xyzx
  mad r1.xyz, r1.wwww, cb4[r0.w + 3].xyzx, r1.xyzx
  mad r0.xyz, v4.wwww, r1.xyzx, r0.xyzx
endif 
mul r1.xyzw, r0.yyyy, cb0[1].xyzw
mad r1.xyzw, r0.xxxx, cb0[0].xyzw, r1.xyzw
mad r0.xyzw, r0.zzzz, cb0[2].xyzw, r1.xyzw
add r0.xyzw, r0.xyzw, cb0[3].xyzw
mul r1.xyzw, r0.yyyy, cb1[1].xyzw
mad r1.xyzw, r0.xxxx, cb1[0].xyzw, r1.xyzw
mad r1.xyzw, r0.zzzz, cb1[2].xyzw, r1.xyzw
mad r1.xyzw, r0.wwww, cb1[3].xyzw, r1.xyzw
mul r2.xyzw, r1.yyyy, cb1[5].xyzw
mad r2.xyzw, r1.xxxx, cb1[4].xyzw, r2.xyzw
mad r2.xyzw, r1.zzzz, cb1[6].xyzw, r2.xyzw
mad o0.xyzw, r1.wwww, cb1[7].xyzw, r2.xyzw
add o3.xy, v2.xyxx, cb5[0].xyxx
mul r1.xyz, v1.yyyy, cb0[1].xyzx
mad r1.xyz, v1.xxxx, cb0[0].xyzx, r1.xyzx
mad r1.xyz, v1.zzzz, cb0[2].xyzx, r1.xyzx
add o4.xyz, r1.xyzx, cb0[3].xyzx
mov o1.xyzw, cb2[0].xyzw
mov o2.xyz, r0.xyzx
mov o3.z, l(0)
ret 
// Approximately 68 instruction slots used
#endif

const BYTE Anim_VS[] =
{
     68,  88,  66,  67,  99,   3, 
    250, 163, 106, 200,   7,  66, 
    142, 186,   0, 216, 254, 120, 
    138, 236,   1,   0,   0,   0, 
    236,  16,   0,   0,   5,   0, 
      0,   0,  52,   0,   0,   0, 
     96,   4,   0,   0,  28,   5, 
      0,   0, 212,   5,   0,   0, 
     80,  16,   0,   0,  82,  68, 
     69,  70,  36,   4,   0,   0, 
      6,   0,   0,   0,  72,   1, 
      0,   0,   6,   0,   0,   0, 
     60,   0,   0,   0,   0,   5, 
    254, 255,   0,   1,   0,   0, 
    239,   3,   0,   0,  82,  68, 
     49,  49,  60,   0,   0,   0, 
     24,   0,   0,   0,  32,   0, 
      0,   0,  40,   0,   0,   0, 
     36,   0,   0,   0,  12,   0, 
      0,   0,   0,   0,   0,   0, 
    252,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,   1,   0, 
      0,   0,   3,   1,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   1,   0,   0,   0, 
      1,   0,   0,   0,   9,   1, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      2,   0,   0,   0,   1,   0, 
      0,   0,   1,   0,   0,   0, 
     22,   1,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   3,   0,   0,   0, 
      1,   0,   0,   0,   1,   0, 
      0,   0,  37,   1,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   4,   0, 
      0,   0,   1,   0,   0,   0, 
      1,   0,   0,   0,  57,   1, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      5,   0,   0,   0,   1,   0, 
      0,   0,   1,   0,   0,   0, 
     79,  66,  74,  69,  67,  84, 
      0,  83,  67,  69,  78,  69, 
      0,  84,  82,  65,  78,  83, 
     80,  65,  82,  69,  78,  67, 
     89,   0,  66,  73,  78,  68, 
     80,  79,  83,  95,  74,  79, 
     73,  78,  84,  83,   0,  73, 
     78,  84,  69,  82,  80,  79, 
     76,  65,  84,  69,  68,  95, 
     74,  79,  73,  78,  84,  83, 
      0,  85,  86,  95,  83,  67, 
     82,  79,  76,  76,  73,  78, 
     71,   0, 171, 171, 252,   0, 
      0,   0,   1,   0,   0,   0, 
    216,   1,   0,   0,  64,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   3,   1, 
      0,   0,   2,   0,   0,   0, 
     64,   2,   0,   0, 128,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   9,   1, 
      0,   0,   1,   0,   0,   0, 
    172,   2,   0,   0,  16,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,  22,   1, 
      0,   0,   1,   0,   0,   0, 
     12,   3,   0,   0, 128,  12, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,  37,   1, 
      0,   0,   1,   0,   0,   0, 
     96,   3,   0,   0, 128,  12, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,  57,   1, 
      0,   0,   1,   0,   0,   0, 
    188,   3,   0,   0,  16,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   2, 
      0,   0,   0,   0,   0,   0, 
     64,   0,   0,   0,   2,   0, 
      0,   0,  28,   2,   0,   0, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0,  76, 111,  99,  97, 
    108,  95, 119, 111, 114, 108, 
    100,  77,  97, 116, 114, 105, 
    120,   0, 102, 108, 111,  97, 
    116,  52, 120,  52,   0, 171, 
      2,   0,   3,   0,   4,   0, 
      4,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  18,   2,   0,   0, 
    144,   2,   0,   0,   0,   0, 
      0,   0,  64,   0,   0,   0, 
      2,   0,   0,   0,  28,   2, 
      0,   0,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 155,   2, 
      0,   0,  64,   0,   0,   0, 
     64,   0,   0,   0,   2,   0, 
      0,   0,  28,   2,   0,   0, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0,  86, 105, 101, 119, 
     77,  97, 116, 114, 105, 120, 
      0, 112, 114, 111, 106, 101, 
     99, 116, 105, 111, 110,  77, 
     97, 116, 114, 105, 120,   0, 
    212,   2,   0,   0,   0,   0, 
      0,   0,  16,   0,   0,   0, 
      2,   0,   0,   0, 232,   2, 
      0,   0,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0,  99, 111, 
    108, 111, 114,  82,  71,  66, 
     65,   0, 102, 108, 111,  97, 
    116,  52,   0, 171, 171, 171, 
      1,   0,   3,   0,   1,   0, 
      4,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0, 222,   2,   0,   0, 
     52,   3,   0,   0,   0,   0, 
      0,   0, 128,  12,   0,   0, 
      2,   0,   0,   0,  60,   3, 
      0,   0,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0,  66, 105, 
    110, 100,  80, 111, 115,   0, 
      2,   0,   3,   0,   4,   0, 
      4,   0,  50,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  18,   2,   0,   0, 
    136,   3,   0,   0,   0,   0, 
      0,   0, 128,  12,   0,   0, 
      2,   0,   0,   0, 152,   3, 
      0,   0,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0,  73, 110, 
    116, 101, 114, 112, 111, 108, 
     97, 116, 101, 100,   0, 171, 
    171, 171,   2,   0,   3,   0, 
      4,   0,   4,   0,  50,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,  18,   2, 
      0,   0, 228,   3,   0,   0, 
      0,   0,   0,   0,  16,   0, 
      0,   0,   2,   0,   0,   0, 
    232,   2,   0,   0,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    117, 118,  95, 111, 102, 102, 
    115, 101, 116, 115,   0,  77, 
    105,  99, 114, 111, 115, 111, 
    102, 116,  32,  40,  82,  41, 
     32,  72,  76,  83,  76,  32, 
     83, 104,  97, 100, 101, 114, 
     32,  67, 111, 109, 112, 105, 
    108, 101, 114,  32,  57,  46, 
     51,  48,  46,  57,  50,  48, 
     48,  46,  50,  48,  55,  56, 
     57,   0, 171, 171,  73,  83, 
     71,  78, 180,   0,   0,   0, 
      5,   0,   0,   0,   8,   0, 
      0,   0, 128,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   3,   0,   0,   0, 
      0,   0,   0,   0,   7,   7, 
      0,   0, 137,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   3,   0,   0,   0, 
      1,   0,   0,   0,   7,   7, 
      0,   0, 144,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   3,   0,   0,   0, 
      2,   0,   0,   0,   3,   3, 
      0,   0, 153,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
      3,   0,   0,   0,  15,  15, 
      0,   0, 166,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   3,   0,   0,   0, 
      4,   0,   0,   0,  15,  15, 
      0,   0,  80,  79,  83,  73, 
     84,  73,  79,  78,   0,  78, 
     79,  82,  77,  65,  76,   0, 
     84,  69,  88,  67,  79,  79, 
     82,  68,   0,  66,  76,  69, 
     78,  68,  73,  78,  68,  73, 
     67,  69,  83,   0,  66,  76, 
     69,  78,  68,  87,  69,  73, 
     71,  72,  84,   0, 171, 171, 
     79,  83,  71,  78, 176,   0, 
      0,   0,   5,   0,   0,   0, 
      8,   0,   0,   0, 128,   0, 
      0,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,   3,   0, 
      0,   0,   0,   0,   0,   0, 
     15,   0,   0,   0, 140,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   1,   0,   0,   0, 
     15,   0,   0,   0, 146,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   2,   0,   0,   0, 
      7,   8,   0,   0, 155,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   3,   0,   0,   0, 
      7,   8,   0,   0, 168,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   4,   0,   0,   0, 
      7,   8,   0,   0,  83,  86, 
     95,  80,  79,  83,  73,  84, 
     73,  79,  78,   0,  67,  79, 
     76,  79,  82,   0,  80,  79, 
     83,  73,  84,  73,  79,  78, 
      0,  84,  69,  88,  84,  85, 
     82,  69,  67,  79,  79,  82, 
     68,   0,  78,  79,  82,  77, 
     65,  76,   0, 171,  83,  72, 
     69,  88, 116,  10,   0,   0, 
     80,   0,   1,   0, 157,   2, 
      0,   0, 106,   8,   0,   1, 
     89,   0,   0,   4,  70, 142, 
     32,   0,   0,   0,   0,   0, 
      4,   0,   0,   0,  89,   0, 
      0,   4,  70, 142,  32,   0, 
      1,   0,   0,   0,   8,   0, 
      0,   0,  89,   0,   0,   4, 
     70, 142,  32,   0,   2,   0, 
      0,   0,   1,   0,   0,   0, 
     89,   8,   0,   4,  70, 142, 
     32,   0,   3,   0,   0,   0, 
    200,   0,   0,   0,  89,   8, 
      0,   4,  70, 142,  32,   0, 
      4,   0,   0,   0, 200,   0, 
      0,   0,  89,   0,   0,   4, 
     70, 142,  32,   0,   5,   0, 
      0,   0,   1,   0,   0,   0, 
     95,   0,   0,   3, 114,  16, 
     16,   0,   0,   0,   0,   0, 
     95,   0,   0,   3, 114,  16, 
     16,   0,   1,   0,   0,   0, 
     95,   0,   0,   3,  50,  16, 
     16,   0,   2,   0,   0,   0, 
     95,   0,   0,   3, 242,  16, 
     16,   0,   3,   0,   0,   0, 
     95,   0,   0,   3, 242,  16, 
     16,   0,   4,   0,   0,   0, 
    103,   0,   0,   4, 242,  32, 
     16,   0,   0,   0,   0,   0, 
      1,   0,   0,   0, 101,   0, 
      0,   3, 242,  32,  16,   0, 
      1,   0,   0,   0, 101,   0, 
      0,   3, 114,  32,  16,   0, 
      2,   0,   0,   0, 101,   0, 
      0,   3, 114,  32,  16,   0, 
      3,   0,   0,   0, 101,   0, 
      0,   3, 114,  32,  16,   0, 
      4,   0,   0,   0, 104,   0, 
      0,   2,   4,   0,   0,   0, 
     41,   0,   0,   7,  18,   0, 
     16,   0,   0,   0,   0,   0, 
     10,  16,  16,   0,   3,   0, 
      0,   0,   1,  64,   0,   0, 
      2,   0,   0,   0,  56,   0, 
      0,  10, 242,   0,  16,   0, 
      1,   0,   0,   0,  86,  21, 
     16,   0,   0,   0,   0,   0, 
     70, 142,  32,   6,   3,   0, 
      0,   0,   1,   0,   0,   0, 
     10,   0,  16,   0,   0,   0, 
      0,   0,  50,   0,   0,  11, 
    242,   0,  16,   0,   1,   0, 
      0,   0,   6,  16,  16,   0, 
      0,   0,   0,   0,  70, 142, 
     32,   4,   3,   0,   0,   0, 
     10,   0,  16,   0,   0,   0, 
      0,   0,  70,  14,  16,   0, 
      1,   0,   0,   0,  50,   0, 
      0,  12, 242,   0,  16,   0, 
      1,   0,   0,   0, 166,  26, 
     16,   0,   0,   0,   0,   0, 
     70, 142,  32,   6,   3,   0, 
      0,   0,   2,   0,   0,   0, 
     10,   0,  16,   0,   0,   0, 
      0,   0,  70,  14,  16,   0, 
      1,   0,   0,   0,   0,   0, 
      0,  10, 242,   0,  16,   0, 
      1,   0,   0,   0,  70,  14, 
     16,   0,   1,   0,   0,   0, 
     70, 142,  32,   6,   3,   0, 
      0,   0,   3,   0,   0,   0, 
     10,   0,  16,   0,   0,   0, 
      0,   0,  56,   0,   0,  10, 
    226,   0,  16,   0,   0,   0, 
      0,   0,  86,   5,  16,   0, 
      1,   0,   0,   0,   6, 137, 
     32,   6,   4,   0,   0,   0, 
      1,   0,   0,   0,  10,   0, 
     16,   0,   0,   0,   0,   0, 
     50,   0,   0,  11, 226,   0, 
     16,   0,   0,   0,   0,   0, 
      6,   0,  16,   0,   1,   0, 
      0,   0,   6, 137,  32,   4, 
      4,   0,   0,   0,  10,   0, 
     16,   0,   0,   0,   0,   0, 
     86,  14,  16,   0,   0,   0, 
      0,   0,  50,   0,   0,  12, 
    226,   0,  16,   0,   0,   0, 
      0,   0, 166,  10,  16,   0, 
      1,   0,   0,   0,   6, 137, 
     32,   6,   4,   0,   0,   0, 
      2,   0,   0,   0,  10,   0, 
     16,   0,   0,   0,   0,   0, 
     86,  14,  16,   0,   0,   0, 
      0,   0,  50,   0,   0,  12, 
    114,   0,  16,   0,   0,   0, 
      0,   0, 246,  15,  16,   0, 
      1,   0,   0,   0,  70, 130, 
     32,   6,   4,   0,   0,   0, 
      3,   0,   0,   0,  10,   0, 
     16,   0,   0,   0,   0,   0, 
    150,   7,  16,   0,   0,   0, 
      0,   0,  56,   0,   0,   7, 
    114,   0,  16,   0,   0,   0, 
      0,   0,  70,   2,  16,   0, 
      0,   0,   0,   0,   6,  16, 
     16,   0,   4,   0,   0,   0, 
     49,   0,   0,  10, 114,   0, 
     16,   0,   1,   0,   0,   0, 
      2,  64,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0, 150,  23,  16,   0, 
      4,   0,   0,   0,  31,   0, 
      4,   3,  10,   0,  16,   0, 
      1,   0,   0,   0,  41,   0, 
      0,   7, 130,   0,  16,   0, 
      0,   0,   0,   0,  26,  16, 
     16,   0,   3,   0,   0,   0, 
      1,  64,   0,   0,   2,   0, 
      0,   0,  56,   0,   0,  10, 
    242,   0,  16,   0,   2,   0, 
      0,   0,  86,  21,  16,   0, 
      0,   0,   0,   0,  70, 142, 
     32,   6,   3,   0,   0,   0, 
      1,   0,   0,   0,  58,   0, 
     16,   0,   0,   0,   0,   0, 
     50,   0,   0,  11, 242,   0, 
     16,   0,   2,   0,   0,   0, 
      6,  16,  16,   0,   0,   0, 
      0,   0,  70, 142,  32,   4, 
      3,   0,   0,   0,  58,   0, 
     16,   0,   0,   0,   0,   0, 
     70,  14,  16,   0,   2,   0, 
      0,   0,  50,   0,   0,  12, 
    242,   0,  16,   0,   2,   0, 
      0,   0, 166,  26,  16,   0, 
      0,   0,   0,   0,  70, 142, 
     32,   6,   3,   0,   0,   0, 
      2,   0,   0,   0,  58,   0, 
     16,   0,   0,   0,   0,   0, 
     70,  14,  16,   0,   2,   0, 
      0,   0,   0,   0,   0,  10, 
    242,   0,  16,   0,   2,   0, 
      0,   0,  70,  14,  16,   0, 
      2,   0,   0,   0,  70, 142, 
     32,   6,   3,   0,   0,   0, 
      3,   0,   0,   0,  58,   0, 
     16,   0,   0,   0,   0,   0, 
     56,   0,   0,  10, 114,   0, 
     16,   0,   3,   0,   0,   0, 
     86,   5,  16,   0,   2,   0, 
      0,   0,  70, 130,  32,   6, 
      4,   0,   0,   0,   1,   0, 
      0,   0,  58,   0,  16,   0, 
      0,   0,   0,   0,  50,   0, 
      0,  11, 114,   0,  16,   0, 
      3,   0,   0,   0,   6,   0, 
     16,   0,   2,   0,   0,   0, 
     70, 130,  32,   4,   4,   0, 
      0,   0,  58,   0,  16,   0, 
      0,   0,   0,   0,  70,   2, 
     16,   0,   3,   0,   0,   0, 
     50,   0,   0,  12, 114,   0, 
     16,   0,   2,   0,   0,   0, 
    166,  10,  16,   0,   2,   0, 
      0,   0,  70, 130,  32,   6, 
      4,   0,   0,   0,   2,   0, 
      0,   0,  58,   0,  16,   0, 
      0,   0,   0,   0,  70,   2, 
     16,   0,   3,   0,   0,   0, 
     50,   0,   0,  12, 114,   0, 
     16,   0,   2,   0,   0,   0, 
    246,  15,  16,   0,   2,   0, 
      0,   0,  70, 130,  32,   6, 
      4,   0,   0,   0,   3,   0, 
      0,   0,  58,   0,  16,   0, 
      0,   0,   0,   0,  70,   2, 
     16,   0,   2,   0,   0,   0, 
     50,   0,   0,   9, 114,   0, 
     16,   0,   0,   0,   0,   0, 
     86,  21,  16,   0,   4,   0, 
      0,   0,  70,   2,  16,   0, 
      2,   0,   0,   0,  70,   2, 
     16,   0,   0,   0,   0,   0, 
     21,   0,   0,   1,  31,   0, 
      4,   3,  26,   0,  16,   0, 
      1,   0,   0,   0,  41,   0, 
      0,   7, 130,   0,  16,   0, 
      0,   0,   0,   0,  42,  16, 
     16,   0,   3,   0,   0,   0, 
      1,  64,   0,   0,   2,   0, 
      0,   0,  56,   0,   0,  10, 
    242,   0,  16,   0,   2,   0, 
      0,   0,  86,  21,  16,   0, 
      0,   0,   0,   0,  70, 142, 
     32,   6,   3,   0,   0,   0, 
      1,   0,   0,   0,  58,   0, 
     16,   0,   0,   0,   0,   0, 
     50,   0,   0,  11, 242,   0, 
     16,   0,   2,   0,   0,   0, 
      6,  16,  16,   0,   0,   0, 
      0,   0,  70, 142,  32,   4, 
      3,   0,   0,   0,  58,   0, 
     16,   0,   0,   0,   0,   0, 
     70,  14,  16,   0,   2,   0, 
      0,   0,  50,   0,   0,  12, 
    242,   0,  16,   0,   2,   0, 
      0,   0, 166,  26,  16,   0, 
      0,   0,   0,   0,  70, 142, 
     32,   6,   3,   0,   0,   0, 
      2,   0,   0,   0,  58,   0, 
     16,   0,   0,   0,   0,   0, 
     70,  14,  16,   0,   2,   0, 
      0,   0,   0,   0,   0,  10, 
    242,   0,  16,   0,   2,   0, 
      0,   0,  70,  14,  16,   0, 
      2,   0,   0,   0,  70, 142, 
     32,   6,   3,   0,   0,   0, 
      3,   0,   0,   0,  58,   0, 
     16,   0,   0,   0,   0,   0, 
     56,   0,   0,  10, 178,   0, 
     16,   0,   1,   0,   0,   0, 
     86,   5,  16,   0,   2,   0, 
      0,   0,  70, 136,  32,   6, 
      4,   0,   0,   0,   1,   0, 
      0,   0,  58,   0,  16,   0, 
      0,   0,   0,   0,  50,   0, 
      0,  11, 178,   0,  16,   0, 
      1,   0,   0,   0,   6,   0, 
     16,   0,   2,   0,   0,   0, 
     70, 136,  32,   4,   4,   0, 
      0,   0,  58,   0,  16,   0, 
      0,   0,   0,   0,  70,  12, 
     16,   0,   1,   0,   0,   0, 
     50,   0,   0,  12, 178,   0, 
     16,   0,   1,   0,   0,   0, 
    166,  10,  16,   0,   2,   0, 
      0,   0,  70, 136,  32,   6, 
      4,   0,   0,   0,   2,   0, 
      0,   0,  58,   0,  16,   0, 
      0,   0,   0,   0,  70,  12, 
     16,   0,   1,   0,   0,   0, 
     50,   0,   0,  12, 178,   0, 
     16,   0,   1,   0,   0,   0, 
    246,  15,  16,   0,   2,   0, 
      0,   0,  70, 136,  32,   6, 
      4,   0,   0,   0,   3,   0, 
      0,   0,  58,   0,  16,   0, 
      0,   0,   0,   0,  70,  12, 
     16,   0,   1,   0,   0,   0, 
     50,   0,   0,   9, 114,   0, 
     16,   0,   0,   0,   0,   0, 
    166,  26,  16,   0,   4,   0, 
      0,   0,  70,   3,  16,   0, 
      1,   0,   0,   0,  70,   2, 
     16,   0,   0,   0,   0,   0, 
     21,   0,   0,   1,  31,   0, 
      4,   3,  42,   0,  16,   0, 
      1,   0,   0,   0,  41,   0, 
      0,   7, 130,   0,  16,   0, 
      0,   0,   0,   0,  58,  16, 
     16,   0,   3,   0,   0,   0, 
      1,  64,   0,   0,   2,   0, 
      0,   0,  56,   0,   0,  10, 
    242,   0,  16,   0,   1,   0, 
      0,   0,  86,  21,  16,   0, 
      0,   0,   0,   0,  70, 142, 
     32,   6,   3,   0,   0,   0, 
      1,   0,   0,   0,  58,   0, 
     16,   0,   0,   0,   0,   0, 
     50,   0,   0,  11, 242,   0, 
     16,   0,   1,   0,   0,   0, 
      6,  16,  16,   0,   0,   0, 
      0,   0,  70, 142,  32,   4, 
      3,   0,   0,   0,  58,   0, 
     16,   0,   0,   0,   0,   0, 
     70,  14,  16,   0,   1,   0, 
      0,   0,  50,   0,   0,  12, 
    242,   0,  16,   0,   1,   0, 
      0,   0, 166,  26,  16,   0, 
      0,   0,   0,   0,  70, 142, 
     32,   6,   3,   0,   0,   0, 
      2,   0,   0,   0,  58,   0, 
     16,   0,   0,   0,   0,   0, 
     70,  14,  16,   0,   1,   0, 
      0,   0,   0,   0,   0,  10, 
    242,   0,  16,   0,   1,   0, 
      0,   0,  70,  14,  16,   0, 
      1,   0,   0,   0,  70, 142, 
     32,   6,   3,   0,   0,   0, 
      3,   0,   0,   0,  58,   0, 
     16,   0,   0,   0,   0,   0, 
     56,   0,   0,  10, 114,   0, 
     16,   0,   2,   0,   0,   0, 
     86,   5,  16,   0,   1,   0, 
      0,   0,  70, 130,  32,   6, 
      4,   0,   0,   0,   1,   0, 
      0,   0,  58,   0,  16,   0, 
      0,   0,   0,   0,  50,   0, 
      0,  11, 114,   0,  16,   0, 
      2,   0,   0,   0,   6,   0, 
     16,   0,   1,   0,   0,   0, 
     70, 130,  32,   4,   4,   0, 
      0,   0,  58,   0,  16,   0, 
      0,   0,   0,   0,  70,   2, 
     16,   0,   2,   0,   0,   0, 
     50,   0,   0,  12, 114,   0, 
     16,   0,   1,   0,   0,   0, 
    166,  10,  16,   0,   1,   0, 
      0,   0,  70, 130,  32,   6, 
      4,   0,   0,   0,   2,   0, 
      0,   0,  58,   0,  16,   0, 
      0,   0,   0,   0,  70,   2, 
     16,   0,   2,   0,   0,   0, 
     50,   0,   0,  12, 114,   0, 
     16,   0,   1,   0,   0,   0, 
    246,  15,  16,   0,   1,   0, 
      0,   0,  70, 130,  32,   6, 
      4,   0,   0,   0,   3,   0, 
      0,   0,  58,   0,  16,   0, 
      0,   0,   0,   0,  70,   2, 
     16,   0,   1,   0,   0,   0, 
     50,   0,   0,   9, 114,   0, 
     16,   0,   0,   0,   0,   0, 
    246,  31,  16,   0,   4,   0, 
      0,   0,  70,   2,  16,   0, 
      1,   0,   0,   0,  70,   2, 
     16,   0,   0,   0,   0,   0, 
     21,   0,   0,   1,  56,   0, 
      0,   8, 242,   0,  16,   0, 
      1,   0,   0,   0,  86,   5, 
     16,   0,   0,   0,   0,   0, 
     70, 142,  32,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
     50,   0,   0,  10, 242,   0, 
     16,   0,   1,   0,   0,   0, 
      6,   0,  16,   0,   0,   0, 
      0,   0,  70, 142,  32,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  70,  14,  16,   0, 
      1,   0,   0,   0,  50,   0, 
      0,  10, 242,   0,  16,   0, 
      0,   0,   0,   0, 166,  10, 
     16,   0,   0,   0,   0,   0, 
     70, 142,  32,   0,   0,   0, 
      0,   0,   2,   0,   0,   0, 
     70,  14,  16,   0,   1,   0, 
      0,   0,   0,   0,   0,   8, 
    242,   0,  16,   0,   0,   0, 
      0,   0,  70,  14,  16,   0, 
      0,   0,   0,   0,  70, 142, 
     32,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,  56,   0, 
      0,   8, 242,   0,  16,   0, 
      1,   0,   0,   0,  86,   5, 
     16,   0,   0,   0,   0,   0, 
     70, 142,  32,   0,   1,   0, 
      0,   0,   1,   0,   0,   0, 
     50,   0,   0,  10, 242,   0, 
     16,   0,   1,   0,   0,   0, 
      6,   0,  16,   0,   0,   0, 
      0,   0,  70, 142,  32,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,  70,  14,  16,   0, 
      1,   0,   0,   0,  50,   0, 
      0,  10, 242,   0,  16,   0, 
      1,   0,   0,   0, 166,  10, 
     16,   0,   0,   0,   0,   0, 
     70, 142,  32,   0,   1,   0, 
      0,   0,   2,   0,   0,   0, 
     70,  14,  16,   0,   1,   0, 
      0,   0,  50,   0,   0,  10, 
    242,   0,  16,   0,   1,   0, 
      0,   0, 246,  15,  16,   0, 
      0,   0,   0,   0,  70, 142, 
     32,   0,   1,   0,   0,   0, 
      3,   0,   0,   0,  70,  14, 
     16,   0,   1,   0,   0,   0, 
     56,   0,   0,   8, 242,   0, 
     16,   0,   2,   0,   0,   0, 
     86,   5,  16,   0,   1,   0, 
      0,   0,  70, 142,  32,   0, 
      1,   0,   0,   0,   5,   0, 
      0,   0,  50,   0,   0,  10, 
    242,   0,  16,   0,   2,   0, 
      0,   0,   6,   0,  16,   0, 
      1,   0,   0,   0,  70, 142, 
     32,   0,   1,   0,   0,   0, 
      4,   0,   0,   0,  70,  14, 
     16,   0,   2,   0,   0,   0, 
     50,   0,   0,  10, 242,   0, 
     16,   0,   2,   0,   0,   0, 
    166,  10,  16,   0,   1,   0, 
      0,   0,  70, 142,  32,   0, 
      1,   0,   0,   0,   6,   0, 
      0,   0,  70,  14,  16,   0, 
      2,   0,   0,   0,  50,   0, 
      0,  10, 242,  32,  16,   0, 
      0,   0,   0,   0, 246,  15, 
     16,   0,   1,   0,   0,   0, 
     70, 142,  32,   0,   1,   0, 
      0,   0,   7,   0,   0,   0, 
     70,  14,  16,   0,   2,   0, 
      0,   0,   0,   0,   0,   8, 
     50,  32,  16,   0,   3,   0, 
      0,   0,  70,  16,  16,   0, 
      2,   0,   0,   0,  70, 128, 
     32,   0,   5,   0,   0,   0, 
      0,   0,   0,   0,  56,   0, 
      0,   8, 114,   0,  16,   0, 
      1,   0,   0,   0,  86,  21, 
     16,   0,   1,   0,   0,   0, 
     70, 130,  32,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
     50,   0,   0,  10, 114,   0, 
     16,   0,   1,   0,   0,   0, 
      6,  16,  16,   0,   1,   0, 
      0,   0,  70, 130,  32,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  70,   2,  16,   0, 
      1,   0,   0,   0,  50,   0, 
      0,  10, 114,   0,  16,   0, 
      1,   0,   0,   0, 166,  26, 
     16,   0,   1,   0,   0,   0, 
     70, 130,  32,   0,   0,   0, 
      0,   0,   2,   0,   0,   0, 
     70,   2,  16,   0,   1,   0, 
      0,   0,   0,   0,   0,   8, 
    114,  32,  16,   0,   4,   0, 
      0,   0,  70,   2,  16,   0, 
      1,   0,   0,   0,  70, 130, 
     32,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,  54,   0, 
      0,   6, 242,  32,  16,   0, 
      1,   0,   0,   0,  70, 142, 
     32,   0,   2,   0,   0,   0, 
      0,   0,   0,   0,  54,   0, 
      0,   5, 114,  32,  16,   0, 
      2,   0,   0,   0,  70,   2, 
     16,   0,   0,   0,   0,   0, 
     54,   0,   0,   5,  66,  32, 
     16,   0,   3,   0,   0,   0, 
      1,  64,   0,   0,   0,   0, 
      0,   0,  62,   0,   0,   1, 
     83,  84,  65,  84, 148,   0, 
      0,   0,  68,   0,   0,   0, 
      4,   0,   0,   0,   0,   0, 
      0,   0,  10,   0,   0,   0, 
     21,   0,   0,   0,   4,   0, 
      0,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,   3,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     10,   0,   0,   0,   0,   0, 
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
      0,   0,   0,   0,   0,   0
};