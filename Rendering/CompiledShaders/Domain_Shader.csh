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
//
// Resource Bindings:
//
// Name                                 Type  Format         Dim Slot Elements
// ------------------------------ ---------- ------- ----------- ---- --------
// OBJECT                            cbuffer      NA          NA    0        1
//
//
//
// Patch Constant signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// SV_TessFactor            0   x           0  TRIEDGE   float       
// TESSELATED               0    y          0     NONE    uint    y  
// SV_TessFactor            1   x           1  TRIEDGE   float       
// SV_TessFactor            2   x           2  TRIEDGE   float       
// SV_InsideTessFactor      0   x           3   TRIINT   float       
//
//
// Input signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// SV_POSITION              0   xyzw        0     NONE   float       
// COLOR                    0   xyzw        1     NONE   float       
// POSITION                 0   xyz         2     NONE   float   xyz 
// TEXTURECOORD             0   xyz         3     NONE   float   xyz 
// NORMAL                   0   xyz         4     NONE   float   xyz 
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
// BFUZED                   0   x           5     NONE    uint   x   
//
// Tessellation Domain   # of control points
// -------------------- --------------------
// Triangle                                3
//
ds_5_0
dcl_input_control_point_count 3
dcl_tessellator_domain domain_tri
dcl_globalFlags refactoringAllowed
dcl_constantbuffer cb0[4], immediateIndexed
dcl_input vpc0.y
dcl_input vDomain.xyz
dcl_input vicp[3][2].xyz
dcl_input vicp[3][3].xyz
dcl_input vicp[3][4].xyz
dcl_output_siv o0.xyzw, position
dcl_output o1.xyzw
dcl_output o2.xyz
dcl_output o3.xyz
dcl_output o4.xyz
dcl_output o5.x
dcl_temps 2
mul r0.xyz, vDomain.yyyy, vicp[1][2].xyzx
mad r0.xyz, vicp[0][2].xyzx, vDomain.xxxx, r0.xyzx
mad r0.xyz, vicp[2][2].xyzx, vDomain.zzzz, r0.xyzx
mul r1.xyzw, r0.yyyy, cb0[1].xyzw
mad r1.xyzw, r0.xxxx, cb0[0].xyzw, r1.xyzw
mad r0.xyzw, r0.zzzz, cb0[2].xyzw, r1.xyzw
add r0.xyzw, r0.xyzw, cb0[3].xyzw
mov o0.xyzw, r0.xyzw
mov o2.xyz, r0.xyzx
mov o1.xyzw, l(1.000000,1.000000,1.000000,1.000000)
mul r0.xyz, vDomain.yyyy, vicp[1][3].xyzx
mad r0.xyz, vicp[0][3].xyzx, vDomain.xxxx, r0.xyzx
mad o3.xyz, vicp[2][3].xyzx, vDomain.zzzz, r0.xyzx
mul r0.xyz, vDomain.yyyy, vicp[1][4].xyzx
mad r0.xyz, vicp[0][4].xyzx, vDomain.xxxx, r0.xyzx
mad r0.xyz, vicp[2][4].xyzx, vDomain.zzzz, r0.xyzx
dp3 r0.w, r0.xyzx, r0.xyzx
rsq r0.w, r0.w
mul o4.xyz, r0.wwww, r0.xyzx
mov o5.x, vpc0.y
ret 
// Approximately 21 instruction slots used
#endif

