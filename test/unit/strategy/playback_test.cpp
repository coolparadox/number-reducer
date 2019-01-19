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

#include <memory>
#include <forward_list>
#include <CppUTest/TestHarness.h>

#include "protocol/protocol.hpp"
#include "protocol/violation_error.hpp"
#include "strategy/infinity.hpp"
#include "strategy/playback.hpp"
#include "strategy/exhaustion_error.hpp"
#include "strategy/unavailable_error.hpp"

using deepnum::clarith::protocol::Protocol;
using deepnum::clarith::protocol::ViolationError;

namespace deepnum
{
namespace clarith
{
namespace strategy
{

TEST_GROUP(PlaybackTest)
{
};

TEST(PlaybackTest, ReachesInfinity)
{
    Playback strategy(std::unique_ptr<std::forward_list<Protocol>>(new std::forward_list<Protocol> {}));
    CHECK_THROWS(ExhaustionError, strategy.Egest());
    CHECK_TRUE(dynamic_cast<Infinity*>(strategy.GetNewStrategy().get()));
}

TEST(PlaybackTest, DoesNotOfferPrematureStrategy)
{
    CHECK_THROWS(UnavailableError, Playback(std::unique_ptr<std::forward_list<Protocol>>(new std::forward_list<Protocol> { Protocol::kZero })).GetNewStrategy());
}

TEST(PlaybackTest, ReplaysTwo)
{
    LONGS_EQUAL(Protocol::kTwo, Playback(std::unique_ptr<std::forward_list<Protocol>>(new std::forward_list<Protocol> { Protocol::kTwo })).Egest());
}

TEST(PlaybackTest, ReplaysOne)
{
    LONGS_EQUAL(Protocol::kOne, Playback(std::unique_ptr<std::forward_list<Protocol>>(new std::forward_list<Protocol> { Protocol::kOne })).Egest());
}

TEST(PlaybackTest, ReplaysZero)
{
    LONGS_EQUAL(Protocol::kZero, Playback(std::unique_ptr<std::forward_list<Protocol>>(new std::forward_list<Protocol> { Protocol::kZero })).Egest());
}

TEST(PlaybackTest, ReplaysNegative)
{
    LONGS_EQUAL(Protocol::kNeg, Playback(std::unique_ptr<std::forward_list<Protocol>>(new std::forward_list<Protocol> { Protocol::kNeg })).Egest());
}

TEST(PlaybackTest, DetectsViolationErrorOnNonFinalEndMessage)
{
    {
        Playback strategy(std::unique_ptr<std::forward_list<Protocol>>(new std::forward_list<Protocol> { Protocol::kEnd, Protocol::kOne }));
        LONGS_EQUAL(Protocol::kEnd, strategy.Egest());
        CHECK_THROWS(ViolationError, strategy.Egest());
    }
}

TEST(PlaybackTest, DetectsViolationErrorOnNonInitialNegMessage)
{
    {
        Playback strategy(std::unique_ptr<std::forward_list<Protocol>>(new std::forward_list<Protocol> { Protocol::kOne, Protocol::kNeg }));
        LONGS_EQUAL(Protocol::kOne, strategy.Egest());
        CHECK_THROWS(ViolationError, strategy.Egest());
    }
}

TEST(PlaybackTest, DetectsViolationErrorOnNonInitialZeroMessage)
{
    {
        Playback strategy(std::unique_ptr<std::forward_list<Protocol>>(new std::forward_list<Protocol> { Protocol::kOne, Protocol::kZero }));
        LONGS_EQUAL(Protocol::kOne, strategy.Egest());
        CHECK_THROWS(ViolationError, strategy.Egest());
    }
}

TEST(PlaybackTest, DetectsViolationErrorOnFinalTwoMessage)
{
    {
        Playback strategy(std::unique_ptr<std::forward_list<Protocol>>(new std::forward_list<Protocol> { Protocol::kTwo, Protocol::kEnd }));
        LONGS_EQUAL(Protocol::kTwo, strategy.Egest());
        CHECK_THROWS(ViolationError, strategy.Egest());
    }
    {
        Playback strategy(std::unique_ptr<std::forward_list<Protocol>>(new std::forward_list<Protocol> { Protocol::kTwo }));
        LONGS_EQUAL(Protocol::kTwo, strategy.Egest());
        CHECK_THROWS(ViolationError, strategy.Egest());
    }
}

}  // namespace strategy
}  // namespace clarith
}  // namespace deepnum
