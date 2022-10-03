#pragma once
/*!
 * Matches the bucket-id enum in GOAL
 */
namespace jak1 {
enum class BucketId {
  BUCKET0 = 0,
  BUCKET1 = 1,
  // 2
  SKY_DRAW = 3,
  OCEAN_MID_AND_FAR = 4,
  TFRAG_TEX_LEVEL0 = 5,
  TFRAG_LEVEL0 = 6,
  TFRAG_NEAR_LEVEL0 = 7,
  TIE_NEAR_LEVEL0 = 8,
  TIE_LEVEL0 = 9,
  MERC_TFRAG_TEX_LEVEL0 = 10,
  GENERIC_TFRAG_TEX_LEVEL0 = 11,
  TFRAG_TEX_LEVEL1 = 12,
  TFRAG_LEVEL1 = 13,
  TFRAG_NEAR_LEVEL1 = 14,
  TIE_NEAR_LEVEL1 = 15,
  TIE_LEVEL1 = 16,
  MERC_TFRAG_TEX_LEVEL1 = 17,
  GENERIC_TFRAG_TEX_LEVEL1 = 18,
  SHRUB_TEX_LEVEL0 = 19,
  SHRUB_NORMAL_LEVEL0 = 20,
  // 21
  SHRUB_BILLBOARD_LEVEL0 = 22,
  SHRUB_TRANS_LEVEL0 = 23,
  SHRUB_GENERIC_LEVEL0 = 24,
  SHRUB_TEX_LEVEL1 = 25,
  SHRUB_NORMAL_LEVEL1 = 26,
  // 27
  SHRUB_BILLBOARD_LEVEL1 = 28,
  SHRUB_TRANS_LEVEL1 = 29,
  SHRUB_GENERIC_LEVEL1 = 30,
  ALPHA_TEX_LEVEL0 = 31,
  TFRAG_TRANS0_AND_SKY_BLEND_LEVEL0 = 32,
  // 33
  TFRAG_DIRT_LEVEL0 = 34,
  // 35
  TFRAG_ICE_LEVEL0 = 36,
  // 37
  ALPHA_TEX_LEVEL1 = 38,
  TFRAG_TRANS1_AND_SKY_BLEND_LEVEL1 = 39,
  // 40
  TFRAG_DIRT_LEVEL1 = 41,
  // 42
  TFRAG_ICE_LEVEL1 = 43,
  // 44
  MERC_AFTER_ALPHA = 45,
  GENERIC_ALPHA = 46,
  SHADOW = 47,
  PRIS_TEX_LEVEL0 = 48,
  MERC_PRIS_LEVEL0 = 49,
  GENERIC_PRIS_LEVEL0 = 50,
  PRIS_TEX_LEVEL1 = 51,
  MERC_PRIS_LEVEL1 = 52,
  GENERIC_PRIS_LEVEL1 = 53,
  MERC_EYES_AFTER_PRIS = 54,
  MERC_AFTER_PRIS = 55,
  GENERIC_PRIS = 56,
  WATER_TEX_LEVEL0 = 57,
  MERC_WATER_LEVEL0 = 58,
  GENERIC_WATER_LEVEL0 = 59,
  WATER_TEX_LEVEL1 = 60,
  MERC_WATER_LEVEL1 = 61,
  GENERIC_WATER_LEVEL1 = 62,
  OCEAN_NEAR = 63,
  DEPTH_CUE = 64,
  PRE_SPRITE_TEX = 65,  // maybe it's just common textures?
  SPRITE = 66,
  DEBUG = 67,
  DEBUG_NO_ZBUF = 68,
  SUBTITLE = 69,
  MAX_BUCKETS = 70
};
}

namespace jak2 {

enum class BucketId {
  SPECIAL_BUCKET_2 = 2,
  TFRAG_L0_TFRAG = 8,
  TIE_L0_TFRAG = 9,
  TFRAG_L1_TFRAG = 19,
  TIE_L1_TFRAG = 20,
  SHRUB_L0_SHRUB = 74,
  SHRUB_L1_SHRUB = 83,
  TFRAG_T_L0_ALPHA = 128,
  DEBUG_NO_ZBUF1 = 318,
  DEBUG2 = 324,
  DEBUG_NO_ZBUF2 = 325,
  DEBUG3 = 326,
  MAX_BUCKETS = 327
};
}

enum class BucketCategory {
  TFRAG,
  TIE,
  SHRUB,
  TEX,
  MERC,
  GENERIC,
  SPRITE,
  OCEAN,
  OTHER,
  MAX_CATEGORIES
};

constexpr const char* BUCKET_CATEGORY_NAMES[(int)BucketCategory::MAX_CATEGORIES] = {
    "tfrag", "tie", "shrub", "tex", "merc", "generic", "sprite", "ocean", "other"};
