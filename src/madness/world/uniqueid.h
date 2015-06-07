/*
  This file is part of MADNESS.

  Copyright (C) 2007,2010 Oak Ridge National Laboratory

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA

  For more information please contact:

  Robert J. Harrison
  Oak Ridge National Laboratory
  One Bethel Valley Road
  P.O. Box 2008, MS-6367

  email: harrisonrj@ornl.gov
  tel:   865-241-3937
  fax:   865-572-0680
*/

/**
 \file uniqueid.h
 \brief \todo Brief description needed.
 \ingroup world
*/

#ifndef MADNESS_WORLD_UNIQUEID_H__INCLUDED
#define MADNESS_WORLD_UNIQUEID_H__INCLUDED

#include <cstddef>
#include <iostream>
#include <madness/world/worldhash.h>

namespace madness {

    class World;

    /// \addtogroup world
    /// @{

    /// Class for unique global IDs.
    class uniqueidT {
        friend class World;
    private:
        unsigned long worldid; ///< ID of the \c World the object belongs to.
        unsigned long objid; ///< ID of the object.

        /// Constructor that sets the world and object IDs.

        /// \param[in] worldid The ID of the \c World the object belongs to.
        /// \param[in] objid The ID of the object.
        uniqueidT(unsigned long worldid, unsigned long objid)
                : worldid(worldid), objid(objid) {};

    public:
        /// Constructor.
        uniqueidT()
                : worldid(0), objid(0) {};

        /// Equality comparison operator

        /// \param[in] other The \c uniqueidT to compare against.
        /// \return True if both `uniqueidT`s are the same.
        bool operator==(const uniqueidT& other) const {
            return objid==other.objid && worldid==other.worldid;
        }

        /// \todo Brief description needed.

        /// \todo Return description needed (probably obvious from the brief description).
        /// \return Description needed.
        operator bool() const {
            return objid!=0;
        }

        /// Serialize a unique ID object.

        /// \tparam Archive The archive type.
        /// \param[in,out] ar The archive.
        template <typename Archive>
        void serialize(Archive& ar) {
            ar & worldid & objid;
        }

        /// Access the \c World ID.

        /// \return The \c World ID.
        unsigned long get_world_id() const {
            return worldid;
        }

        /// Access the object ID.

        /// \return The object ID.
        unsigned long get_obj_id() const {
            return objid;
        }

        /// Stream insertion function for a \c uniqueidT.

        /// \param[in,out] s The output stream.
        /// \param[in] id The \c uniqueidT to be output.
        /// \return The output stream (for chaining).
        friend std::ostream& operator<<(std::ostream& s, const uniqueidT& id) {
            s << "{" << id.get_world_id() << "," << id.get_obj_id() << "}";
            return s;
        }
    }; // class uniqueidT

    /// Hash a \c uniqueidT.

    /// \param[in] id The \c uniqueidT.
    /// \return The hash.
    inline hashT hash_value(const uniqueidT& id) {
        hashT seed = hash_value(id.get_world_id());
        detail::combine_hash(seed, hash_value(id.get_obj_id()));

        return seed;
    }

    /// @}

}  // namespace madness


#endif // MADNESS_WORLD_UNIQUEID_H__INCLUDED