const BYTE Domain_Shader[] =
{
     68,  88,  66,  67,  99, 169, 
      8, 204,  63, 253, 117, 201, 
    146, 220, 207,   8, 208, 175, 
    138,  60,   1,   0,   0,   0, 
    104,   7,   0,   0,   6,   0, 
      0,   0,  56,   0,   0,   0, 
     88,   1,   0,   0,  16,   2, 
      0,   0, 200,   2,   0,   0, 
    160,   3,   0,   0, 204,   6, 
      0,   0,  82,  68,  69,  70, 
     24,   1,   0,   0,   1,   0, 
      0,   0, 100,   0,   0,   0, 
      1,   0,   0,   0,  60,   0, 
      0,   0,   0,   5,  83,  68, 
      0,   1,   0,   0, 228,   0, 
      0,   0,  82,  68,  49,  49, 
     60,   0,   0,   0,  24,   0, 
      0,   0,  32,   0,   0,   0, 
     40,   0,   0,   0,  36,   0, 
      0,   0,  12,   0,   0,   0, 
      0,   0,   0,   0,  92,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   1,   0,   0,   0, 
     79,  66,  74,  69,  67,  84, 
      0, 171,  92,   0,   0,   0, 
      1,   0,   0,   0, 124,   0, 
      0,   0,  64,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0, 164,   0,   0,   0, 
      0,   0,   0,   0,  64,   0, 
      0,   0,   2,   0,   0,   0, 
    192,   0,   0,   0,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
     76, 111,  99,  97, 108,  95, 
    119, 111, 114, 108, 100,  77, 
     97, 116, 114, 105, 120,   0, 
    102, 108, 111,  97, 116,  52, 
    120,  52,   0, 171,   2,   0, 
      3,   0,   4,   0,   4,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
    182,   0,   0,   0,  77, 105, 
     99, 114, 111, 115, 111, 102, 
    116,  32,  40,  82,  41,  32, 
     72,  76,  83,  76,  32,  83, 
    104,  97, 100, 101, 114,  32, 
     67, 111, 109, 112, 105, 108, 
    101, 114,  32,  57,  46,  51, 
     48,  46,  57,  50,  48,  48, 
     46,  50,  48,  55,  56,  57, 
      0, 171,  73,  83,  71,  78, 
    176,   0,   0,   0,   5,   0, 
      0,   0,   8,   0,   0,   0, 
    128,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   0,   0, 
      0,   0,  15,   0,   0,   0, 
    140,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   1,   0, 
      0,   0,  15,   0,   0,   0, 
    146,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   2,   0, 
      0,   0,   7,   7,   0,   0, 
    155,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   3,   0, 
      0,   0,   7,   7,   0,   0, 
    168,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   4,   0, 
      0,   0,   7,   7,   0,   0, 
     83,  86,  95,  80,  79,  83, 
     73,  84,  73,  79,  78,   0, 
     67,  79,  76,  79,  82,   0, 
     80,  79,  83,  73,  84,  73, 
     79,  78,   0,  84,  69,  88, 
     84,  85,  82,  69,  67,  79, 
     79,  82,  68,   0,  78,  79, 
     82,  77,  65,  76,   0, 171, 
     80,  67,  83,  71, 176,   0, 
      0,   0,   5,   0,   0,   0, 
      8,   0,   0,   0, 128,   0, 
      0,   0,   0,   0,   0,   0, 
     13,   0,   0,   0,   3,   0, 
      0,   0,   0,   0,   0,   0, 
      1,   0,   0,   0, 142,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
      2,   2,   0,   0, 128,   0, 
      0,   0,   1,   0,   0,   0, 
     13,   0,   0,   0,   3,   0, 
      0,   0,   1,   0,   0,   0, 
      1,   0,   0,   0, 128,   0, 
      0,   0,   2,   0,   0,   0, 
     13,   0,   0,   0,   3,   0, 
      0,   0,   2,   0,   0,   0, 
      1,   0,   0,   0, 153,   0, 
      0,   0,   0,   0,   0,   0, 
     14,   0,   0,   0,   3,   0, 
      0,   0,   3,   0,   0,   0, 
      1,   0,   0,   0,  83,  86, 
     95,  84, 101, 115, 115,  70, 
     97,  99, 116, 111, 114,   0, 
     84,  69,  83,  83,  69,  76, 
     65,  84,  69,  68,   0,  83, 
     86,  95,  73, 110, 115, 105, 
    100, 101,  84, 101, 115, 115, 
     70,  97,  99, 116, 111, 114, 
      0, 171, 171, 171,  79,  83, 
     71,  78, 208,   0,   0,   0, 
      6,   0,   0,   0,   8,   0, 
      0,   0, 152,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   3,   0,   0,   0, 
      0,   0,   0,   0,  15,   0, 
      0,   0, 164,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   3,   0,   0,   0, 
      1,   0,   0,   0,  15,   0, 
      0,   0, 170,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   3,   0,   0,   0, 
      2,   0,   0,   0,   7,   8, 
      0,   0, 179,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   3,   0,   0,   0, 
      3,   0,   0,   0,   7,   8, 
      0,   0, 192,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   3,   0,   0,   0, 
      4,   0,   0,   0,   7,   8, 
      0,   0, 199,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
      5,   0,   0,   0,   1,  14, 
      0,   0,  83,  86,  95,  80, 
     79,  83,  73,  84,  73,  79, 
     78,   0,  67,  79,  76,  79, 
     82,   0,  80,  79,  83,  73, 
     84,  73,  79,  78,   0,  84, 
     69,  88,  84,  85,  82,  69, 
     67,  79,  79,  82,  68,   0, 
     78,  79,  82,  77,  65,  76, 
      0,  66,  70,  85,  90,  69, 
     68,   0, 171, 171,  83,  72, 
     69,  88,  36,   3,   0,   0, 
     80,   0,   4,   0, 201,   0, 
      0,   0, 147,  24,   0,   1, 
    149,  16,   0,   1, 106,   8, 
      0,   1,  89,   0,   0,   4, 
     70, 142,  32,   0,   0,   0, 
      0,   0,   4,   0,   0,   0, 
     95,   0,   0,   3,  34, 176, 
     17,   0,   0,   0,   0,   0, 
     95,   0,   0,   2, 114, 192, 
      1,   0,  95,   0,   0,   4, 
    114, 144,  33,   0,   3,   0, 
      0,   0,   2,   0,   0,   0, 
     95,   0,   0,   4, 114, 144, 
     33,   0,   3,   0,   0,   0, 
      3,   0,   0,   0,  95,   0, 
      0,   4, 114, 144,  33,   0, 
      3,   0,   0,   0,   4,   0, 
      0,   0, 103,   0,   0,   4, 
    242,  32,  16,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
    101,   0,   0,   3, 242,  32, 
     16,   0,   1,   0,   0,   0, 
    101,   0,   0,   3, 114,  32, 
     16,   0,   2,   0,   0,   0, 
    101,   0,   0,   3, 114,  32, 
     16,   0,   3,   0,   0,   0, 
    101,   0,   0,   3, 114,  32, 
     16,   0,   4,   0,   0,   0, 
    101,   0,   0,   3,  18,  32, 
     16,   0,   5,   0,   0,   0, 
    104,   0,   0,   2,   2,   0, 
      0,   0,  56,   0,   0,   7, 
    114,   0,  16,   0,   0,   0, 
      0,   0,  86, 197,   1,   0, 
     70, 146,  33,   0,   1,   0, 
      0,   0,   2,   0,   0,   0, 
     50,   0,   0,   9, 114,   0, 
     16,   0,   0,   0,   0,   0, 
     70, 146,  33,   0,   0,   0, 
      0,   0,   2,   0,   0,   0, 
      6, 192,   1,   0,  70,   2, 
     16,   0,   0,   0,   0,   0, 
     50,   0,   0,   9, 114,   0, 
     16,   0,   0,   0,   0,   0, 
     70, 146,  33,   0,   2,   0, 
      0,   0,   2,   0,   0,   0, 
    166, 202,   1,   0,  70,   2, 
     16,   0,   0,   0,   0,   0, 
     56,   0,   0,   8, 242,   0, 
     16,   0,   1,   0,   0,   0, 
     86,   5,  16,   0,   0,   0, 
      0,   0,  70, 142,  32,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,  50,   0,   0,  10, 
    242,   0,  16,   0,   1,   0, 
      0,   0,   6,   0,  16,   0, 
      0,   0,   0,   0,  70, 142, 
     32,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,  70,  14, 
     16,   0,   1,   0,   0,   0, 
     50,   0,   0,  10, 242,   0, 
     16,   0,   0,   0,   0,   0, 
    166,  10,  16,   0,   0,   0, 
      0,   0,  70, 142,  32,   0, 
      0,   0,   0,   0,   2,   0, 
      0,   0,  70,  14,  16,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   8, 242,   0,  16,   0, 
      0,   0,   0,   0,  70,  14, 
     16,   0,   0,   0,   0,   0, 
     70, 142,  32,   0,   0,   0, 
      0,   0,   3,   0,   0,   0, 
     54,   0,   0,   5, 242,  32, 
     16,   0,   0,   0,   0,   0, 
     70,  14,  16,   0,   0,   0, 
      0,   0,  54,   0,   0,   5, 
    114,  32,  16,   0,   2,   0, 
      0,   0,  70,   2,  16,   0, 
      0,   0,   0,   0,  54,   0, 
      0,   8, 242,  32,  16,   0, 
      1,   0,   0,   0,   2,  64, 
      0,   0,   0,   0, 128,  63, 
      0,   0, 128,  63,   0,   0, 
    128,  63,   0,   0, 128,  63, 
     56,   0,   0,   7, 114,   0, 
     16,   0,   0,   0,   0,   0, 
     86, 197,   1,   0,  70, 146, 
     33,   0,   1,   0,   0,   0, 
      3,   0,   0,   0,  50,   0, 
      0,   9, 114,   0,  16,   0, 
      0,   0,   0,   0,  70, 146, 
     33,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   6, 192, 
      1,   0,  70,   2,  16,   0, 
      0,   0,   0,   0,  50,   0, 
      0,   9, 114,  32,  16,   0, 
      3,   0,   0,   0,  70, 146, 
     33,   0,   2,   0,   0,   0, 
      3,   0,   0,   0, 166, 202, 
      1,   0,  70,   2,  16,   0, 
      0,   0,   0,   0,  56,   0, 
      0,   7, 114,   0,  16,   0, 
      0,   0,   0,   0,  86, 197, 
      1,   0,  70, 146,  33,   0, 
      1,   0,   0,   0,   4,   0, 
      0,   0,  50,   0,   0,   9, 
    114,   0,  16,   0,   0,   0, 
      0,   0,  70, 146,  33,   0, 
      0,   0,   0,   0,   4,   0, 
      0,   0,   6, 192,   1,   0, 
     70,   2,  16,   0,   0,   0, 
      0,   0,  50,   0,   0,   9, 
    114,   0,  16,   0,   0,   0, 
      0,   0,  70, 146,  33,   0, 
      2,   0,   0,   0,   4,   0, 
      0,   0, 166, 202,   1,   0, 
     70,   2,  16,   0,   0,   0, 
      0,   0,  16,   0,   0,   7, 
    130,   0,  16,   0,   0,   0, 
      0,   0,  70,   2,  16,   0, 
      0,   0,   0,   0,  70,   2, 
     16,   0,   0,   0,   0,   0, 
     68,   0,   0,   5, 130,   0, 
     16,   0,   0,   0,   0,   0, 
     58,   0,  16,   0,   0,   0, 
      0,   0,  56,   0,   0,   7, 
    114,  32,  16,   0,   4,   0, 
      0,   0, 246,  15,  16,   0, 
      0,   0,   0,   0,  70,   2, 
     16,   0,   0,   0,   0,   0, 
     54,   0,   0,   5,  18,  32, 
     16,   0,   5,   0,   0,   0, 
     26, 176,  17,   0,   0,   0, 
      0,   0,  62,   0,   0,   1, 
     83,  84,  65,  84, 148,   0, 
      0,   0,  21,   0,   0,   0, 
      2,   0,   0,   0,   0,   0, 
      0,   0,  11,   0,   0,   0, 
      8,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      5,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   3,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   2,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0
};
