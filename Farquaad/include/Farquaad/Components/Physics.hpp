// Copyright 2015 Bablawn3d5

#pragma once

#include <entityx/entityx.h>
#include <Box2D/Box2D.h>
#include <bitset>
#include <string>

enum CollisionCategory {
  CATEGORY_1 = 1 << 0,
  CATEGORY_2 = 1 << 1,
  CATEGORY_3 = 1 << 2,
  CATEGORY_4 = 1 << 3,
  CATEGORY_5 = 1 << 4,
  CATEGORY_6 = 1 << 5,
  CATEGORY_7 = 1 << 6,
  CATEGORY_8 = 1 << 7,
  CATEGORY_9 = 1 << 8,
  CATEGORY_10 = 1 << 9,
  CATEGORY_11 = 1 << 10,
  CATEGORY_12 = 1 << 11,
  CATEGORY_13 = 1 << 12,
  CATEGORY_14 = 1 << 13,
  CATEGORY_15 = 1 << 14,
  CATEGORY_16 = 1 << 15
};

// TODO(SMA) : Move me somewhere else.
template <typename ToCheck, std::size_t ExpectedSize, 
  std::size_t RealSize = sizeof(ToCheck)>
void check_size() {
  static_assert(ExpectedSize == RealSize, "Size is off!");
}

// HACK(SMA) : I can't define enum CollisionCategory : uint16, as this breaks
// serlization for whatever reason. So for now just assume uint16 and work 
// from there.
// using bit_type = std::underlying_type<CollisionCategory>::type;
using bit_type = uint16;
using bitset_type = std::bitset<sizeof(bit_type) * CHAR_BIT>;
struct CollisionCategoryBitset : public bitset_type {
  using type = bit_type;
  CollisionCategoryBitset(bit_type mask = 0) : bitset_type(mask) {
    check_size<type, sizeof(uint16)>();
  }
  explicit CollisionCategoryBitset(std::string bitstring) : bitset_type(bitstring) {
    check_size<type, sizeof(uint16)>();
  }

  void set_bits(const type bits) {
    this->set();
    this->operator&=(bitset_type(bits));
  }

  bit_type get_bits() const {
    return this->operator bit_type();
  }

  operator bit_type() const { return static_cast<bit_type>(this->to_ulong()); }
};

struct Physics {
public:
    // Pass in the height/width as pixels
    explicit Physics(
        b2BodyType bodyType = b2_staticBody,
        int widthPixels = 0,
        int heightPixels = 0) :
        bodyType(bodyType),
        size({ widthPixels , heightPixels }) {
        halfSize = 0.5f * (sf::Vector2f)size;
        isDirty = true;
    }
    ~Physics() {}

    bool isDirty;

    b2Body* body = NULL;
    b2BodyType bodyType;
    sf::Vector2f halfSize;
    sf::Vector2i size;

    CollisionCategory collisionCategory = CollisionCategory::CATEGORY_1;
    CollisionCategoryBitset collisionMask = CollisionCategory::CATEGORY_1;
    int collisionGroup = 0;
};
