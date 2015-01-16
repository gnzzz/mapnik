/*****************************************************************************
 *
 * This file is part of Mapnik (c++ mapping toolkit)
 *
 * Copyright (C) 2015 Artem Pavlenko
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 *****************************************************************************/

// mapnik
#include <mapnik/json/extract_bounding_box_grammar.hpp>

// boost
#include <boost/spirit/include/qi_omit.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/repository/include/qi_seek.hpp>
#include <boost/spirit/repository/include/qi_iter_pos.hpp>
// stl
#include <iostream>
#include <string>

namespace mapnik { namespace json {

namespace repo = boost::spirit::repository;

template <typename Iterator, typename ErrorHandler>
extract_bounding_box_grammar<Iterator, ErrorHandler>::extract_bounding_box_grammar()
    : extract_bounding_box_grammar::base_type(start,"bounding boxes")
{
    qi::lit_type lit;
    qi::double_type double_;
    qi::_val_type _val;
    qi::_1_type _1;
    qi::_2_type _2;
    qi::_3_type _3;
    qi::_4_type _4;
    qi::omit_type omit;
    qi::_r1_type _r1;
    qi::_r2_type _r2;
    qi::_r3_type _r3;
    qi::_a_type _a;
    qi::_b_type _b;
    qi::skip_type skip;
    qi::lexeme_type lexeme;
    boost::spirit::repository::qi::seek_type seek;
    boost::spirit::repository::qi::iter_pos_type iter_pos;
    using qi::fail;
    using qi::on_error;
    using boost::phoenix::push_back;

    start = features(_r1)
        ;
    features = iter_pos[_a = _1] >> -(lit('{') >> -lit("\"type\"")
                                      >> lit(':') >> lit("\"FeatureCollection\"")
                                      >> lit(',') >> lit("\"features\"")
                                      >> lit(':'))
                                 >> lit('[') >> *(seek[lexeme[skip[iter_pos[_b = _1] >> lit('{') >> lit("\"type\"") >> lit(':') >> lit("\"Feature\"")]]]
                                                  >> feature(_r1, _a, _b))
        ;

    feature = bounding_box(_r1, offset(_r2, _r3))
        ;
    bounding_box = seek["\"coordinates\""] >> lit(':') >> coords[push_box(_r1, _r2, _1)]
        ;
    coords = (rings_array(_a) | rings (_a) | ring(_a) | pos[calculate_bounding_box(_a,_1)])[_val = _a]
        ;
    pos = lit('[') > -(double_ > lit(',') > double_) > omit[*(lit(',') > double_)] > lit(']')
        ;
    ring = lit('[') >> pos[calculate_bounding_box(_r1,_1)] % lit(',') > lit(']')
        ;
    rings = lit('[') >> ring(_r1)  % lit(',') > lit(']')
        ;
    rings_array = lit('[') >> rings(_r1) % lit(',') > lit(']')
        ;

    coords.name("Coordinates");
    pos.name("Position");
    ring.name("Ring");
    rings.name("Rings");
    rings_array.name("Rings array");

    // error handler
    on_error<fail>(coords, error_handler(_1, _2, _3, _4));
}

}}