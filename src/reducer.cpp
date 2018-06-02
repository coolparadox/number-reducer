/*
 * Copyright 2018 Rafael Lorandi <coolparadox@gmail.com>
 *
 * This file is part of coolparadox-number-reducer, a library for handling
 * rational numbers in continued log format.
 * 
 * coolparadox-number-reducer is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * coolparadox-number-reducer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with coolparadox-number-reducer.  If not, see <http://www.gnu.org/licenses/>
 */

#include "reducer.hpp"

#include "protocol.hpp"
#include "strategy/exhaustion_error.hpp"
#include "strategy/strategy.hpp"

using coolparadox::number::reducer::strategy::Strategy;
using coolparadox::number::reducer::strategy::ExhaustionError;

namespace coolparadox {
namespace number {
namespace reducer {

Reducer::Reducer(std::unique_ptr<Strategy> strategy)
        : strategy_(std::move(strategy)) {
}

Protocol Reducer::Reduce() {
    try {
        return strategy_->Reduce();
    }
    catch (ExhaustionError) {
        strategy_ = strategy_->GetNewStrategy();
    }
    return strategy_->Reduce();
}

}  // namespace reducer
}  // namespace number
}  // namespace coolparadox
