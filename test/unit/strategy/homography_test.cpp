/*
 * Copyright 2019 Rafael Lorandi <coolparadox@gmail.com>
 *
 * This file is part of dn-clarith, a library for performing arithmetic
 * in continued logarithm representation.
 * 
 * dn-clarith is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * dn-clarith is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with dn-clarith.  If not, see <http://www.gnu.org/licenses/>
 */

#include "strategy/homography.hpp"

#include <CppUTest/TestHarness.h>

#include "number.hpp"
#include "protocol/protocol.hpp"
#include "strategy/zero.hpp"
#include "strategy/ratio.hpp"
#include "strategy/unavailable_error.hpp"
#include "strategy/exhaustion_error.hpp"
#include "strategy/undefined_ratio_error.hpp"
#include "util.hpp"

using deepnum::clarith::protocol::Protocol;
using deepnum::clarith::Number;

namespace deepnum
{
namespace clarith
{
namespace strategy
{

#define UNITY(x) Homography(x, 1, 0, 0, 1)
#define NEG_TWO new Number(new Ratio(-2, 1))
#define NEG_ONE new Number(new Ratio(-1, 1))
#define ZERO new Number(new Zero())
#define ONE_HALF new Number(new Ratio(1, 2))
#define ONE new Number(new Ratio(1, 1))
#define TWO new Number(new Ratio(2, 1))
#define THREE new Number(new Ratio(3, 1))
#define FOUR new Number(new Ratio(4, 1))
#define INFINITY new Number(new Ratio(1, 0))

TEST_GROUP(HomographyTest)
{
};

TEST(HomographyTest, SanitizesConstructorParameters)
{
    CHECK_THROWS(UndefinedRatioError, Homography(ONE, 0, 0, 0, 0));
}

TEST(HomographyTest, DoesNotProvideNewStrategyWhenNotExhausted)
{
    CHECK_THROWS(UnavailableError, UNITY(new Number(new Ratio(0, 1))).GetNewStrategy());
}

TEST(HomographyTest, DegeneratesToRatioOnEndOfInput)
{
    Homography s1 = UNITY(ZERO);
    CHECK_THROWS(ExhaustionError, s1.Egest());
    Strategy* s2 = s1.GetNewStrategy();
    CHECK_TRUE(dynamic_cast<Ratio*>(s2));
    delete s2;
}

TEST(HomographyTest, DegeneratesToRatioOnDiscardedInput)
{
    Homography s1(TWO, 0, 1, 0, 1);
    CHECK_THROWS(ExhaustionError, s1.Egest());
    Strategy* s2 = s1.GetNewStrategy();
    CHECK_TRUE(dynamic_cast<Ratio*>(s2));
    delete s2;
}

TEST(HomographyTest, DegeneratesToRatioOnDegeneratedHomography1)
{
    Homography s1(TWO, 3, 2, 3, 2);
    CHECK_THROWS(ExhaustionError, s1.Egest());
    Strategy* s2 = s1.GetNewStrategy();
    CHECK_TRUE(dynamic_cast<Ratio*>(s2));
    delete s2;
}

TEST(HomographyTest, DegeneratesToRatioOnDegeneratedHomography2)
{
    Homography s1(TWO, 3, 0, 2, 0);
    CHECK_THROWS(ExhaustionError, s1.Egest());
    Strategy* s2 = s1.GetNewStrategy();
    CHECK_TRUE(dynamic_cast<Ratio*>(s2));
    delete s2;
}

TEST(HomographyTest, ZeroByZeroIsUndefined)
{
    CHECK_THROWS(UndefinedRatioError, Util::Compare(ZERO, new Number(new Homography(ZERO, 1, 0, 0, 0))));
    CHECK_THROWS(UndefinedRatioError, Util::Compare(ZERO, new Number(new Homography(ZERO, 0, 0, 1, 0))));
}

TEST(HomographyTest, OneByZeroIsInfinity)
{
    LONGS_EQUAL(0, Util::Compare(INFINITY, new Number(new Homography(ONE, 1, 0, 0, 0))));
    LONGS_EQUAL(0, Util::Compare(INFINITY, new Number(new Homography(ZERO, 0, 1, 1, 0))));
}

TEST(HomographyTest, UnityFollowsInput)
{
    CHECK_THROWS(ExhaustionError, UNITY(ZERO).Egest());
    LONGS_EQUAL(Protocol::Amplify, UNITY(ONE_HALF).Egest());
    LONGS_EQUAL(Protocol::Uncover, UNITY(ONE).Egest());
    LONGS_EQUAL(Protocol::Turn, UNITY(TWO).Egest());
    LONGS_EQUAL(Protocol::Reflect, UNITY(NEG_ONE).Egest());
    LONGS_EQUAL(Protocol::Ground, UNITY(NEG_TWO).Egest());
}

TEST(HomographyTest, DoublesInput)
{
    LONGS_EQUAL(0, Util::Compare(new Number(new Homography(TWO, 2, 0, 0, 1)), FOUR));
}

TEST(HomographyTest, HalvesInput)
{
    LONGS_EQUAL(0, Util::Compare(new Number(new Homography(TWO, 1, 0, 0, 2)), ONE));
}

TEST(HomographyTest, AddsOneToInput)
{
    LONGS_EQUAL(0, Util::Compare(new Number(new Homography(TWO, 1, 1, 0, 1)), THREE));
}

TEST(HomographyTest, SubstractsOneFromInput)
{
    LONGS_EQUAL(0, Util::Compare(new Number(new Homography(TWO, 1, -1, 0, 1)), ONE));
}

TEST(HomographyTest, ReciprocatesInput)
{
    LONGS_EQUAL(0, Util::Compare(new Number(new Homography(TWO, 0, 1, 1, 0)), ONE_HALF));
}

TEST(HomographyTest, NegatesInput)
{
    LONGS_EQUAL(0, Util::Compare(new Number(new Homography(TWO, -1, 0, 0, 1)), NEG_TWO));
}

// FIXME: add overflow tests

}  // namespace strategy
}  // namespace clarith
}  // namespace deepnum
