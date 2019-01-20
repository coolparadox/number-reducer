/*
 * Copyright 2018 Rafael Lorandi <coolparadox@gmail.com>
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

#include "protocol/protocol.hpp"
#include "strategy/exhaustion_error.hpp"
#include "strategy/strategy.hpp"

#include "number.hpp"

using deepnum::clarith::protocol::Protocol;
using deepnum::clarith::strategy::Strategy;
using deepnum::clarith::strategy::ExhaustionError;

namespace deepnum
{
namespace clarith
{

Number::Number(gsl::not_null<gsl::owner<strategy::Strategy*>> strategy)
        : strategy_(strategy)
{
}

Number::~Number()
{
    delete strategy_;
}

Protocol Number::Egest()
{
    try
    {
#if NUMBER_SANITY_CHECK
        return watcher_.Watch(strategy_->Egest());
#else
        return strategy_->Egest();
#endif
    }
    catch (ExhaustionError)
    {
        Strategy* aux = strategy_;
        strategy_ = strategy_->GetNewStrategy();
        delete aux;
    }
    return Egest();
}

}  // namespace clarith
}  // namespace deepnum
