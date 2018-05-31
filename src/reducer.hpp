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

#ifndef SRC_REDUCER_HPP_
#define SRC_REDUCER_HPP_

#include <memory>

namespace coolparadox {
namespace number {
namespace reducer {

enum class Protocol;

namespace strategy {
class Strategy;
}  // namespace strategy

/**
 * Plays a reducing strategy.
 */
class Reducer {
 public:
    /*
     * Constructs a strategy player.
     * \param[in] strategy The strategy to be played.
     * \see Strategy
     */
    explicit Reducer(std::unique_ptr<strategy::Strategy> strategy);

    /**
     * Retrieves next Protocol message from underlying strategy.
     * \return Extracted Protocol message.
     * \see Protocol
     */
    Protocol Reduce();

 private:
    std::unique_ptr<strategy::Strategy> strategy_;
};

}  // namespace reducer
}  // namespace number
}  // namespace coolparadox

#endif  // SRC_REDUCER_HPP_
