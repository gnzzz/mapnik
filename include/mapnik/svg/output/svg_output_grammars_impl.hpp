/*****************************************************************************
 *
 * This file is part of Mapnik (c++ mapping toolkit)
 *
 * Copyright (C) 2014 Artem Pavlenko
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

// NOTE: This is an implementation header file and is only meant to be included
//    from implementation files. It therefore doesn't have an include guard.

// mapnik
#include <mapnik/svg/output/svg_output_grammars.hpp>

// boost
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-local-typedef"
#include <boost/spirit/include/karma.hpp>
#include <boost/fusion/include/std_pair.hpp>
#pragma GCC diagnostic pop

namespace mapnik { namespace svg {

using namespace boost::spirit;

// Path attributes
template <typename OutputIterator>
svg_path_attributes_grammar<OutputIterator>::svg_path_attributes_grammar()
        : svg_path_attributes_grammar::base_type(svg_path_attributes)
{
    karma::lit_type lit;
    karma::double_type double_;
    karma::string_type kstring;

    svg_path_attributes =
        lit("fill=\"") << kstring << lit("\" ")
        << lit("fill-opacity=\"") << double_ << lit("\" ")
        << lit("stroke=\"") << kstring << lit("\" ")
        << lit("stroke-opacity=\"") << double_ << lit("\" ")
        << lit("stroke-width=\"") << double_ << lit("px\" ")
        << lit("stroke-linecap=\"") << kstring << lit("\" ")
        << lit("stroke-linejoin=\"") << kstring << lit("\" ")
        << lit("stroke-dashoffset=\"") << double_ << lit("px") << lit("\"");
}

template <typename OutputIterator>
svg_path_dash_array_grammar<OutputIterator>::svg_path_dash_array_grammar()
        : svg_path_dash_array_grammar::base_type(svg_path_dash_array)
{
    karma::double_type double_;
    karma::lit_type lit;

    svg_path_dash_array = lit("stroke-dasharray=\"") <<
            -((double_ << lit(',') << double_) % lit(',')) << lit('"');
}

// Rectangle attributes
template <typename OutputIterator>
svg_rect_attributes_grammar<OutputIterator>::svg_rect_attributes_grammar()
    : svg_rect_attributes_grammar::base_type(svg_rect_attributes)
{
    karma::lit_type lit;
    karma::int_type int_;
    karma::string_type kstring;

    svg_rect_attributes =
           lit("x=\"") << int_ << lit("\" ")
        << lit("y=\"") << int_ << lit("\" ")
        << lit("width=\"") << int_ << lit("px\" ")
        << lit("height=\"") << int_ << lit("px\" ")
        << lit("fill=\"") << kstring << lit("\"");
}

// Root element attributes
template <typename OutputIterator>
svg_root_attributes_grammar<OutputIterator>::svg_root_attributes_grammar()
        : svg_root_attributes_grammar::base_type(svg_root_attributes)
{
    karma::lit_type lit;
    karma::int_type int_;
    karma::string_type kstring;
    karma::double_type double_;

    svg_root_attributes =
        lit("width=\"") << int_ << lit("px\" ")
        << lit("height=\"") << int_ << lit("px\" ")
        << lit("version=\"") << double_ << lit("\" ")
        << lit("xmlns=\"") << kstring << lit("\" ")
        << lit("xmlns:inkscape=\"http://www.inkscape.org/namespaces/inkscape\" ")
        << lit("xmlns:xlink=\"http://www.w3.org/1999/xlink\"");
}


// Text stuff
template <typename OutputIterator>
svg_text_attributes_grammar<OutputIterator>::svg_text_attributes_grammar()
        : svg_text_attributes_grammar::base_type(svg_text_attributes)
{
        karma::lit_type lit;
        karma::string_type kstring;
        karma::double_type double_;

        svg_text_attributes =
                lit(" font-family=\"") << kstring << lit("\" ")
                << lit("font-size=\"") << double_ << lit("px\" ")
//                << lit(" fill=") << kstring 
//                << lit(" fill-opacity=") << double_
//                << lit(" style=\"") 
//                    << lit("stroke: ") << kstring 
//                    << lit("; stroke-opacity: ") << double_ 
//                    << "; stroke-width: " << double_ 
//                << lit("\"")
                << lit("text-transform=\"") << kstring << lit("\" ");
    
}

template <typename OutputIterator>
svg_text_fill_attributes_grammar<OutputIterator>::svg_text_fill_attributes_grammar()
        : svg_text_fill_attributes_grammar::base_type(svg_text_attributes_fill)
{
        karma::lit_type lit;
        karma::string_type kstring;
        karma::double_type double_;

        svg_text_attributes_fill =
                lit(" font-family=\"") << kstring << lit("\" ")
                << lit(" font-size=\"") << double_ << lit("px\" ")
                << lit(" text-transform=\"") << kstring << lit("\" ")
                << lit(" fill=\"") << kstring  << lit("\" ")
                << lit(" fill-opacity=\"") << double_ << lit("\" ");
}

template <typename OutputIterator>
svg_text_stroke_attributes_grammar<OutputIterator>::svg_text_stroke_attributes_grammar()
        : svg_text_stroke_attributes_grammar::base_type(svg_text_attributes_stroke)
{
        karma::lit_type lit;
        karma::string_type kstring;
        karma::double_type double_;

        svg_text_attributes_stroke =
                lit(" font-family=\"") << kstring << lit("\" ")
                << lit("font-size=\"") << double_ << lit("px\" ")
                << lit("text-transform=\"") << kstring << lit("\" ")
                << lit("fill=\"none\" ")
                << lit("style=\"") 
                    << lit("stroke: ") << kstring << lit("; ")
                    << lit("stroke-opacity: ") << double_ << lit("; ")
                    << "stroke-width: " << double_
                << lit("\"");
}


}
}
